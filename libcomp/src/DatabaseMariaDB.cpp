/**
 * @file libcomp/src/DatabaseMariaDB.cpp
 * @ingroup libcomp
 *
 * @author HACKfrost
 *
 * @brief Class to handle a MariaDB database.
 *
 * This file is part of the COMP_hack Library (libcomp).
 *
 * Copyright (C) 2012-2020 COMP_hack Team <compomega@tutanota.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "DatabaseMariaDB.h"

#ifndef EXOTIC_PLATFORM

// libcomp Includes
#include "BaseLog.h"
#include "BaseServer.h"
#include "DataStore.h"
#include "DatabaseBind.h"
#include "DatabaseQueryMariaDB.h"

// config-win.h and my_global.h redefine bool unless explicitly defined
#define bool bool

// MariaDB Includes
#include <mysql.h>

using namespace libcomp;

static libcomp::String ConnectionString(MYSQL* pConnection) {
  return libcomp::String("{%1-%2}")
      .Arg((uint32_t)(((uint64_t)pConnection) >> 32), 8, 16, '0')
      .Arg((uint32_t)((uint64_t)pConnection), 8, 16, '0');
}

DatabaseMariaDB::DatabaseMariaDB(
    const std::shared_ptr<objects::DatabaseConfigMariaDB>& config)
    : Database(std::dynamic_pointer_cast<objects::DatabaseConfig>(config)) {}

DatabaseMariaDB::~DatabaseMariaDB() { Close(); }

bool DatabaseMariaDB::Open() {
  return ConnectToDatabase(GetConnection(false), "");
}

bool DatabaseMariaDB::Close() {
  bool result = true;

  std::lock_guard<std::mutex> lock(mConnectionLock);
  for (auto kv : mConnections) {
    result &= Close(kv.second);
  }
  mConnections.clear();

  return result;
}

bool DatabaseMariaDB::Close(MYSQL*& connection) {
  if (nullptr != connection && nullptr != connection) {
    mysql_close(connection);

    LogDatabaseDebug([&]() {
      return String("Database connection closed: %1\n")
          .Arg(ConnectionString(connection));
    });

    connection = nullptr;
  } else {
    LogDatabaseDebug([&]() {
      return String("Database connection NOT closed: %1\n")
          .Arg(ConnectionString(connection));
    });
  }

  return true;
}

bool DatabaseMariaDB::IsOpen() const {
  // Connections are only added when they are valid
  return mConnections.size() > 0;
}

DatabaseQuery DatabaseMariaDB::Prepare(const String& query) {
  auto connection = GetConnection(true);
  return DatabaseQuery(new DatabaseQueryMariaDB(connection), query);
}

bool DatabaseMariaDB::Exists() {
  DatabaseQuery q = Prepare(
      String(
          "SELECT 1 FROM information_schema.TABLES WHERE TABLE_SCHEMA = '%1';")
          .Arg(
              std::dynamic_pointer_cast<objects::DatabaseConfigMariaDB>(mConfig)
                  ->GetDatabaseName()));

  if (!q.Execute()) {
    LogDatabaseCritical([&]() {
      return String("Failed to query for database: %1\n").Arg(GetLastError());
    });

    return false;
  }

  std::list<std::unordered_map<std::string, std::vector<char>>> results;
  q.Next();

  return q.GetRows(results) && results.size() > 0;
}

bool DatabaseMariaDB::Setup(bool rebuild,
                            const std::shared_ptr<BaseServer>& server,
                            const std::shared_ptr<BaseScriptEngine>& engine,
                            DataStore* pDataStore,
                            const std::string& migrationDirectory) {
  if (!IsOpen()) {
    LogDatabaseErrorMsg("Trying to setup a database that is not open!\n");

    return false;
  }

  auto databaseName =
      std::dynamic_pointer_cast<objects::DatabaseConfigMariaDB>(mConfig)
          ->GetDatabaseName();
  if (!Exists()) {
    // Delete the old database if it exists.
    if (!Execute(String("DROP DATABASE IF EXISTS %1;").Arg(databaseName))) {
      LogDatabaseErrorMsg("Failed to delete existing database\n");

      return false;
    }

    // Now re-create the database.
    if (!Execute(String("CREATE DATABASE %1 CHARACTER SET utf8mb4 COLLATE "
                        "utf8mb4_general_ci;")
                     .Arg(databaseName))) {
      LogDatabaseErrorMsg("Failed to create database\n");

      return false;
    }

    // Use the database.
    if (!Use()) {
      LogDatabaseErrorMsg("Failed to use the newly created database\n");

      return false;
    }
  } else if (!Use()) {
    LogDatabaseErrorMsg("Failed to use the existing database\n");

    return false;
  }

  LogDatabaseDebug([&]() {
    return String("Database connection established to '%1' database.\n")
        .Arg(databaseName);
  });

  if (!VerifyAndSetupSchema(rebuild)) {
    LogDatabaseErrorMsg("Schema verification and setup failed.\n");

    return false;
  }

  if (!TableExists("Migrations")) {
    std::stringstream ss;
    ss << "CREATE TABLE IF NOT EXISTS `Migrations` "
       << "(`Migration` varchar(128) PRIMARY KEY);";

    if (Execute(ss.str())) {
      LogDatabaseInfoMsg("Migration table created.\n");
    } else {
      LogDatabaseErrorMsg("Failed to create the migration table!\n");

      return false;
    }
  }

  // Handle migrations if the data store exists.
  if (pDataStore) {
    std::list<libcomp::String> files;
    std::list<libcomp::String> dirs;
    std::list<libcomp::String> symLinks;

    if (pDataStore->GetListing(migrationDirectory, files, dirs, symLinks)) {
      files.sort();

      for (auto file : files) {
        if (file.Right(4) != ".nut") {
          continue;
        }

        libcomp::String migration = file.Left(file.Length() - 4);

        auto query = Prepare(
            "SELECT COUNT(`Migration`) FROM `Migrations` WHERE `Migration` = "
            ":file");

        if (!query.IsValid() || !query.Bind("file", migration)) {
          LogDatabaseErrorMsg("Failed to bind when checking for migration.\n");

          return false;
        }

        if (!query.Execute() || !query.Next()) {
          LogDatabaseErrorMsg(
              "Failed to execute query when checking for migration.\n");

          return false;
        }

        int64_t count = 0;

        if (!query.GetValue("COUNT(`Migration`)", count)) {
          LogDatabaseErrorMsg(
              "Failed to get value from query when checking for migration.\n");
          LogDatabaseDebugMsg(GetLastError());

          return false;
        }

        auto path = libcomp::String("%1/%2").Arg(migrationDirectory).Arg(file);

        if (!count) {
          if (ApplyMigration(server, engine, pDataStore, migration, path)) {
            query =
                Prepare("INSERT INTO `Migrations` (`Migration`) VALUES(:file)");

            if (!query.IsValid() || !query.Bind("file", migration) ||
                !query.Execute()) {
              LogDatabaseErrorMsg(
                  "Failed to insert migration into database.\n");

              return false;
            }
          } else {
            return false;
          }
        }
      }
    } else {
      LogDatabaseErrorMsg("Migration directory does not exist!\n");

      return false;
    }
  }

  return true;
}

bool DatabaseMariaDB::Use() {
  // USE not supported so close the connection and re-open
  auto config =
      std::dynamic_pointer_cast<objects::DatabaseConfigMariaDB>(mConfig);

  return ConnectToDatabase(GetConnection(false), config->GetDatabaseName());
}

std::list<std::shared_ptr<PersistentObject>> DatabaseMariaDB::LoadObjects(
    size_t typeHash, DatabaseBind* pValue) {
  std::list<std::shared_ptr<PersistentObject>> objects;

  auto metaObject = PersistentObject::GetRegisteredMetadata(typeHash);

  if (nullptr == metaObject) {
    LogDatabaseErrorMsg("Failed to lookup MetaObject.\n");

    return {};
  }

  String sql =
      String("SELECT * FROM `%1`%2")
          .Arg(metaObject->GetName())
          .Arg((nullptr != pValue
                    ? String(" WHERE `%1` = :%1").Arg(pValue->GetColumn())
                    : ""));

  DatabaseQuery query = Prepare(sql);

  if (!query.IsValid()) {
    LogDatabaseError(
        [&]() { return String("Failed to prepare SQL query: %1\n").Arg(sql); });

    LogDatabaseError(
        [&]() { return String("Database said: %1\n").Arg(GetLastError()); });

    return {};
  }

  if (nullptr != pValue && !pValue->Bind(query)) {
    LogDatabaseError([&]() {
      return String("Failed to bind value: %1\n").Arg(pValue->GetColumn());
    });

    LogDatabaseError(
        [&]() { return String("Database said: %1\n").Arg(GetLastError()); });

    return {};
  }

  if (!query.Execute()) {
    LogDatabaseError(
        [&]() { return String("Failed to execute query: %1\n").Arg(sql); });

    LogDatabaseError(
        [&]() { return String("Database said: %1\n").Arg(GetLastError()); });

    return {};
  }

  int failures = 0;

  while (query.Next()) {
    auto obj = LoadSingleObjectFromRow(typeHash, query);

    if (nullptr != obj) {
      objects.push_back(obj);
    } else {
      failures++;
    }
  }

  if (failures > 0) {
    LogDatabaseError([&]() {
      return String("%1 '%2' row%3 failed to load.\n")
          .Arg(failures)
          .Arg(metaObject->GetName())
          .Arg(failures != 1 ? "s" : "");
    });
  }

  return objects;
}

bool DatabaseMariaDB::InsertSingleObject(
    std::shared_ptr<PersistentObject>& obj) {
  auto metaObject = obj->GetObjectMetadata();

  std::stringstream objstream;
  if (!obj->Save(objstream)) {
    return false;
  }

  if (obj->GetUUID().IsNull() && !obj->Register(obj)) {
    return false;
  }

  std::list<String> columnNames;
  columnNames.push_back("`UID`");

  std::list<String> columnBinds;
  columnBinds.push_back(":UID");

  auto values = obj->GetMemberBindValues(true);

  for (auto value : values) {
    auto columnName = value->GetColumn();
    columnNames.push_back(String("`%1`").Arg(columnName));
    columnBinds.push_back(String(":%1").Arg(columnName));
  }

  String sql = String("INSERT INTO `%1` (%2) VALUES (%3);")
                   .Arg(metaObject->GetName())
                   .Arg(String::Join(columnNames, ", "))
                   .Arg(String::Join(columnBinds, ", "));

  DatabaseQuery query = Prepare(sql);

  if (!query.IsValid()) {
    LogDatabaseError(
        [&]() { return String("Failed to prepare SQL query: %1\n").Arg(sql); });

    LogDatabaseError(
        [&]() { return String("Database said: %1\n").Arg(GetLastError()); });

    return false;
  }

  if (!query.Bind("UID", obj->GetUUID())) {
    LogDatabaseErrorMsg("Failed to bind value: UID\n");

    LogDatabaseError(
        [&]() { return String("Database said: %1\n").Arg(GetLastError()); });

    return false;
  }

  for (auto value : values) {
    if (!value->Bind(query)) {
      LogDatabaseError([&]() {
        return String("Failed to bind value: %1\n").Arg(value->GetColumn());
      });

      LogDatabaseError(
          [&]() { return String("Database said: %1\n").Arg(GetLastError()); });

      return false;
    }

    delete value;
  }

  if (!query.Execute()) {
    LogDatabaseError(
        [&]() { return String("Failed to execute query: %1\n").Arg(sql); });

    LogDatabaseError(
        [&]() { return String("Database said: %1\n").Arg(GetLastError()); });

    return false;
  }

  return true;
}

bool DatabaseMariaDB::UpdateSingleObject(
    std::shared_ptr<PersistentObject>& obj) {
  auto metaObject = obj->GetObjectMetadata();

  std::stringstream objstream;
  if (!obj->Save(objstream)) {
    return false;
  }

  if (obj->GetUUID().IsNull()) {
    return false;
  }

  auto values = obj->GetMemberBindValues();
  if (values.size() == 0) {
    // Nothing updated, nothing to do
    return true;
  }

  std::list<String> columnNames;

  for (auto value : values) {
    columnNames.push_back(String("`%1` = :%1").Arg(value->GetColumn()));
  }

  String sql = String("UPDATE `%1` SET %2 WHERE `UID` = :UID;")
                   .Arg(metaObject->GetName())
                   .Arg(String::Join(columnNames, ", "));

  DatabaseQuery query = Prepare(sql);

  if (!query.IsValid()) {
    LogDatabaseError(
        [&]() { return String("Failed to prepare SQL query: %1\n").Arg(sql); });

    LogDatabaseError(
        [&]() { return String("Database said: %1\n").Arg(GetLastError()); });

    return false;
  }

  if (!query.Bind("UID", obj->GetUUID())) {
    LogDatabaseErrorMsg("Failed to bind value: UID\n");

    LogDatabaseError(
        [&]() { return String("Database said: %1\n").Arg(GetLastError()); });

    return false;
  }

  for (auto value : values) {
    if (!value->Bind(query)) {
      LogDatabaseError([&]() {
        return String("Failed to bind value: %1\n").Arg(value->GetColumn());
      });

      LogDatabaseError(
          [&]() { return String("Database said: %1\n").Arg(GetLastError()); });

      return false;
    }

    delete value;
  }

  if (!query.Execute()) {
    LogDatabaseError(
        [&]() { return String("Failed to execute query: %1\n").Arg(sql); });

    LogDatabaseError(
        [&]() { return String("Database said: %1\n").Arg(GetLastError()); });

    return false;
  }

  return true;
}

bool DatabaseMariaDB::DeleteObjects(
    std::list<std::shared_ptr<PersistentObject>>& objs) {
  std::unordered_map<std::shared_ptr<libobjgen::MetaObject>,
                     std::list<std::shared_ptr<PersistentObject>>>
      metaObjectMap;
  for (auto obj : objs) {
    auto metaObj = obj->GetObjectMetadata();
    metaObjectMap[metaObj].push_back(obj);
  }

  for (auto mPair : metaObjectMap) {
    auto metaObject = mPair.first;

    std::list<String> uidBindings;
    for (auto obj : mPair.second) {
      auto uuid = obj->GetUUID();
      if (uuid.IsNull()) {
        return false;
      }

      obj->Unregister();

      std::string uuidStr = obj->GetUUID().ToString();
      uidBindings.push_back(String("'%1'").Arg(uuidStr));
    }

    if (!Execute(String("DELETE FROM `%1` WHERE `UID` in (%2);")
                     .Arg(metaObject->GetName())
                     .Arg(String::Join(uidBindings, ", ")))) {
      return false;
    }
  }

  return true;
}

bool DatabaseMariaDB::VerifyAndSetupSchema(bool recreateTables) {
  auto metaObjectTables = GetMappedObjects();
  if (metaObjectTables.size() == 0) {
    return true;
  }

  auto databaseName =
      std::dynamic_pointer_cast<objects::DatabaseConfigMariaDB>(mConfig)
          ->GetDatabaseName();

  LogDatabaseInfoMsg("Verifying database table structure.\n");

  DatabaseQuery q = Prepare(
      libcomp::String("SELECT TABLE_NAME, COLUMN_NAME, DATA_TYPE FROM "
                      "information_schema.COLUMNS WHERE TABLE_SCHEMA = '%1';")
          .Arg(databaseName));

  if (!q.Execute()) {
    LogDatabaseCriticalMsg("Failed to query for existing columns\n");

    return false;
  }

  std::unordered_map<std::string, std::unordered_map<std::string, String>>
      fieldMap;
  while (q.Next()) {
    String name;
    String colName;
    String colType;

    if (!q.GetValue("TABLE_NAME", name) ||
        !q.GetValue("COLUMN_NAME", colName) ||
        !q.GetValue("DATA_TYPE", colType)) {
      LogDatabaseCritical([&]() {
        return String(
                   "Invalid query results returned from the COLUMNS table.\n")
            .Arg(name);
      });

      return false;
    }

    fieldMap[name.ToLower().ToUtf8()][colName.ToLower().ToUtf8()] = colType;
  }

  q = Prepare(libcomp::String(
                  "SELECT TABLE_NAME, INDEX_NAME, COLUMN_NAME FROM "
                  "INFORMATION_SCHEMA.STATISTICS WHERE TABLE_SCHEMA = '%1';")
                  .Arg(databaseName));
  if (!q.Execute()) {
    LogDatabaseCriticalMsg("Failed to query for existing indexes\n");

    return false;
  }

  std::unordered_map<std::string, std::set<std::string>> indexedFields;
  while (q.Next()) {
    String name;
    String idxName;

    if (!q.GetValue("TABLE_NAME", name) || !q.GetValue("INDEX_NAME", idxName)) {
      LogDatabaseCritical([&]() {
        return String(
                   "Invalid query results returned from the STATISTICS "
                   "table.\n")
            .Arg(name);
      });

      return false;
    }

    indexedFields[name.ToLower().ToUtf8()].insert(idxName.ToLower().ToUtf8());
  }

  for (auto metaObjectTable : metaObjectTables) {
    auto metaObject = *metaObjectTable.get();
    auto objName = metaObject.GetName();
    auto objNameLower = String(objName).ToLower().ToUtf8();

    std::vector<std::shared_ptr<libobjgen::MetaVariable>> vars;
    for (auto iter = metaObject.VariablesBegin();
         iter != metaObject.VariablesEnd(); iter++) {
      String type = GetVariableType(*iter);
      if (type.IsEmpty()) {
        LogDatabaseError([&]() {
          return String("Unsupported field type encountered: %1\n")
              .Arg((*iter)->GetCodeType());
        });

        return false;
      }
      vars.push_back(*iter);
    }

    bool creating = false;
    bool recreating = false;
    bool updating = false;
    std::set<std::string> needsIndex;
    auto tableIter = fieldMap.find(objNameLower);
    if (tableIter == fieldMap.end()) {
      creating = true;
    } else {
      recreating = recreateTables;

      std::unordered_map<std::string, String> columns = tableIter->second;

      auto indexes = indexedFields[objNameLower];
      columns.erase("uid");
      for (auto var : vars) {
        auto name = String(var->GetName()).ToLower().ToUtf8();
        auto type = GetVariableType(var);

        // Do not compare on size specifiers
        if (type.Contains("(")) {
          type = type.Split("(").front();
        }

        if (columns.find(name) == columns.end()) {
          updating = true;
        } else if (columns[name] != type) {
          recreating = true;
        }

        auto indexName =
            String("idx_%1_%2").Arg(objNameLower).Arg(name).ToUtf8();
        if (var->IsLookupKey() && indexes.find(indexName) == indexes.end()) {
          needsIndex.insert(var->GetName());
        }
      }
    }

    if (recreating) {
      if (mConfig->GetAutoSchemaUpdate()) {
        LogDatabaseInfo([&]() {
          return String("Archiving table '%1'...\n").Arg(metaObject.GetName());
        });

        LogDatabaseInfo([&]() {
          return String("Dropping table '%1'...\n").Arg(metaObject.GetName());
        });

        if (Execute(String("DROP TABLE `%1`;").Arg(objName))) {
          LogDatabaseInfoMsg("Re-creation complete\n");
        } else {
          LogDatabaseErrorMsg("Re-creation failed\n");
          return false;
        }

        creating = true;
      } else {
        LogDatabaseError([&]() {
          return String(
                     "The schema for '%1' does not match and cannot be used "
                     "until it has been corrected!\n")
              .Arg(metaObject.GetName());
        });

        return false;
      }
    }

    if (creating) {
      LogDatabaseInfo([&]() {
        return String("Creating table '%1'...\n").Arg(metaObject.GetName());
      });

      bool success = false;

      std::stringstream ss;
      ss << "CREATE TABLE IF NOT EXISTS `" << objName
         << "` (`UID` varchar(36) PRIMARY KEY";
      for (size_t i = 0; i < vars.size(); i++) {
        auto var = vars[i];
        String type = GetVariableType(var);

        ss << "," << std::endl << "`" << var->GetName() << "` " << type;
      }
      ss << ");";

      success = Execute(ss.str());

      if (success) {
        LogDatabaseInfoMsg("Creation complete\n");
      } else {
        LogDatabaseErrorMsg("Creation failed\n");

        return false;
      }
    } else if (updating) {
      LogDatabaseInfo([&]() {
        return String("Updating table '%1'...\n").Arg(metaObject.GetName());
      });

      auto objColumns = fieldMap[objNameLower];

      bool hashExists;
      size_t typeHash =
          PersistentObject::GetTypeHashByName(metaObject.GetName(), hashExists);

      auto emptyObj = PersistentObject::New(typeHash);

      std::unordered_map<std::string, DatabaseBind*> defaultVals;
      for (auto val : emptyObj->GetMemberBindValues(true)) {
        defaultVals[val->GetColumn().C()] = val;
      }

      for (size_t i = 0; i < vars.size(); i++) {
        auto var = vars[i];
        auto col = defaultVals[var->GetName()];
        String type = GetVariableType(var);
        if (col && objColumns.find(String(var->GetName()).ToLower().C()) ==
                       objColumns.end()) {
          if (Execute(String("ALTER TABLE `%1` ADD `%2` %3;")
                          .Arg(objName)
                          .Arg(var->GetName())
                          .Arg(type))) {
            LogDatabaseInfo([&]() {
              return String("Created column '%1'\n").Arg(var->GetName());
            });
          } else {
            LogDatabaseError([&]() {
              return String("Failed to create column '%1'\n")
                  .Arg(var->GetName());
            });

            return false;
          }

          q = Prepare(String("UPDATE `%1` SET `%2` = :%3;")
                          .Arg(metaObject.GetName())
                          .Arg(col->GetColumn())
                          .Arg(col->GetColumn()));
          if (!col->Bind(q)) {
            LogDatabaseWarning([&]() {
              return String("Failed to bind default value for column '%1'\n")
                  .Arg(col->GetColumn());
            });
          }

          if (q.Execute()) {
            LogDatabaseInfo([&]() {
              return String("Applied default value to column '%1'\n")
                  .Arg(col->GetColumn());
            });
          } else {
            LogDatabaseError([&]() {
              return String("Failed to apply default value to column '%1'\n")
                  .Arg(col->GetColumn());
            });

            return false;
          }
        }
      }
    }

    // If we made the table or are missing an index, make them now
    if (needsIndex.size() > 0 || creating) {
      for (size_t i = 0; i < vars.size(); i++) {
        auto var = vars[i];

        auto name = var->GetName();

        if (!var->IsLookupKey() ||
            (!creating && needsIndex.find(name) == needsIndex.end())) {
          continue;
        }

        auto indexStr = String("idx_%1_%2").Arg(objName).Arg(name);

        // MariaDB indexes values based off a set size so values like blobs and
        // strings without a limited size need to be indexed by a specified
        // amount
        bool limitIndex =
            GetVariableType(var) == "blob" ||
            var->GetMetaType() ==
                libobjgen::MetaVariable::MetaVariableType_t::TYPE_STRING;
        auto fieldStr =
            String("`%1`%2").Arg(var->GetName()).Arg(limitIndex ? "(10)" : "");

        auto cmd = String("CREATE INDEX %1 ON `%2`(%3);")
                       .Arg(indexStr)
                       .Arg(objName)
                       .Arg(fieldStr);

        if (Execute(cmd)) {
          LogDatabaseInfo([&]() {
            return String("Created '%1' column index.\n").Arg(indexStr);
          });
        } else {
          LogDatabaseError([&]() {
            return String("Creation of '%1' column index failed.\n")
                .Arg(indexStr);
          });

          return false;
        }
      }
    }

    if (!creating && !recreating && !updating && needsIndex.size() == 0) {
      LogDatabaseInfo([&]() {
        return String("'%1': Verified\n").Arg(metaObject.GetName());
      });
    }
  }

  LogDatabaseInfoMsg("Database verification complete.\n");

  return true;
}

bool DatabaseMariaDB::ProcessStandardChangeSet(
    const std::shared_ptr<DBStandardChangeSet>& changes) {
  auto connection = GetConnection(true);
  if (connection == nullptr) {
    return false;
  }

  if (mysql_autocommit(connection, false)) {
    LogDatabaseDebug([&]() {
      return String("mysql_autocommit failed for connection: %1\n")
          .Arg(ConnectionString(connection));
    });

    LogDatabaseDebug([&]() {
      return String("Last SQL error: %1\n").Arg(GetLastError(connection));
    });

    return false;
  }

  bool result = true;
  for (auto obj : changes->GetInserts()) {
    if (!InsertSingleObject(obj)) {
      result = false;
      break;
    }
  }

  if (result) {
    for (auto obj : changes->GetUpdates()) {
      if (!UpdateSingleObject(obj)) {
        result = false;
        break;
      }
    }
  }

  auto deletes = changes->GetDeletes();
  if (result && deletes.size()) {
    result = DeleteObjects(deletes);
  }

  if (result) {
    result = !mysql_commit(connection);

    if (!result) {
      LogDatabaseDebug([&]() {
        return String("mysql_commit failed for connection: %1\n")
            .Arg(ConnectionString(connection));
      });

      LogDatabaseDebug([&]() {
        return String("Last SQL error: %1\n").Arg(GetLastError(connection));
      });
    }
  } else if (mysql_rollback(connection)) {
    LogDatabaseDebug([&]() {
      return String("mysql_rollback failed for connection: %1\n")
          .Arg(ConnectionString(connection));
    });

    LogDatabaseDebug([&]() {
      return String("Last SQL error: %1\n").Arg(GetLastError(connection));
    });

    // If this happens the server may need to be shut down
    LogDatabaseCriticalMsg("Rollback failed!\n");
  }

  if (mysql_autocommit(connection, true)) {
    LogDatabaseDebug([&]() {
      return String("mysql_autocommit failed for connection: %1\n")
          .Arg(ConnectionString(connection));
    });

    LogDatabaseDebug([&]() {
      return String("Last SQL error: %1\n").Arg(GetLastError(connection));
    });

    return false;
  }

  return result;
}

bool DatabaseMariaDB::ProcessOperationalChangeSet(
    const std::shared_ptr<DBOperationalChangeSet>& changes) {
  auto connection = GetConnection(true);
  if (connection == nullptr) {
    return false;
  }

  if (mysql_autocommit(connection, false)) {
    LogDatabaseDebug([&]() {
      return String("mysql_autocommit failed for connection: %1\n")
          .Arg(ConnectionString(connection));
    });

    LogDatabaseDebug([&]() {
      return String("Last SQL error: %1\n").Arg(GetLastError(connection));
    });

    return false;
  }

  bool result = true;
  std::set<std::shared_ptr<libcomp::PersistentObject>> objs;
  for (auto op : changes->GetOperations()) {
    auto obj = op->GetRecord();
    switch (op->GetType()) {
      case DBOperationalChange::DBOperationType::DBOP_INSERT:
        result &= InsertSingleObject(obj);
        break;
      case DBOperationalChange::DBOperationType::DBOP_UPDATE:
        result &= UpdateSingleObject(obj);
        break;
      case DBOperationalChange::DBOperationType::DBOP_DELETE:
        result &= DeleteSingleObject(obj);
        break;
      case DBOperationalChange::DBOperationType::DBOP_EXPLICIT:
        objs.insert(obj);
        result &= ProcessExplicitUpdate(
            std::dynamic_pointer_cast<DBExplicitUpdate>(op));
        break;
    }

    if (!result) {
      break;
    }
  }

  if (result) {
    result = !mysql_commit(connection);

    if (!result) {
      LogDatabaseDebug([&]() {
        return String("mysql_autocommit failed for connection: %1\n")
            .Arg(ConnectionString(connection));
      });

      LogDatabaseDebug([&]() {
        return String("Last SQL error: %1\n").Arg(GetLastError(connection));
      });
    }
  } else if (mysql_rollback(connection)) {
    LogDatabaseDebug([&]() {
      return String("mysql_rollback failed for connection: %1\n")
          .Arg(ConnectionString(connection));
    });

    LogDatabaseDebug([&]() {
      return String("Last SQL error: %1\n").Arg(GetLastError(connection));
    });

    // If this happens the server may need to be shut down
    LogDatabaseCriticalMsg("Rollback failed!\n");
  }

  if (mysql_autocommit(connection, true)) {
    LogDatabaseDebug([&]() {
      return String("mysql_autocommit failed for connection: %1\n")
          .Arg(ConnectionString(connection));
    });

    LogDatabaseDebug([&]() {
      return String("Last SQL error: %1\n").Arg(GetLastError(connection));
    });

    return false;
  }

  if (result) {
    for (auto obj : objs) {
      auto bind = new DatabaseBindUUID("UID", obj->GetUUID());
      result = (nullptr != LoadSingleObject(
                               libcomp::PersistentObject::GetTypeHashByName(
                                   obj->GetObjectMetadata()->GetName(), result),
                               bind));

      if (!result) {
        break;
      }
    }
  }

  return result;
}

bool DatabaseMariaDB::ProcessExplicitUpdate(
    const std::shared_ptr<DBExplicitUpdate>& update) {
  auto obj = update->GetRecord();
  auto expectedVals = update->GetExpectedValues();
  auto changedVals = update->GetChanges();
  if (changedVals.size() == 0) {
    return false;
  }

  size_t idx = 0;
  std::list<String> updateClause;
  std::list<String> whereClause;
  // Bind the update clause values
  for (auto cPair : changedVals) {
    auto it = expectedVals.find(cPair.first);
    if (it == expectedVals.end()) {
      return false;
    }

    updateClause.push_back(String("`%1` = :%2").Arg(cPair.first).Arg(idx++));
  }

  auto uidIdx = idx++;

  // Now bind the where clause values
  for (auto cPair : changedVals) {
    whereClause.push_back(String("`%1` = :%2").Arg(cPair.first).Arg(idx++));
  }

  String sql = String("UPDATE `%1` SET %2 WHERE `UID` = :%3 AND %4;")
                   .Arg(obj->GetObjectMetadata()->GetName())
                   .Arg(String::Join(updateClause, ", "))
                   .Arg(uidIdx)
                   .Arg(String::Join(whereClause, " AND "));

  DatabaseQuery query = Prepare(sql);

  if (!query.IsValid()) {
    LogDatabaseError(
        [&]() { return String("Failed to prepare SQL query: %1\n").Arg(sql); });

    LogDatabaseError(
        [&]() { return String("Database said: %1\n").Arg(GetLastError()); });

    return false;
  }

  idx = 0;
  for (auto cPair : changedVals) {
    if (!cPair.second->Bind(query, idx++)) {
      LogDatabaseError([&]() {
        return String("Failed to bind value: %1\n").Arg(cPair.first);
      });

      LogDatabaseError(
          [&]() { return String("Database said: %1\n").Arg(GetLastError()); });

      return false;
    }
  }

  if (!query.Bind(idx++, obj->GetUUID())) {
    LogDatabaseErrorMsg("Failed to bind value: UID\n");

    LogDatabaseError(
        [&]() { return String("Database said: %1\n").Arg(GetLastError()); });

    return false;
  }

  for (auto cPair : changedVals) {
    if (!expectedVals[cPair.first]->Bind(query, idx++)) {
      LogDatabaseError([&]() {
        return String("Failed to bind where clause for value: %1\n")
            .Arg(cPair.first);
      });

      LogDatabaseError(
          [&]() { return String("Database said: %1\n").Arg(GetLastError()); });

      return false;
    }
  }

  if (!query.Execute()) {
    LogDatabaseError(
        [&]() { return String("Failed to execute query: %1\n").Arg(sql); });

    LogDatabaseError(
        [&]() { return String("Database said: %1\n").Arg(GetLastError()); });

    return false;
  }

  return query.AffectedRowCount() == 1;
}

bool DatabaseMariaDB::ConnectToDatabase(MYSQL*& connection,
                                        const libcomp::String& databaseName) {
  Close(connection);

  connection = mysql_init(NULL);
  if (connection == NULL) {
    LogDatabaseErrorMsg("mysql_init failed\n");

    return false;
  }

  auto config =
      std::dynamic_pointer_cast<objects::DatabaseConfigMariaDB>(mConfig);
  auto hostIP = config->GetIP();
  auto username = config->GetUsername();
  auto password = config->GetPassword();

  connection = mysql_real_connect(
      connection, (!hostIP.IsEmpty() ? hostIP.C() : "localhost"),
      (!username.IsEmpty() ? username.C() : NULL),
      (!password.IsEmpty() ? password.C() : NULL),
      (!databaseName.IsEmpty() ? databaseName.C() : NULL), config->GetPort(),
      NULL, 0);
  if (connection == NULL) {
    LogDatabaseErrorMsg("Failed to open database connection\n");

    Close(connection);

    return false;
  }

  // Set the encoding of the connection.
  if (mysql_set_character_set(connection, "utf8mb4")) {
    LogDatabaseErrorMsg(
        "Failed to set character set for database connection\n");

    Close(connection);

    return false;
  }

  // Set auto reconnect in case a connection idles too long
  bool reconnect = 1;
  if (mysql_options(connection, MYSQL_OPT_RECONNECT, &reconnect)) {
    LogDatabaseErrorMsg("Failed to set MYSQL_OPT_RECONNECT on the database.\n");

    Close(connection);

    return false;
  }

  LogDatabaseDebug([&]() {
    return String("New database connection opened: %1\n")
        .Arg(ConnectionString(connection));
  });

  return true;
}

MYSQL*& DatabaseMariaDB::GetConnection(bool autoConnect) {
  auto threadID = std::this_thread::get_id();
  std::lock_guard<std::mutex> lock(mConnectionLock);
  if (mConnections.find(threadID) == mConnections.end()) {
    MYSQL* connection = nullptr;

    if (autoConnect) {
      auto config =
          std::dynamic_pointer_cast<objects::DatabaseConfigMariaDB>(mConfig);
      ConnectToDatabase(connection, config->GetDatabaseName());
    }

    mConnections[threadID] = connection;
  }

  return mConnections[threadID];
}

String DatabaseMariaDB::GetVariableType(
    const std::shared_ptr<libobjgen::MetaVariable> var) {
  switch (var->GetMetaType()) {
    case libobjgen::MetaVariable::MetaVariableType_t::TYPE_STRING:
      return "text";
    case libobjgen::MetaVariable::MetaVariableType_t::TYPE_REF:
      return "varchar(36)";
    case libobjgen::MetaVariable::MetaVariableType_t::TYPE_BOOL:
      return "bit";
    case libobjgen::MetaVariable::MetaVariableType_t::TYPE_S8:
    case libobjgen::MetaVariable::MetaVariableType_t::TYPE_S16:
    case libobjgen::MetaVariable::MetaVariableType_t::TYPE_S32:
    case libobjgen::MetaVariable::MetaVariableType_t::TYPE_U8:
    case libobjgen::MetaVariable::MetaVariableType_t::TYPE_U16:
    case libobjgen::MetaVariable::MetaVariableType_t::TYPE_ENUM:
      return "int";
    case libobjgen::MetaVariable::MetaVariableType_t::TYPE_U32:
    case libobjgen::MetaVariable::MetaVariableType_t::TYPE_S64:
      return "bigint";
    case libobjgen::MetaVariable::MetaVariableType_t::TYPE_FLOAT:
      return "float";
    case libobjgen::MetaVariable::MetaVariableType_t::TYPE_DOUBLE:
      return "double";
    case libobjgen::MetaVariable::MetaVariableType_t::TYPE_U64:
    case libobjgen::MetaVariable::MetaVariableType_t::TYPE_ARRAY:
    case libobjgen::MetaVariable::MetaVariableType_t::TYPE_LIST:
    case libobjgen::MetaVariable::MetaVariableType_t::TYPE_SET:
    case libobjgen::MetaVariable::MetaVariableType_t::TYPE_MAP:
    default:
      break;
  }

  return "blob";
}

String DatabaseMariaDB::GetLastError() {
  auto connection = GetConnection(false);

  if (connection) {
    const char* szError = mysql_error(connection);

    if (nullptr != szError && 0 != szError[0]) {
      return szError;
    }
  }

  return "Invalid connection.";
}

String DatabaseMariaDB::GetLastError(MYSQL* connection) {
  if (connection) {
    const char* szError = mysql_error(connection);

    if (nullptr != szError && 0 != szError[0]) {
      return szError;
    }
  }

  return "Invalid connection.";
}

bool DatabaseMariaDB::TableExists(const libcomp::String& table) {
  int64_t tableExists = 0;

  auto config =
      std::dynamic_pointer_cast<objects::DatabaseConfigMariaDB>(mConfig);
  auto query =
      Prepare(libcomp::String(
                  "SELECT COUNT(TABLE_NAME) FROM INFORMATION_SCHEMA.STATISTICS "
                  "WHERE TABLE_NAME = '%1' AND TABLE_SCHEMA = '%2';")
                  .Arg(table)
                  .Arg(config->GetDatabaseName()));

  if (!query.IsValid() || !query.Execute() || !query.Next() ||
      !query.GetValue(0, tableExists)) {
    return false;
  }

  return 0 != tableExists;
}

#endif  // !EXOTIC_PLATFORM
