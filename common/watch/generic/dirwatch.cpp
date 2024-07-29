// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "dirwatch.hpp"

namespace srilakshmikanthanp::pulldog::common {
/**
 * @brief Construct a Directory Watcher object
 */
DirWatcher::DirWatcher(const QString &path, QObject *parent): QObject(parent), path(path) {
  for(const auto &file: std::filesystem::recursive_directory_iterator(path.toStdWString())) {
    auto filePath = QString::fromStdWString(file.path().wstring());
    files[filePath] = {QFileInfo(filePath), types::FileId(filePath)};
  }
}

/**
 * @brief Get the Path object
 */
QString DirWatcher::getPath() const {
  return path;
}

/**
 * @brief Poll the directory
 */
bool DirWatcher::poll() {
  QList<FileInfo> entryCreated, entryUpdated, entryRemoved;
  QList<QPair<FileInfo, FileInfo>> entryRenamed;

  namespace fs = std::filesystem;

  // lambda function that infers relative path
  auto relativePath = [this](const QString &file) {
    auto relPath = fs::relative(file.toStdWString(), path.toStdWString());
    return QDir::cleanPath(QString::fromStdWString(relPath.wstring()));
  };

  // for created and updated files
  for(const auto &file: fs::recursive_directory_iterator(path.toStdWString())) {
    auto fileInfo = QFileInfo(QString::fromStdWString(file.path().wstring()));
    auto fileId   = types::FileId(fileInfo.filePath());
    auto filePath = fileInfo.filePath();

    // if the file is not in the cache
    if(!files.contains(filePath)) {
      entryCreated.append({fileInfo, fileId});
      files[filePath] = {fileInfo, fileId};
      continue;
    }

    // get the last modified time
    auto cacheLast = std::get<0>(files[filePath]).lastModified().toUTC();
    auto fileLast = fileInfo.lastModified().toUTC();
    auto cacheSize = std::get<0>(files[filePath]).size();
    auto fileSize = fileInfo.size();

    // if the file is updated
    if(cacheLast != fileLast || cacheSize != fileSize) {
      entryUpdated.append({fileInfo, fileId});
      files[filePath] = {fileInfo, fileId};
    }
  }

  // for removed files
  for(auto it = files.begin(); it != files.end();) {
    if (!fs::exists(it.key().toStdWString())) {
      entryRemoved.append(it.value());
      it = files.erase(it);
    } else {
      ++it;
    }
  }

  // identify the renamed files from removed and created
  for(auto created: entryCreated) {
    for(auto removed: entryRemoved) {
      if(std::get<1>(created).isSameFile(std::get<1>(removed))) {
        entryRenamed.append({removed, created});
      }
    }
  }

  // remove the renamed files from removed and created
  for(auto renamed: entryRenamed) {
    entryCreated.removeOne(renamed.second);
    entryRemoved.removeOne(renamed.first);
  }

  // emit the signals for created
  for(auto info: entryCreated) {
    emit fileCreated(path, relativePath(std::get<0>(info).filePath()));
  }

  // emit the signals for updated
  for(auto info: entryUpdated) {
    emit fileUpdated(path, relativePath(std::get<0>(info).filePath()));
  }

  // emit the signals for removed
  for(auto info: entryRemoved) {
    emit fileRemoved(path, relativePath(std::get<0>(info).filePath()));
  }

  // emit the signals for renamed
  for(auto info: entryRenamed) {
    auto newFile = relativePath(std::get<0>(info.second).filePath());
    auto oldFile = relativePath(std::get<0>(info.first).filePath());
    emit fileRename(path, oldFile, newFile);
  }

  // return true if any change
  return entryCreated.size() || entryUpdated.size() || entryRemoved.size();
}
} // namespace srilakshmikanthanp::pulldog::common
