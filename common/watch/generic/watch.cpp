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

  qDebug() << "Watching: " << files.size() << " files";
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

    // if the file is not in the cache
    if(!files.contains(filePath)) {
      emit fileCreated(path, relStr);
      files[filePath] = fileInfo;
      continue;
    }

    // get the last modified time
    auto cacheLast = files[filePath].lastModified().toUTC();
    auto fileLast = fileInfo.lastModified().toUTC();

    // if the file is updated
    if(cacheLast != fileLast) {
      emit fileUpdated(path, relStr);
      files[filePath] = fileInfo;
    }
  }

  // for removed files
  for(auto it = files.begin(); it != files.end();) {
    if (!fs::exists(it.key().toStdWString())) {
      auto relPath = fs::relative(it.key().toStdWString(), path.toStdWString());
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
  connect(&watcher, &WinWatch::fileCreated, this, &GenericWatch::fileCreated);
  connect(&watcher, &WinWatch::fileRemoved, this, &GenericWatch::fileRemoved);
  connect(&watcher, &WinWatch::fileUpdated, this, &GenericWatch::fileUpdated);
  connect(&watcher, &WinWatch::fileRename, this, &GenericWatch::fileRename);

  connect(&poller, &QTimer::timeout, this, &GenericWatch::poll);

  // move to the thread
#ifdef _WIN32
  watcher.moveToThread(&watcherThread);
  watcherThread.start();
#endif

  poller.moveToThread(&pollerThread);

  // start the poller
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
    directory->poll();
  }
}

/**
 * @brief Add a path to watch
 *
 * @param path
 */
void GenericWatch::addPath(const QString &path, bool recursive) {
  DirectoryWatcher *dirWatch = nullptr;

  try {
    dirWatch = new DirectoryWatcher(path);
  } catch(const std::exception &e) {
    emit onError(e.what());
    emit pathRemoved(path);
    return;
  }

#ifdef _WIN32
  this->watcher.addPath(path, recursive);
#endif

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
  emit pathAdded(path);
  locker.unlock();
}

/**
 * @brief Remove a path from watch
 *
 * @param path
 */
void GenericWatch::removePath(const QString &path) {
  QMutexLocker locker(&mutex);

#ifdef _WIN32
  watcher.removePath(path);
#endif

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
