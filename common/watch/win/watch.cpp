// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "watch.hpp"

namespace srilakshmikanthanp::pulldog::common {
void WinWatch::processFileInfo(DirWatch *dir, const FILE_NOTIFY_INFORMATION *fileInfo) {
  auto fileName = QString::fromWCharArray(fileInfo->FileName, fileInfo->FileNameLength / sizeof(WCHAR));
  auto filePath = QDir::cleanPath(dir->baseDir + "/" + fileName);
  auto relative = std::filesystem::relative(filePath.toStdString(), dir->baseDir.toStdString());
  auto relQtStr = QString::fromStdString(relative.string());

  switch (fileInfo->Action) {
    case FILE_ACTION_MODIFIED:
      emit fileUpdated(dir->baseDir, relQtStr);
      break;
    case FILE_ACTION_ADDED:
      emit fileCreated(dir->baseDir, relQtStr);
      break;
    case FILE_ACTION_RENAMED_OLD_NAME:
      dir->oldFileName = relQtStr;
      break;
    case FILE_ACTION_RENAMED_NEW_NAME:
      emit fileRename(dir->baseDir, dir->oldFileName, relQtStr);
      break;
    case FILE_ACTION_REMOVED:
      emit fileRemoved(dir->baseDir, relQtStr);
      break;
    default:
      break;
  }
}

QString WinWatch::getFileNameFromHandle(HANDLE handle) const {
  return utility::getFileNameFromHandle(handle);
}

// call ReadDirectoryChangesW
bool WinWatch::readDirectoryChanges(DirWatch *dir) {
  // Issue another call to ReadDirectoryChangesW to continue monitoring
  if (!ReadDirectoryChangesW(
    dir->handle,
    dir->buffer,
    sizeof(dir->buffer),
    dir->recursive,
    FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME |
    FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_LAST_WRITE |
    FILE_NOTIFY_CHANGE_CREATION,
    NULL,
    &dir->overlapped,
    WinWatch::DirectoryChangesCallback)) {
    return false;
  }

  return true;
}

void CALLBACK WinWatch::DirectoryChangesCallback(
  DWORD errorCode,
  DWORD numberOfBytesTransferred,
  LPOVERLAPPED overlapped
) {
  // Get the instance of the class
  auto watcher = reinterpret_cast<WinWatch*>(overlapped->hEvent);

  // Identify the directory watch instance
  DirWatch *directory = nullptr;

  for (auto dir: watcher->directories) {
    if (&dir->overlapped == overlapped) {
      directory = dir;
      break;
    }
  }

  if (directory == nullptr) {
    watcher->onError("DirectoryWatch instance not found.");
    return;
  }

  // Defer the re-issuing of ReadDirectoryChangesW
  DEFER([=]() {
    if (!watcher->readDirectoryChanges(directory)) {
      watcher->onError(QString("Error in re-issuing ReadDirectoryChangesW: %1").arg(GetLastError()));
      watcher->directories.removeOne(directory);
      CloseHandle(directory->handle);
      emit watcher->pathsChanged(directory->baseDir, false);
    }
  });

  if (errorCode != ERROR_SUCCESS) {
    watcher->onError(QString("Error in ReadDirectoryChangesW callback: %1").arg(errorCode));
    return;
  }

  if (numberOfBytesTransferred == 0) {
    return;
  }

  // Process the buffer
  FILE_NOTIFY_INFORMATION* fileInfo = nullptr;
  uint8_t* base = directory->buffer;

  while (true) {
    // Get the file info structure from the buffer
    fileInfo = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(base);

    // Process the file info structure
    watcher->processFileInfo(directory, fileInfo);

    // Check if this is the last entry
    if (fileInfo->NextEntryOffset == 0) {
      break;
    }

    // Move to the next entry
    base += fileInfo->NextEntryOffset;
  }
}

/**
 * @brief Construct a new Watcher:: Watcher object
 *
 * @param parent
 */
WinWatch::WinWatch(QObject *parent) : IWatch(parent) {
  // do nothing
}

/**
 * @brief Destroy the Watcher:: Watcher object
 */
WinWatch::~WinWatch() {
  for (auto& directory : this->directories) {
    CloseHandle(directory->handle);
  }
}

/**
 * @brief paths
 *
 * @return QStringList
 */
QStringList WinWatch::paths() const {
  auto paths = QStringList();

  for(auto handle: this->directories) {
    paths.push_back(this->getFileNameFromHandle(handle->handle));
  }

  return paths;
}

/**
 * @brief Add a path to watch
 *
 * @param path
 */
void WinWatch::addPath(const QString &path, bool recursive) {
  WinWatch::DirWatch *directory = new WinWatch::DirWatch();

  directory->handle = CreateFileW(
    path.toStdWString().c_str(),
    FILE_LIST_DIRECTORY,
    FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
    NULL,
    OPEN_EXISTING,
    FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
    NULL
  );

  if (directory->handle == INVALID_HANDLE_VALUE) {
    onError(QString("Failed to open directory: %1").arg(path));
    return;
  }

  ZeroMemory(&directory->overlapped, sizeof(OVERLAPPED));
  directory->overlapped.hEvent = reinterpret_cast<HANDLE>(this);
  directory->baseDir = path;
  directory->watcher = this;
  directory->recursive = recursive;

  if (!readDirectoryChanges(directory)) {
    onError(QString("Error in ReadDirectoryChangesW: %1").arg(GetLastError()));
    CloseHandle(directory->handle);
    emit pathsChanged(path, false);
    return;
  }

  directories.push_back(directory);

  emit pathsChanged(path, true);
}

/**
 * @brief Remove a path from watch
 *
 * @param path
 */
void WinWatch::removePath(const QString &path) {
  for (auto dir: directories) {
    if (this->getFileNameFromHandle(dir->handle) == path) {
      CloseHandle(dir->handle);
      directories.removeOne(dir);
      break;
    }
  }
}
}  // namespace srilakshmikanthanp::pulldog::common::watcher
