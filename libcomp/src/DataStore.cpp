/**
 * @file libcomp/src/DataStore.cpp
 * @ingroup libcomp
 *
 * @author COMP Omega <compomega@tutanota.com>
 *
 * @brief Class to manage the data store (for static game data).
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

// libcomp Includes
#include "DataStore.h"

#include "Crypto.h"
#include "DataFile.h"
#include "Log.h"

#ifndef EXOTIC_PLATFORM
#include "ScriptEngine.h"
#endif  // !EXOTIC_PLATFORM

// Standard C++11 Includes
#include <limits>

// PhysFS Includes
#include <physfs.h>

using namespace libcomp;

DataStore::DataStore(const char* szProgram) {
  // Init PhysFS.
  if (0 == PHYSFS_init(szProgram)) {
    LogDataStoreCriticalMsg("Failed to init PhysFS!\n");
  }

  // Allow symlinks in the datastore directory.
  PHYSFS_permitSymbolicLinks(1);
}

DataStore::~DataStore() {
  // Make sure PhysFS is cleaned up.
  if (0 == PHYSFS_deinit()) {
    LogDataStoreWarningMsg("Failed to cleanup PhysFS.\n");
  }
}

libcomp::String DataStore::GetError() {
#if (2 < PHYSFS_VER_MAJOR) || (2 == PHYSFS_VER_MAJOR && (1 <= PHYSFS_VER_MINOR))
  return PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
#else
  return PHYSFS_getLastError();
#endif
}

bool DataStore::AddSearchPaths(const std::list<libcomp::String>& paths) {
  // There must be at least one path.
  if (paths.empty()) {
    return false;
  }

  // This will be used as the write directory.
  libcomp::String lastPath = paths.back();

  // Search order will be last to first path in this list.
  for (libcomp::String path : paths) {
    if (!AddSearchPath(path)) {
      return false;
    }
  }

  // Set the write directory.
  if (0 == PHYSFS_setWriteDir(lastPath.C())) {
    LogDataStoreError([&]() {
      return String("DataStore: Failed to set the write directory to: %1\n")
          .Arg(lastPath);
    });

    return false;
  }

  return true;
}

bool DataStore::GetListing(const libcomp::String& path,
                           std::list<libcomp::String>& files,
                           std::list<libcomp::String>& dirs,
                           std::list<libcomp::String>& symLinks, bool recursive,
                           bool fullPath) {
  char** szFiles = PHYSFS_enumerateFiles(path.C());

  if (NULL == szFiles) {
    LogDataStoreWarning([&]() {
      return String("DataStore: Failed to enumerate directory: %1\n").Arg(path);
    });

    return false;
  }

  libcomp::String basePath = path;

  if ("/" != basePath.Right(1)) {
    basePath += "/";
  }

  for (char** szFile = szFiles; NULL != *szFile; ++szFile) {
    libcomp::String fileName = *szFile;
    libcomp::String filePath = basePath + fileName;

    if (fullPath) {
      fileName = filePath;
    }

    const char* szFilePath = filePath.C();

#if (2 < PHYSFS_VER_MAJOR) || (2 == PHYSFS_VER_MAJOR && (1 <= PHYSFS_VER_MINOR))
    PHYSFS_Stat stat;

    if (0 != PHYSFS_stat(szFilePath, &stat) &&
        stat.filetype == PHYSFS_FILETYPE_DIRECTORY) {
#else
    if (0 != PHYSFS_isDirectory(szFilePath)) {
#endif
      if (recursive) {
        std::list<libcomp::String> recursiveFiles;
        std::list<libcomp::String> recursiveDirs;
        std::list<libcomp::String> recursiveSymLinks;

        if (!GetListing(filePath, recursiveFiles, recursiveDirs,
                        recursiveSymLinks, recursive, fullPath)) {
          return false;
        }

        if (!fullPath) {
          // Append the relative path onto the files.
          for (libcomp::String& file : recursiveFiles) {
            file = fileName + libcomp::String("/") + file;
          }

          // Append the relative path onto the directories.
          for (libcomp::String& dir : recursiveDirs) {
            dir = fileName + libcomp::String("/") + dir;
          }

          // Append the relative path onto the symlinks.
          for (libcomp::String& symLink : recursiveSymLinks) {
            symLink = fileName + libcomp::String("/") + symLink;
          }
        }

        files.splice(files.end(), recursiveFiles);
        dirs.splice(dirs.end(), recursiveDirs);
        symLinks.splice(symLinks.end(), recursiveSymLinks);
      }

      dirs.push_back(fileName);
#if (2 < PHYSFS_VER_MAJOR) || (2 == PHYSFS_VER_MAJOR && (1 <= PHYSFS_VER_MINOR))
    } else if (0 != PHYSFS_stat(szFilePath, &stat) &&
               stat.filetype == PHYSFS_FILETYPE_SYMLINK) {
#else
    } else if (0 != PHYSFS_isSymbolicLink(szFilePath)) {
#endif
      symLinks.push_back(fileName);
    } else {
      files.push_back(fileName);
    }
  }

  PHYSFS_freeList(szFiles);

  return true;
}

bool DataStore::PrintListing(const libcomp::String& path, bool recursive,
                             bool fullPath) {
  std::list<libcomp::String> files;
  std::list<libcomp::String> dirs;
  std::list<libcomp::String> symLinks;

  if (GetListing(path, files, dirs, symLinks, recursive, fullPath)) {
    LogDataStoreDebug([&]() { return String("Listing for: %1\n").Arg(path); });

    for (libcomp::String file : files) {
      LogDataStoreDebug([&]() { return String("File: %1\n").Arg(file); });
    }

    for (libcomp::String dir : dirs) {
      LogDataStoreDebug([&]() { return String("Directory: %1\n").Arg(dir); });
    }

    for (libcomp::String symLink : symLinks) {
      LogDataStoreDebug(
          [&]() { return String("Symbolic Link: %1\n").Arg(symLink); });
    }

    return true;
  }

  return false;
}

bool DataStore::AddSearchPath(const libcomp::String& path, bool append) {
  static const char* szMountPoint = "/";

  // Prepend the path to the search path list. The list is searched from
  // first to last when looking for a file so a path earlier in the list
  // will override files contained in a path later in the list.
  if (0 == PHYSFS_mount(path.C(), szMountPoint, append ? 1 : 0)) {
    LogDataStoreError(
        [&]() { return String("DataStore: Error:  %1\n").Arg(GetError()); });

    return false;
  }

  return true;
}

DataFile* DataStore::Open(const libcomp::String& path, FileMode_t mode) {
  auto f = new DataFile(path, mode);

  if (!f->IsOpen()) {
    delete f;

    return nullptr;
  }

  return f;
}

bool DataStore::Exists(const libcomp::String& path) {
  return 0 != PHYSFS_exists(path.C());
}

int64_t DataStore::FileSize(const libcomp::String& path) {
  auto f = Open(path);

  if (nullptr == f) {
    return -1;
  }

  int64_t size = f->GetSize();

  delete f;
  f = nullptr;

  return size;
}

bool DataStore::Delete(const libcomp::String& path, bool recursive) {
  const char* szPath = path.C();

#if (2 < PHYSFS_VER_MAJOR) || (2 == PHYSFS_VER_MAJOR && (1 <= PHYSFS_VER_MINOR))
  PHYSFS_Stat stat;

  if (recursive && 0 != PHYSFS_stat(szPath, &stat) &&
      stat.filetype == PHYSFS_FILETYPE_DIRECTORY) {
#else
  if (recursive && 0 != PHYSFS_isDirectory(szPath)) {
#endif
    std::list<libcomp::String> files;
    std::list<libcomp::String> dirs;
    std::list<libcomp::String> symLinks;

    if (GetListing(path, files, dirs, symLinks, true, true)) {
      for (auto file : files) {
        if (!Delete(file)) {
          return false;
        }
      }

      for (auto symLink : symLinks) {
        if (!Delete(symLink)) {
          return false;
        }
      }

      for (auto dir : dirs) {
        if (!Delete(dir)) {
          return false;
        }
      }
    }
  }

  return 0 != PHYSFS_delete(szPath);
}

bool DataStore::CreateDirectory(const libcomp::String& path) {
  return 0 != PHYSFS_mkdir(path.C());
}

std::vector<char> DataStore::ReadFile(const libcomp::String& path) {
  auto f = Open(path);

  if (nullptr == f) {
    return {};
  }

  int64_t size = f->GetSize();

  if (0 > size || std::numeric_limits<uint32_t>::max() < size) {
    delete f;
    f = nullptr;

    return {};
  }

  std::vector<char> buffer = f->Read(static_cast<uint32_t>(size));

  delete f;
  f = nullptr;

  return buffer;
}

bool DataStore::WriteFile(const libcomp::String& path,
                          const std::vector<char>& data) {
  auto f = Open(path, FileMode_t::WRITE);

  if (nullptr == f) {
    return false;
  }

  bool ok = f->Write(data);

  delete f;
  f = nullptr;

  return ok;
}

std::vector<char> DataStore::DecryptFile(const libcomp::String& path) {
  std::vector<char> data = ReadFile(path);

  if (!data.empty() && Crypto::DecryptFile(data)) {
    return data;
  }

  return {};
}

bool DataStore::EncryptFile(const libcomp::String& path,
                            const std::vector<char>& data) {
  std::vector<char> dataCopy = data;

  if (!Crypto::EncryptFile(dataCopy)) {
    return false;
  }

  return WriteFile(path, dataCopy);
}

libcomp::String DataStore::GetHash(const libcomp::String& path) {
  std::vector<char> data = ReadFile(path);

  if (!data.empty()) {
    return Crypto::SHA1(data);
  }

  return {};
}

#ifndef EXOTIC_PLATFORM
namespace libcomp {
template <>
ScriptEngine& ScriptEngine::Using<DataStore>() {
  if (!BindingExists("DataStore")) {
    Sqrat::Class<DataStore, Sqrat::NoConstructor<DataStore>> binding(
        mVM, "DataStore");
    Bind<DataStore>("DataStore", binding);

    binding.Func("GetError", &DataStore::GetError)
        .Func("Exists", &DataStore::Exists);  // Last call to binding
  }

  return *this;
}
}  // namespace libcomp
#endif  // !EXOTIC_PLATFORM
