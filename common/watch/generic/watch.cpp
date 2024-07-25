// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "watch.hpp"

namespace srilakshmikanthanp::pulldog::common {
/**
 * @brief Construct a Directory Watcher object
 */
DirectoryWatcher::DirectoryWatcher(const QString &path, QObject *parent): QObject(parent), path(path) {
  for(const auto &file: std::filesystem::recursive_directory_iterator(path.toStdString())) {
    auto fileInfo = QFileInfo(QString::fromStdWString(file.path().wstring()));
    auto filePath = fileInfo.filePath();
    files[filePath] = fileInfo;
  }
}

/**
 * @brief Get the Path object
 */
QString DirectoryWatcher::getPath() const {
  return path;
}

/**
 * @brief Poll the directory
 */
void DirectoryWatcher::poll() {
  namespace fs = std::filesystem; // using namespace alias for std::filesystem

  // for created and updated files
  for(const auto &file: fs::recursive_directory_iterator(path.toStdString())) {
    auto fileInfo = QFileInfo(QString::fromStdWString(file.path().wstring()));
    auto filePath = fileInfo.filePath();

    // infer relative path using fs::relative
    auto relPath = fs::relative(file.path(), path.toStdWString());
    auto relStr = QString::fromStdWString(relPath.wstring());

    // to preferred style
    relStr = QDir::cleanPath(relStr);

    // if the file is not in the cache
    if(!files.contains(filePath)) {
      files[filePath] = fileInfo;
      emit fileCreated(path, relStr);
      continue;
    }

    // get the last modified time
    auto cacheLast = files[filePath].lastModified().toUTC();
    auto fileLast = fileInfo.lastModified().toUTC();
    auto cacheSize = files[filePath].size();
    auto fileSize = fileInfo.size();

    // if the file is updated
    if(cacheLast != fileLast || cacheSize != fileSize) {
      files[filePath] = fileInfo;
      emit fileUpdated(path, relStr);
    }
  }

  // for removed files
  for(auto it = files.begin(); it != files.end();) {
    if (!fs::exists(it.key().toStdWString())) {
      auto keyPath = it.key().toStdWString();
      auto file = path.toStdWString();
      auto relPath = fs::relative(keyPath, file);
      auto relStr = QString::fromStdWString(relPath.wstring());
      emit fileRemoved(path, relStr);
      it = files.erase(it);
    } else {
      ++it;
    }
  }
}

// --------------------------------------------------------------------------------

/**
 * @brief Construct a new WinWatch object
 */
GenericWatch::GenericWatch(QObject *parent) {
  // connects the signals and slots
  connect(&poller, &QTimer::timeout, this, &GenericWatch::poll);
  poller.moveToThread(&pollerThread);
  connect(&pollerThread, &QThread::started, [this] {
    poller.start(pollInterval);
  });

  // start the thread's
  pollerThread.start();
}

/**
 * @brief Poll
 */
void GenericWatch::poll() {
  QMutexLocker locker(&mutex);
  for(auto directory: directories) {
    auto dir = QDir(directory->getPath());
    if(dir.exists()) {
      directory->poll();
    }
  }
}

/**
 * @brief Add a path to watch
 *
 * @param path
 */
void GenericWatch::addPath(const QString &dir, bool recursive) {
  DirectoryWatcher* dirWatch = nullptr;
  auto path = QDir::cleanPath(dir);

  try {
    dirWatch = new DirectoryWatcher(path);
  } catch (const std::filesystem::filesystem_error &e) {
    emit onError(e.what());
    emit pathRemoved(path);
    return;
  }

  dirWatch->moveToThread(&watcherThread);

  connect(
    dirWatch, &DirectoryWatcher::fileCreated,
    this, &GenericWatch::fileCreated
  );

  connect(
    dirWatch, &DirectoryWatcher::fileRemoved,
    this, &GenericWatch::fileRemoved
  );

  connect(
    dirWatch, &DirectoryWatcher::fileUpdated,
    this, &GenericWatch::fileUpdated
  );

  QMutexLocker locker(&mutex);
  directories.append(dirWatch);
  emit pathAdded(QDir::cleanPath(dir));
  locker.unlock();
}

/**
 * @brief Remove a path from watch
 *
 * @param path
 */
void GenericWatch::removePath(const QString &dir) {
  QMutexLocker locker(&mutex);
  auto path = QDir::cleanPath(dir);

  directories.removeIf([path](auto dir) {
    return dir->getPath() == path;
  });

  emit pathRemoved(path);
}

/**
 * @brief paths
 */
QStringList GenericWatch::paths() const {
  QMutexLocker locker(&mutex);
  QStringList paths;

  for(const auto directory: directories) {
    paths << directory->getPath();
  }

  return paths;
}
} // namespace srilakshmikanthanp::pulldog::common
