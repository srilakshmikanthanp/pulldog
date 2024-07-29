// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "watch.hpp"

namespace srilakshmikanthanp::pulldog::common {
void WinWatch::processFileInfo(
  DirWatch *dir, const FILE_NOTIFY_INFORMATION *fileInfo,
  QList<QPair<QString, QString>> &entryCreated,
  QList<QPair<QString, QString>> &entryUpdated,
  QList<QPair<QString, QString>> &entryRemoved,
  QList<std::tuple<QString, QString, QString>> &entryRenamed
) {
  auto fileName = QString::fromWCharArray(fileInfo->FileName, fileInfo->FileNameLength / sizeof(WCHAR));
  auto filePath = QDir::cleanPath(dir->baseDir + "/" + fileName);
  auto relative = std::filesystem::relative(filePath.toStdString(), dir->baseDir.toStdString());
  auto relQtStr = QDir::cleanPath(QString::fromStdString(relative.string()));

  switch (fileInfo->Action) {
    case FILE_ACTION_MODIFIED:
      entryUpdated.append({dir->baseDir, relQtStr});
      break;
    case FILE_ACTION_ADDED:
      entryCreated.append({dir->baseDir, relQtStr});
      break;
    case FILE_ACTION_RENAMED_OLD_NAME:
      dir->oldFileName = relQtStr;
      break;
    case FILE_ACTION_RENAMED_NEW_NAME:
      entryRenamed.append({dir->baseDir, dir->oldFileName, relQtStr});
      break;
    case FILE_ACTION_REMOVED:
      entryRemoved.append({dir->baseDir, relQtStr});
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
  return ReadDirectoryChangesW(
    dir->handle,
    dir->buffer,
    sizeof(dir->buffer),
    dir->recursive,
    FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME |
    FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_LAST_WRITE |
    FILE_NOTIFY_CHANGE_CREATION,
    NULL,
    &dir->overlapped,
    WinWatch::DirectoryChangesCallback
  );
}

void CALLBACK WinWatch::DirectoryChangesCallback(
  DWORD errorCode,
  DWORD numberOfBytesTransferred,
  LPOVERLAPPED overlapped
) {
  // list of events for created, updated, removed and renamed
  QList<QPair<QString, QString>> entryCreated, entryUpdated, entryRemoved;
  QList<std::tuple<QString, QString, QString>> entryRenamed;

  // Get the instance of the class
  auto watcher = reinterpret_cast<WinWatch*>(overlapped->hEvent);

  // Identify the directory watch instance
  DirWatch *directory = nullptr;

  // Find the directory instance
  for (auto dir: watcher->directories) {
    if (&dir->overlapped == overlapped) {
      directory = dir;
      break;
    }
  }

  // Check if the directory instance is found
  if (directory == nullptr) {
    return watcher->onError("DirectoryWatch instance not found.");
  }

  // Defer the re-issuing of ReadDirectoryChangesW
  DEFER([=]() {
    if (!watcher->readDirectoryChanges(directory)) {
      watcher->onError(QString("Error in re-issuing ReadDirectoryChangesW: %1").arg(GetLastError()));
      watcher->directories.removeOne(directory);
      CloseHandle(directory->handle);
      emit watcher->pathRemoved(directory->baseDir);
    }
  });

  if (errorCode != ERROR_SUCCESS) {
    return watcher->onError(QString("Error in ReadDirectoryChangesW callback: %1").arg(errorCode));
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
    watcher->processFileInfo(
      directory, fileInfo,
      entryCreated,
      entryUpdated,
      entryRemoved,
      entryRenamed
    );

    // Check if this is the last entry
    if (fileInfo->NextEntryOffset == 0) {
      break;
    }

    // Move to the next entry
    base += fileInfo->NextEntryOffset;
  }

  // Emit the signals for created
  for (auto file: entryCreated) {
    emit watcher->fileCreated(file.first, file.second);
  }

  // Emit the signals for updated
  for (auto file: entryUpdated) {
    emit watcher->fileUpdated(file.first, file.second);
  }

  // Emit the signals for removed
  for (auto file: entryRemoved) {
    emit watcher->fileRemoved(file.first, file.second);
  }

  // Emit the signals for renamed
  for (auto file: entryRenamed) {
    auto dirPath = std::get<0>(file);
    auto oldFile = std::get<1>(file);
    auto newFile = std::get<2>(file);
    emit watcher->fileRename(dirPath, oldFile, newFile);
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
void WinWatch::addPath(const QString &dir, bool recursive) {
  WinWatch::DirWatch *directory = new WinWatch::DirWatch();
  auto path = QDir::cleanPath(dir);

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
    emit pathRemoved(path);
    return;
  }

  directories.push_back(directory);

  emit pathAdded(path);
}

/**
 * @brief Remove a path from watch
 *
 * @param path
 */
void WinWatch::removePath(const QString &dir) {
  auto path = QDir::cleanPath(dir);
  for (auto dir: directories) {
    if (this->getFileNameFromHandle(dir->handle) == path) {
      CloseHandle(dir->handle);
      directories.removeOne(dir);
      break;
    }
  }
}
}  // namespace srilakshmikanthanp::pulldog::common::watcher
