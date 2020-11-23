/**
 * @file libcomp/src/DatabaseQueryMariaDB.cpp
 * @ingroup libcomp
 *
 * @author HACKfrost
 *
 * @brief A MariaDB database query.
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

#include "DatabaseQueryMariaDB.h"

#include "DatabaseMariaDB.h"
#include "Log.h"

#ifndef EXOTIC_PLATFORM

// config-win.h and my_global.h redefine bool unless explicitly defined
#define bool bool

// MariaDB Includes
#include <mysql.h>

using namespace libcomp;

static libcomp::String ConnectionString(void* pConnection) {
  return libcomp::String("{%1-%2}")
      .Arg((uint32_t)(((uint64_t)pConnection) >> 32), 8, 16, '0')
      .Arg((uint32_t)((uint64_t)pConnection), 8, 16, '0');
}

static libcomp::String GetLastError(MYSQL* connection) {
  if (connection) {
    const char* szError = mysql_error(connection);

    if (nullptr != szError && 0 != szError[0]) {
      return szError;
    }
  }

  return "Invalid connection.";
}

DatabaseQueryMariaDB::DatabaseQueryMariaDB(MYSQL* pDatabase)
    : mDatabase(pDatabase), mStatement(nullptr), mStatus(0) {}

DatabaseQueryMariaDB::~DatabaseQueryMariaDB() {
  if (nullptr != mStatement) {
    mysql_stmt_close(mStatement);

    LogDatabaseDebug([&]() {
      return String("Database statement closed: %1\n")
          .Arg(ConnectionString(mStatement));
    });
  }
}

bool DatabaseQueryMariaDB::Prepare(const String& query) {
  // MySQL/MariaDB does not support named parameter binding so
  // the query to prepare will need to have named parameters that
  // we will replace here in case the query needs access to the
  // named parameter binding functionality
  std::string transformed(query.C());
  static const std::regex namedParam(":(?:[a-zA-Z0-9_]+)");

  mParamNames.clear();
  std::smatch match;
  while (std::regex_search(transformed, match, namedParam)) {
    mParamNames.push_back(match.str().substr(1));
    transformed = std::regex_replace(transformed, namedParam, "?",
                                     std::regex_constants::format_first_only);
  }

  mStatement = mysql_stmt_init(mDatabase);

  if (nullptr == mStatement) {
    LogDatabaseDebug([&]() {
      return String("Failed to create statement for connection: %1\n")
          .Arg(ConnectionString(mDatabase));
    });
  }

  if (nullptr != mStatement) {
    LogDatabaseDebug([&]() {
      return String("Created statement %1 for connection %2\n")
          .Arg(ConnectionString(mStatement))
          .Arg(ConnectionString(mDatabase));
    });
  }

  unsigned long len = (unsigned long)transformed.length();
  mStatus = mysql_stmt_prepare(mStatement, transformed.c_str(), len);

  if (mStatus) {
    LogDatabaseDebug([&]() {
      return String("Prepare '%1' FAILED for statement %2 for connection %3\n")
          .Arg(transformed.c_str())
          .Arg(ConnectionString(mStatement))
          .Arg(ConnectionString(mDatabase));
    });

    LogDatabaseDebug([&]() {
      return String("Last SQL error: %1\n").Arg(GetLastError(mDatabase));
    });
  } else {
    LogDatabaseDebug([&]() {
      return String("Prepare '%1' for statement %2 for connection %3\n")
          .Arg(transformed.c_str())
          .Arg(ConnectionString(mStatement))
          .Arg(ConnectionString(mDatabase));
    });
  }

  return IsValid();
}

bool DatabaseQueryMariaDB::Execute() {
  if (!IsValid()) {
    return false;
  }

  if (mBindings.size() > 0 &&
      mysql_stmt_bind_param(mStatement, &mBindings[0])) {
    LogDatabaseDebug([&]() {
      return String(
                 "Execute of statement %1 failed for connection %2 due to a "
                 "bad bind\n")
          .Arg(ConnectionString(mStatement))
          .Arg(ConnectionString(mDatabase));
    });

    LogDatabaseDebug([&]() {
      return String("Last SQL error: %1\n").Arg(GetLastError(mDatabase));
    });

    mStatus = -1;
    return false;
  }

  mStatus = mysql_stmt_execute(mStatement);
  mAffectedRowCount = (int64_t)mysql_affected_rows(mDatabase);

  if (!mStatus) {
    LogDatabaseDebug([&]() {
      return String(
                 "Execute of statement %1 for connection %2 is OK with %3 rows "
                 "affected\n")
          .Arg(ConnectionString(mStatement))
          .Arg(ConnectionString(mDatabase))
          .Arg(mAffectedRowCount);
    });
  } else {
    LogDatabaseDebug([&]() {
      return String("Execute of statement %1 failed for connection %2\n")
          .Arg(ConnectionString(mStatement))
          .Arg(ConnectionString(mDatabase));
    });

    LogDatabaseDebug([&]() {
      return String("Last SQL error: %1\n").Arg(GetLastError(mDatabase));
    });
  }

  my_bool aBool = 1;

  if (mysql_stmt_attr_set(mStatement, STMT_ATTR_UPDATE_MAX_LENGTH, &aBool)) {
    LogDatabaseDebug([&]() {
      return String(
                 "mysql_stmt_attr_set of statement %1 failed for connection "
                 "%2\n")
          .Arg(ConnectionString(mStatement))
          .Arg(ConnectionString(mDatabase));
    });

    LogDatabaseDebug([&]() {
      return String("Last SQL error: %1\n").Arg(GetLastError(mDatabase));
    });
  }

  if (mysql_stmt_store_result(mStatement)) {
    LogDatabaseDebug([&]() {
      return String(
                 "mysql_stmt_store_result of statement %1 failed for "
                 "connection %2\n")
          .Arg(ConnectionString(mStatement))
          .Arg(ConnectionString(mDatabase));
    });

    LogDatabaseDebug([&]() {
      return String("Last SQL error: %1\n").Arg(GetLastError(mDatabase));
    });
  }

  MYSQL_RES* result = mysql_stmt_result_metadata(mStatement);

  if (result != nullptr) {
    mResultBindings.clear();

    MYSQL_FIELD* field = mysql_fetch_field(result);
    while (field) {
      mResultColumnNames.push_back(std::string(field->name));
      mResultColumnTypes.push_back((int)field->type);

      mBufferNulls.push_back(false);
      mBufferLengths.push_back(0);

      MYSQL_BIND b;
      memset(&b, 0, sizeof(MYSQL_BIND));

      b.buffer_type = field->type;
      b.is_null = &mBufferNulls.back();
      b.length = &mBufferLengths.back();
      switch (field->type) {
        case MYSQL_TYPE_LONG: {
          mBufferInt.push_back(0);
          b.buffer = &mBufferInt.back();
        } break;
        case MYSQL_TYPE_LONGLONG: {
          mBufferBigInt.push_back(0);
          b.buffer = &mBufferBigInt.back();
        } break;
        case MYSQL_TYPE_FLOAT: {
          mBufferFloat.push_back(0.f);
          b.buffer = &mBufferFloat.back();
        } break;
        case MYSQL_TYPE_DOUBLE: {
          mBufferDouble.push_back(0.0);
          b.buffer = &mBufferDouble.back();
        } break;
        case MYSQL_TYPE_BLOB:
        case MYSQL_TYPE_VAR_STRING:
        case MYSQL_TYPE_STRING: {
          std::vector<char> buff;
          // Insert at least one character to represent the null terminator
          for (unsigned long i = 0; i < field->max_length || i < 1; i++) {
            buff.push_back(0);
          }
          mBufferBlob.push_back(buff);
          b.buffer = &mBufferBlob.back()[0];
          b.buffer_length = (unsigned long)buff.size();
        } break;
        case MYSQL_TYPE_BIT: {
          mBufferBool.push_back(false);
          b.buffer = &mBufferBool.back();
          b.buffer_length = field->length;
        } break;
        default:
          mStatus = -1;
          break;
      }
      mResultBindings.push_back(b);

      field = mysql_fetch_field(result);
    }

    if (mysql_stmt_bind_result(mStatement, &mResultBindings[0])) {
      LogDatabaseDebug([&]() {
        return String(
                   "mysql_stmt_bind_result of statement %1 failed for "
                   "connection %2\n")
            .Arg(ConnectionString(mStatement))
            .Arg(ConnectionString(mDatabase));
      });

      LogDatabaseDebug([&]() {
        return String("Last SQL error: %1\n").Arg(GetLastError(mDatabase));
      });
    }
  }

  mysql_free_result(result);

  return IsValid();
}

bool DatabaseQueryMariaDB::Next() {
  mStatus = mysql_stmt_fetch(mStatement);

  if (mStatus && MYSQL_NO_DATA != mStatus) {
    LogDatabaseDebug([&]() {
      return String(
                 "mysql_stmt_fetch of statement %1 failed for connection %2\n")
          .Arg(ConnectionString(mStatement))
          .Arg(ConnectionString(mDatabase));
    });

    LogDatabaseDebug([&]() {
      return String("Last SQL error: %1\n").Arg(GetLastError(mDatabase));
    });
  }

  return MYSQL_NO_DATA != mStatus && IsValid();
}

bool DatabaseQueryMariaDB::Bind(size_t index, const String& value) {
  auto bind = PrepareBinding(index, MYSQL_TYPE_STRING);
  if (bind == nullptr) {
    return false;
  }

  auto data = value.Data(false);
  mBufferBlob.push_back(data);

  // Ensure that there is at least a 0 representing empty
  if (data.size() == 0) {
    mBufferBlob.back().push_back(0);
  }

  bind->buffer = &mBufferBlob.back()[0];
  bind->buffer_length = (*bind->length) = (unsigned long)data.size();

  LogDatabaseDebug([&]() {
    return String("Bound to index %1: %2\n").Arg(index).Arg(value);
  });

  return IsValid();
}

bool DatabaseQueryMariaDB::Bind(const String& name, const String& value) {
  size_t index = GetNamedBindingIndex(name);
  return IsValid() && Bind(index, value);
}

bool DatabaseQueryMariaDB::Bind(size_t index, const std::vector<char>& value) {
  auto bind = PrepareBinding(index, MYSQL_TYPE_LONG_BLOB);
  if (bind == nullptr) {
    return false;
  }

  mBufferBlob.push_back(value);

  // Ensure that there is at least a 0 representing empty
  if (value.size() == 0) {
    mBufferBlob.back().push_back(0);
  }

  bind->buffer = &mBufferBlob.back()[0];
  bind->buffer_length = (*bind->length) = (unsigned long)value.size();

  LogDatabaseDebug([&]() {
    String s;

    for (auto c : value) {
      s += String("%1").Arg((int)c & 0xFF, 2, 16, '0');
    }

    return String("Bound to index %1: %2\n").Arg(index).Arg(s);
  });

  return IsValid();
}

bool DatabaseQueryMariaDB::Bind(const String& name,
                                const std::vector<char>& value) {
  size_t index = GetNamedBindingIndex(name);
  return IsValid() && Bind(index, value);
}

bool DatabaseQueryMariaDB::Bind(size_t index, const libobjgen::UUID& value) {
  auto bind = PrepareBinding(index, MYSQL_TYPE_VAR_STRING);
  if (bind == nullptr) {
    return false;
  }

  auto uuidStr = libcomp::String(value.ToString());

  mBufferBlob.push_back(uuidStr.Data());
  bind->buffer = &mBufferBlob.back()[0];
  bind->buffer_length = (*bind->length) = 36;

  LogDatabaseDebug([&]() {
    return String("Bound to index %1: %2\n").Arg(index).Arg(value.ToString());
  });

  return IsValid();
}

bool DatabaseQueryMariaDB::Bind(const String& name,
                                const libobjgen::UUID& value) {
  size_t index = GetNamedBindingIndex(name);
  return Bind(index, value);
}

bool DatabaseQueryMariaDB::Bind(size_t index, int32_t value) {
  auto bind = PrepareBinding(index, MYSQL_TYPE_LONG);
  if (bind == nullptr) {
    return false;
  }

  mBufferInt.push_back(value);
  bind->buffer = &mBufferInt.back();

  LogDatabaseDebug([&]() {
    return String("Bound to index %1: %2\n").Arg(index).Arg(value);
  });

  return IsValid();
}

bool DatabaseQueryMariaDB::Bind(const String& name, int32_t value) {
  size_t index = GetNamedBindingIndex(name);
  return IsValid() && Bind(index, value);
}

bool DatabaseQueryMariaDB::Bind(size_t index, int64_t value) {
  auto bind = PrepareBinding(index, MYSQL_TYPE_LONGLONG);
  if (bind == nullptr) {
    return false;
  }

  mBufferBigInt.push_back(value);
  bind->buffer = &mBufferBigInt.back();

  LogDatabaseDebug([&]() {
    return String("Bound to index %1: %2\n").Arg(index).Arg(value);
  });

  return IsValid();
}

bool DatabaseQueryMariaDB::Bind(const String& name, int64_t value) {
  size_t index = GetNamedBindingIndex(name);
  return IsValid() && Bind(index, value);
}

bool DatabaseQueryMariaDB::Bind(size_t index, float value) {
  auto bind = PrepareBinding(index, MYSQL_TYPE_FLOAT);
  if (bind == nullptr) {
    return false;
  }

  mBufferFloat.push_back(value);
  bind->buffer = &mBufferFloat.back();

  LogDatabaseDebug([&]() {
    return String("Bound to index %1: %2\n").Arg(index).Arg(value);
  });

  return IsValid();
}

bool DatabaseQueryMariaDB::Bind(const String& name, float value) {
  size_t index = GetNamedBindingIndex(name);
  return IsValid() && Bind(index, value);
}

bool DatabaseQueryMariaDB::Bind(size_t index, double value) {
  auto bind = PrepareBinding(index, MYSQL_TYPE_DOUBLE);
  if (bind == nullptr) {
    return false;
  }

  mBufferDouble.push_back(value);
  bind->buffer = &mBufferDouble.back();

  LogDatabaseDebug([&]() {
    return String("Bound to index %1: %2\n").Arg(index).Arg(value);
  });

  return IsValid();
}

bool DatabaseQueryMariaDB::Bind(const String& name, double value) {
  size_t index = GetNamedBindingIndex(name);
  return IsValid() && Bind(index, value);
}

bool DatabaseQueryMariaDB::Bind(size_t index, bool value) {
  auto bind = PrepareBinding(index, MYSQL_TYPE_TINY);
  if (bind == nullptr) {
    return false;
  }

  mBufferBool.push_back(value);
  bind->buffer = &mBufferBool.back();

  LogDatabaseDebug([&]() {
    return String("Bound to index %1: %2\n")
        .Arg(index)
        .Arg(value ? "true" : "false");
  });

  return IsValid();
}

bool DatabaseQueryMariaDB::Bind(const String& name, bool value) {
  size_t index = GetNamedBindingIndex(name);
  return IsValid() && Bind(index, value);
}

bool DatabaseQueryMariaDB::Bind(
    size_t index,
    const std::unordered_map<std::string, std::vector<char>>& values) {
  (void)index;
  (void)values;

  /// @todo
  return false;
}

bool DatabaseQueryMariaDB::Bind(
    const String& name,
    const std::unordered_map<std::string, std::vector<char>>& values) {
  (void)name;
  (void)values;

  /// @todo
  return false;
}

bool DatabaseQueryMariaDB::GetValue(size_t index, String& value) {
  if (mResultColumnTypes.size() <= index ||
      (mResultColumnTypes[index] != MYSQL_TYPE_STRING &&
       mResultColumnTypes[index] != MYSQL_TYPE_VAR_STRING &&
       mResultColumnTypes[index] != MYSQL_TYPE_BLOB)) {
    return false;
  }

  auto column = mResultBindings[index];

  size_t bytes = *column.length;

  auto val = (char*)column.buffer;

  std::string str(val, val + bytes);
  value = libcomp::String(str);

  return true;
}

bool DatabaseQueryMariaDB::GetValue(const String& name, String& value) {
  size_t index;
  if (!GetResultColumnIndex(name, index)) {
    return false;
  }

  return GetValue(index, value);
}

bool DatabaseQueryMariaDB::GetValue(size_t index, std::vector<char>& value) {
  if (mResultColumnTypes.size() <= index ||
      mResultColumnTypes[index] != MYSQL_TYPE_BLOB) {
    return false;
  }

  auto column = mResultBindings[index];

  size_t bytes = *column.length;

  auto val = (char*)column.buffer;

  value.clear();
  value.insert(value.begin(), val, val + bytes);

  return true;
}

bool DatabaseQueryMariaDB::GetValue(const String& name,
                                    std::vector<char>& value) {
  size_t index;
  if (!GetResultColumnIndex(name, index)) {
    return false;
  }

  return GetValue(index, value);
}

bool DatabaseQueryMariaDB::GetValue(size_t index, libobjgen::UUID& value) {
  libcomp::String uuidStr;
  if (GetValue(index, uuidStr)) {
    value = libobjgen::UUID(uuidStr.ToUtf8());
    return true;
  }
  return false;
}

bool DatabaseQueryMariaDB::GetValue(const String& name,
                                    libobjgen::UUID& value) {
  size_t index;
  if (!GetResultColumnIndex(name, index)) {
    return false;
  }

  return GetValue(index, value);
}

bool DatabaseQueryMariaDB::GetValue(size_t index, int32_t& value) {
  if (mResultColumnTypes.size() <= index ||
      mResultColumnTypes[index] != MYSQL_TYPE_LONG) {
    return false;
  }

  auto column = mResultBindings[index];

  value = *((int32_t*)column.buffer);
  return true;
}

bool DatabaseQueryMariaDB::GetValue(const String& name, int32_t& value) {
  size_t index;
  if (!GetResultColumnIndex(name, index)) {
    return false;
  }

  return GetValue(index, value);
}

bool DatabaseQueryMariaDB::GetValue(size_t index, int64_t& value) {
  if (mResultColumnTypes.size() <= index ||
      mResultColumnTypes[index] != MYSQL_TYPE_LONGLONG) {
    return false;
  }

  auto column = mResultBindings[index];

  value = *((int64_t*)column.buffer);
  return true;
}

bool DatabaseQueryMariaDB::GetValue(const String& name, int64_t& value) {
  size_t index;
  if (!GetResultColumnIndex(name, index)) {
    return false;
  }

  return GetValue(index, value);
}

bool DatabaseQueryMariaDB::GetValue(size_t index, float& value) {
  if (mResultColumnTypes.size() <= index ||
      mResultColumnTypes[index] != MYSQL_TYPE_FLOAT) {
    return false;
  }

  auto column = mResultBindings[index];

  value = *((float*)column.buffer);
  return true;
}

bool DatabaseQueryMariaDB::GetValue(const String& name, float& value) {
  size_t index = GetNamedBindingIndex(name);
  if (!GetResultColumnIndex(name, index)) {
    return false;
  }

  return GetValue(index, value);
}

bool DatabaseQueryMariaDB::GetValue(size_t index, double& value) {
  if (mResultColumnTypes.size() <= index ||
      mResultColumnTypes[index] != MYSQL_TYPE_DOUBLE) {
    return false;
  }

  auto column = mResultBindings[index];

  value = *((double*)column.buffer);
  return true;
}

bool DatabaseQueryMariaDB::GetValue(const String& name, double& value) {
  size_t index = GetNamedBindingIndex(name);
  if (!GetResultColumnIndex(name, index)) {
    return false;
  }

  return GetValue(index, value);
}

bool DatabaseQueryMariaDB::GetValue(size_t index, bool& value) {
  if (mResultColumnTypes.size() <= index ||
      mResultColumnTypes[index] != MYSQL_TYPE_BIT) {
    return false;
  }

  auto column = mResultBindings[index];

  value = *((bool*)column.buffer);
  return true;
}

bool DatabaseQueryMariaDB::GetValue(const String& name, bool& value) {
  size_t index = GetNamedBindingIndex(name);
  if (!GetResultColumnIndex(name, index)) {
    return false;
  }

  return GetValue(index, value);
}

bool DatabaseQueryMariaDB::GetRows(
    std::list<std::unordered_map<std::string, std::vector<char>>>& rows) {
  size_t colCount = mResultColumnTypes.size();

  while (0 == mStatus) {
    std::unordered_map<std::string, std::vector<char>> m;
    for (size_t i = 0; i < colCount; i++) {
      std::string colName = mResultColumnNames[i];
      int colType = mResultColumnTypes[i];

      std::vector<char> value;

      switch (colType) {
        case MYSQL_TYPE_LONG: {
          int32_t val;
          GetValue(i, val);
          value.insert(value.begin(), reinterpret_cast<const char*>(&val),
                       reinterpret_cast<const char*>(&val) + sizeof(int32_t));
        } break;
        case MYSQL_TYPE_LONGLONG: {
          int64_t val;
          GetValue(i, val);
          value.insert(value.begin(), reinterpret_cast<const char*>(&val),
                       reinterpret_cast<const char*>(&val) + sizeof(int64_t));
        } break;
        case MYSQL_TYPE_FLOAT: {
          float val;
          GetValue(i, val);
          value.insert(value.begin(), reinterpret_cast<const char*>(&val),
                       reinterpret_cast<const char*>(&val) + sizeof(float));
        } break;
        case MYSQL_TYPE_DOUBLE: {
          double val;
          GetValue(i, val);
          value.insert(value.begin(), reinterpret_cast<const char*>(&val),
                       reinterpret_cast<const char*>(&val) + sizeof(double));
        } break;
        case MYSQL_TYPE_BLOB: {
          std::vector<char> val;
          GetValue(i, val);
          value.insert(value.begin(), reinterpret_cast<const char*>(&val),
                       reinterpret_cast<const char*>(&val) + val.size());
        } break;
        case MYSQL_TYPE_STRING:
        case MYSQL_TYPE_VAR_STRING: {
          libcomp::String val;
          GetValue(i, val);

          auto str = val.C();
          value.insert(value.begin(), str, str + val.Length());
        } break;
        case MYSQL_TYPE_BIT: {
          bool val;
          GetValue(i, val);
          value.insert(value.begin(), reinterpret_cast<const char*>(&val),
                       reinterpret_cast<const char*>(&val) + sizeof(bool));
        } break;
        default:
          mStatus = -1;
          break;
      }

      m[colName] = value;
    }
    rows.push_back(m);

    if (mStatus != 0) {
      break;
    }

    Next();
  }

  return IsValid();
}

bool DatabaseQueryMariaDB::IsValid() const {
  return nullptr != mDatabase && nullptr != mStatement &&
         (mStatus == 0 || mStatus == MYSQL_NO_DATA);
}

size_t DatabaseQueryMariaDB::GetNamedBindingIndex(const String& name) {
  auto pos = std::find(mParamNames.begin(), mParamNames.end(), name);
  if (pos == mParamNames.end()) {
    mStatus = -1;
  }

  return (size_t)(pos - mParamNames.begin());
}

bool DatabaseQueryMariaDB::GetResultColumnIndex(const String& name,
                                                size_t& index) const {
  auto iter =
      std::find(mResultColumnNames.begin(), mResultColumnNames.end(), name);
  if (iter == mResultColumnNames.end()) {
    return false;
  }

  index = (size_t)(iter - mResultColumnNames.begin());
  return true;
}

MYSQL_BIND* DatabaseQueryMariaDB::PrepareBinding(size_t index, int type) {
  if (mStatement == nullptr || index >= mStatement->param_count) {
    return nullptr;
  }

  if (mBindings.size() == 0) {
    for (unsigned int i = 0; i < mStatement->param_count; i++) {
      MYSQL_BIND b;
      memset(&b, 0, sizeof(MYSQL_BIND));

      mBufferNulls.push_back(false);
      mBufferLengths.push_back(0);

      b.is_null = &mBufferNulls.back();
      b.length = &mBufferLengths.back();

      mBindings.push_back(b);
    }
  }

  MYSQL_BIND* result = &mBindings[index];
  result->buffer_type = (enum_field_types)type;
  return result;
}

#endif  // !EXOTIC_PLATFORM
