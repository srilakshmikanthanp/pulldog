// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "watchimpl.hpp"

namespace srilakshmikanthanp::pulldog::common {
QString WatchImpl::getFileNameFromHandle(HANDLE handle) const {
  auto buffer = std::make_unique<WCHAR[]>(MAX_PATH);
  auto fileName = QString();

  auto size = GetFinalPathNameByHandleW(
    handle,
    buffer.get(),
    MAX_PATH,
    VOLUME_NAME_DOS
  );

  if(size != 0) {
    fileName = QString::fromWCharArray(buffer.get(), size);
  } else {
    throw std::runtime_error(QString("Error: %1").arg(GetLastError()).toStdString());
  }

  return fileName;
}

void WatchImpl::processFileInfo(Directory dir, const FILE_NOTIFY_INFORMATION *fileInfo) {
  auto fileName = QString::fromWCharArray(fileInfo->FileName, fileInfo->FileNameLength / sizeof(WCHAR));

  switch (fileInfo->Action) {
    case FILE_ACTION_ADDED:
      emit fileCreated(dir.baseDir, fileName);
      break;
    case FILE_ACTION_REMOVED:
      emit fileRemoved(dir.baseDir, fileName);
      break;
    case FILE_ACTION_MODIFIED:
      emit fileUpdated(dir.baseDir, fileName);
      break;
    default:
      break;
  }
}

void CALLBACK WatchImpl::DirectoryChangesCallback(
  DWORD errorCode,
  DWORD numberOfBytesTransferred,
  LPOVERLAPPED overlapped
) {
  // Get the instance of the class
  auto watcher = reinterpret_cast<WatchImpl*>(overlapped->hEvent);

  if (errorCode != ERROR_SUCCESS) {
    watcher->onError(QString("Error in ReadDirectoryChangesW callback: %1").arg(errorCode));
    return;
  }

  if (numberOfBytesTransferred == 0) {
    return;
  }

  // Identify the directory watch instance
  Directory* directory = nullptr;
  
  for (auto& d : watcher->directories) {
    if (&d.overlapped == overlapped) {
      directory = &d;
      break;
    }
  }

  if (directory == nullptr) {
    watcher->onError("DirectoryWatch instance not found.");
    return;
  }

  // Process the buffer
  FILE_NOTIFY_INFORMATION* fileInfo = nullptr;
  char* base = directory->buffer;

  while (true) {
    // Get the file info structure from the buffer
    fileInfo = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(base);

    // Process the file info structure
    watcher->processFileInfo(*directory, fileInfo);

    // Check if this is the last entry
    if (fileInfo->NextEntryOffset == 0) {
      break;
    }
    
    // Move to the next entry
    base += fileInfo->NextEntryOffset;
  }

  // Issue another call to ReadDirectoryChangesW to continue monitoring
  if (!ReadDirectoryChangesW(
    directory->handle,
    directory->buffer,
    sizeof(directory->buffer),
    directory->recursive,
    FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME |
    FILE_NOTIFY_CHANGE_ATTRIBUTES | FILE_NOTIFY_CHANGE_SIZE |
    FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_LAST_ACCESS |
    FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_SECURITY,
    NULL,
    &directory->overlapped,
    &DirectoryChangesCallback)) {
    watcher->onError(QString("Error in re-issuing ReadDirectoryChangesW: %1").arg(GetLastError()));
  }
}

/**
 * @brief Construct a new Watcher:: Watcher object
 *
 * @param parent
 */
WatchImpl::WatchImpl(QObject *parent) : QObject(parent) {
  // Do nothing
}

/**
 * @brief Destroy the Watcher:: Watcher object
 */
WatchImpl::~WatchImpl() {
  for (auto& directory : this->directories) {
    CloseHandle(directory.handle);
  }
}

/**
 * @brief paths
 *
 * @return QStringList
 */
QStringList WatchImpl::paths() const {
  auto paths = QStringList();

  for(auto handle: this->directories) {
    paths.push_back(this->getFileNameFromHandle(handle.handle));
  }

  return paths;
}

/**
 * @brief Add a path to watch
 *
 * @param path
 */
bool WatchImpl::addPath(const QString &path, bool recursive) {
  WatchImpl::Directory directory;
  
  directory.handle = CreateFile(
    path.toStdWString().c_str(),
    FILE_LIST_DIRECTORY,
    FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
    NULL,
    OPEN_EXISTING,
    FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
    NULL
  );

  if (directory.handle == INVALID_HANDLE_VALUE) {
    onError(QString("Failed to open directory: %1").arg(path));
    return false;
  }

  ZeroMemory(&directory.overlapped, sizeof(OVERLAPPED));
  directory.overlapped.hEvent = reinterpret_cast<HANDLE>(this);
  directory.baseDir = path;
  directory.watcher = this;
  directory.recursive = recursive;
  directories.push_back(directory);

  if (!ReadDirectoryChangesW(
    directory.handle,
    directory.buffer,
    sizeof(directory.buffer),
    directory.recursive,  // Monitor the directory tree
    FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME |
    FILE_NOTIFY_CHANGE_ATTRIBUTES | FILE_NOTIFY_CHANGE_SIZE |
    FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_LAST_ACCESS |
    FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_SECURITY,
    NULL,
    &directory.overlapped,
    &WatchImpl::DirectoryChangesCallback)) {
    return false;
  }

  return true;
}

/**
 * @brief Remove a path from watch
 *
 * @param path
 */
void WatchImpl::removePath(const QString &path) {
  for (auto it = this->directories.begin(); it != this->directories.end(); ++it) {
    if (this->getFileNameFromHandle(it->handle) == path) {
      CloseHandle(it->handle);
      directories.erase(it);
      break;
    }
  }
}
}  // namespace srilakshmikanthanp::pulldog::common::watcher
