// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "controller.hpp"

namespace srilakshmikanthanp::pulldog {
void Controller::handleFileUpdate(const QString dir, const QString path) {
  // get the destination file path from the destination root
  auto destFile = destinationRoot.filePath(path);
  auto srcFile = QDir(dir).filePath(path);

  // create all parent directories
  if(!QDir().mkpath(QFileInfo(destFile).dir().path())) {
    emit onError("Failed to create parent directories for " + destFile);
    return;
  }

  // Create a key for the pending file update
  auto key = QPair<QString, QString>(srcFile, destFile);

  // Add or update the pending file update
  pendingFileUpdate[key] = QDateTime::currentMSecsSinceEpoch();
}

/**
 * @brief Process the pending file update
 */
void Controller::processPendingFileUpdate() {
  // get the current time
  auto currentTime = QDateTime::currentMSecsSinceEpoch();

  // iterate the pending file update
  for(auto pending: pendingFileUpdate.keys()) {
    // get the last update time
    auto lastUpdateTime = pendingFileUpdate[pending];

    // check the threshold
    if(currentTime - lastUpdateTime < THRESHOLD) {
      continue;
    }

    // update the last update time
    pendingFileUpdate[pending] = currentTime;

    // extract the source and destination file
    auto destFile = pending.second;
    auto srcFile = pending.first;

    // create an locker object
    common::Locker locker(srcFile, types::LockMode::READ);

    // try to lock the file
    if(locker.tryLock() < 0) {
      continue;
    }

    // unlock the file
    locker.unlock();

    // emit the copy signal
    emit copy(srcFile, destFile);

    // remove the pending file update
    pendingFileUpdate.remove(pending);
  }
}

/**
 * @brief Construct a new Controller object
 */
Controller::Controller(const QString &destRoot, QObject *parent) : QObject(parent) {
  // set the destination root directory
  this->destinationRoot = QDir(destRoot);

  // move the copier object to the thread
  copier.moveToThread(&thread);

  // connect the signals
  connect(
    &watcher, &common::Watch::fileCreated,
    this, &Controller::handleFileUpdate
  );

  connect(
    &watcher, &common::Watch::fileUpdated,
    this, &Controller::handleFileUpdate
  );

  connect(
    &watcher, &common::Watch::pathsChanged,
    this, &Controller::pathsChanged
  );

  // copy signal to copier
  connect(
    this, &Controller::copy,
    &copier, &common::Copier::copy
  );

  // connect the signals
  connect(
    &copier, &common::Copier::onCopyStart,
    this, &Controller::onCopyStart
  );

  connect(
    &copier, &common::Copier::onCopy,
    this, &Controller::onCopy
  );

  connect(
    &copier, &common::Copier::onCopyEnd,
    this, &Controller::onCopyEnd
  );

  connect(
    &copier, &common::Copier::onError,
    this, &Controller::onError
  );

  // connect the timer
  connect(
    &timer, &QTimer::timeout,
    this, &Controller::processPendingFileUpdate
  );

  // start the thread
  thread.start();

  // start the timer
  timer.start(THRESHOLD / 2);
}

Controller::~Controller() {
  thread.quit();
  thread.wait();
}

/**
 * @brief get the destination root
 */
QString Controller::getDestinationRoot() const {
  return destinationRoot.path();
}

/**
 * @brief Add a path to watch
 *
 * @param path
 */
void Controller::addPath(const QString &path, bool recursive) {
  return watcher.addPath(path, recursive);
}

/**
 * @brief Get the Paths object
 */
QStringList Controller::paths() const {
  return watcher.paths();
}

/**
 * @brief Remove a path from watch
 */
void Controller::removePath(const QString &path) {
  watcher.removePath(path);
}
}  // namespace srilakshmikanthanp::pulldog
