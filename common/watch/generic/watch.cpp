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
  for(const auto &file: std::filesystem::recursive_directory_iterator(path.toStdWString())) {
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
bool DirectoryWatcher::poll() {
  namespace fs = std::filesystem; // using namespace alias for std::filesystem
  auto updated = false;

  // for created and updated files
  for(const auto &file: fs::recursive_directory_iterator(path.toStdWString())) {
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
      updated = true;
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
      updated = true;
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
      updated = true;
      it = files.erase(it);
    } else {
      ++it;
    }
  }

  return updated;
}

// --------------------------------------------------------------------------------

/**
 * @brief Construct a new WinWatch object
 */
GenericWatch::GenericWatch(QObject *parent) {
  // connects the signals and slots
  connect(&poller, &QTimer::timeout, this, &GenericWatch::poll);

  // move the poller to the thread
  poller.moveToThread(&pollerThread);

  // start timer after the thread started
  connect(&pollerThread, &QThread::started, [this] {
    poller.start(pollInterval);
  });

  // stop timer before the thread finished
  connect(&pollerThread, &QThread::finished, [this] {
    poller.stop();
  });

  // start the thread's
  pollerThread.start();
}

/**
 * @brief Destroy
 */
GenericWatch::~GenericWatch() {
  for (auto thread: {&watcherThread, &pollerThread}) {
    thread->quit();
    thread->wait();
  }
}

/**
 * @brief Poll
 */
void GenericWatch::poll() {
  QMutexLocker locker(&mutex);
  for(auto directory: directories) {
    auto lastPoll = directory->property(lastPollKey).toDateTime();
    auto interval = directory->property(pollIntervalKey).toInt();
    auto dir = QDir(directory->getPath());

    if(!dir.exists()) {
      continue;
    }

    auto diff = lastPoll.msecsTo(QDateTime::currentDateTime());
    
    if(diff < interval) {
      continue;
    }

    auto updated = directory->poll();
    auto time = QDateTime::currentDateTime();
    auto newInterval = pollInterval;

    if(!updated) {
      newInterval = std::min(interval * 2, maxPollInterval);
    }

    directory->setProperty(lastPollKey, time);
    directory->setProperty(pollIntervalKey, newInterval);
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
  auto time = QDateTime::currentDateTime();

  try {
    dirWatch = new DirectoryWatcher(path);
  } catch (const std::filesystem::filesystem_error &e) {
    emit onError(e.what());
    emit pathRemoved(path);
    return;
  }

  dirWatch->setProperty(pollIntervalKey, pollInterval);
  dirWatch->setProperty(lastPollKey, time);

  connect(
    &this->watcherThread, &QThread::finished,
    dirWatch, &QObject::deleteLater
  );

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
  auto path = QDir::cleanPath(dir);
  QMutexLocker locker(&mutex);
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
