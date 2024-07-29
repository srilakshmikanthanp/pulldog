// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "watch.hpp"

namespace srilakshmikanthanp::pulldog::common {
/**
 * @brief Construct a new WinWatch object
 */
GenericWatch::GenericWatch(QObject *parent) {
  // connects the signals and slots
  connect(&poller, &QTimer::timeout, this, &GenericWatch::poll, Qt::DirectConnection);

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
  for (auto thread: {&pollerThread}) {
    thread->quit();
    thread->wait();
  }
}

/**
 * @brief Poll the directory on its own thread
 */
void GenericWatch::pollDirectory(DirWatcher *directory) {
  auto lastPoll = directory->property(lastPollKey).toDateTime();
  auto diff = lastPoll.msecsTo(QDateTime::currentDateTime());
  auto interval = directory->property(pollIntervalKey).toInt();
  auto dir = QDir(directory->getPath());

  if(!dir.exists() || diff < interval) {
    return;
  }

  auto updated = false;

  try {
    updated = directory->poll();
  } catch (const std::filesystem::filesystem_error &e) {
    return emit onError(e.what());
  }

  auto time = QDateTime::currentDateTime();
  auto nInt = pollInterval;

  if(!updated) {
    nInt = std::min(interval * 2, maxPollInterval);
  }

  directory->setProperty(lastPollKey, time);
  directory->setProperty(pollIntervalKey, nInt);
}

/**
 * @brief Poll
 */
void GenericWatch::poll() {
  QMutexLocker locker(&mutex);
  for(auto directory: directories) {
    this->pollDirectory(directory);
  }
}

/**
 * @brief Add a path to watch
 *
 * @param path
 */
void GenericWatch::addPath(const QString &dir, bool recursive) {
  DirWatcher* dirWatch = nullptr;
  auto path = QDir::cleanPath(dir);
  auto time = QDateTime::currentDateTime();

  try {
    dirWatch = new DirWatcher(path);
  } catch (const std::filesystem::filesystem_error &e) {
    emit onError(e.what());
    emit pathRemoved(path);
    return;
  }

  dirWatch->setProperty(pollIntervalKey, pollInterval);
  dirWatch->setProperty(lastPollKey, time);

  connect(
    &this->pollerThread, &QThread::finished,
    dirWatch, &QObject::deleteLater
  );

  connect(
    dirWatch, &DirWatcher::fileCreated,
    this, &GenericWatch::fileCreated
  );

  connect(
    dirWatch, &DirWatcher::fileRemoved,
    this, &GenericWatch::fileRemoved
  );

  connect(
    dirWatch, &DirWatcher::fileUpdated,
    this, &GenericWatch::fileUpdated
  );

  connect(
    dirWatch, &DirWatcher::fileRename,
    this, &GenericWatch::fileRename
  );

  dirWatch->moveToThread(&pollerThread);

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
