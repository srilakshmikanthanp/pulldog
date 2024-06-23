// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "controller.hpp"

namespace srilakshmikanthanp::pulldog {
void Controller::handleFileUpdate(const QString dir, const QString path) {
  // get the destination file path from the destination root
  auto destFile = destinationRoot.filePath(QFileInfo(dir).fileName() + "/" + path);
  auto srcFile = QDir(dir).filePath(path);

  // Create a key for the pending file update
  auto key = models::Transfer(srcFile, destFile);

  // Add or update the pending file update
  pendingFiles[key] = QDateTime::currentMSecsSinceEpoch();
}

void Controller::handleFileRename(
  const QString directory,
  const QString oldFile,
  const QString newFile) {
  this->handleFileUpdate(directory, newFile);
}

/**
 * @brief start to Copy the file with copier object
 */
void Controller::copy(const models::Transfer &transfer) {
  if(copingFiles.contains(transfer)) {
    auto copier = copingFiles[transfer];
    copier->cancel();
    copingFiles.remove(transfer);
  }

  auto srcFile = transfer.getFrom();
  auto destFile = transfer.getTo();

  // create all parent directories
  if(!QDir().mkpath(QFileInfo(destFile).dir().path())) {
    emit onError("Failed to create parent directories for " + destFile);
    return;
  }

  // create a copier object
  auto copier = new common::Copier(transfer, this);

  // connect the signals
  connect(
    copier, &common::Copier::onCopyStart,
    this, &Controller::onCopyStart
  );

  connect(
    copier, &common::Copier::onCopy,
    this, &Controller::onCopy
  );

  connect(
    copier, &common::Copier::onCopyEnd,
    this, &Controller::onCopyEnd
  );

  connect(
    copier, &common::Copier::onCopyCancel,
    this, &Controller::onCopyCancel
  );

  connect(
    copier, &common::Copier::onError,
    this, &Controller::onError
  );

  // for delete the copier object
  connect(
    copier, &common::Copier::onCopyEnd,
    copier, &common::Copier::deleteLater
  );

  connect(
    copier, &common::Copier::onCopyCancel,
    copier, &common::Copier::deleteLater
  );

  // add the copier to the coping files
  copingFiles[transfer] = copier;

  // to remove the copier from the map
  connect(
    copier, &common::Copier::destroyed,
    [this, transfer]() {
      copingFiles.remove(transfer);
    }
  );

  // start the copier
  copier->start();
}

/**
 * @brief Process the pending file update
 */
void Controller::processPendingFileUpdate() {
  // get the current time
  auto currentTime = QDateTime::currentMSecsSinceEpoch();

  // check the concurrent copies
  if(copingFiles.size() >= concurrentCopies) {
    return;
  }

  // iterate the pending file update
  for(auto pending: pendingFiles.keys()) {
    // get the last update time
    auto lastUpdateTime = pendingFiles[pending];

    // check the threshold
    if(currentTime - lastUpdateTime < threshold) {
      continue;
    }

    // update the last update time
    pendingFiles[pending] = currentTime;

    // extract the source and destination file
    auto srcFile = pending.getFrom();

    // create an locker object
    common::Locker locker(srcFile, types::LockMode::READ);

    // try to lock the file
    if(locker.tryLock() < 0) {
      continue;
    }

    // unlock the file
    locker.unlock();

    // remove the pending file update
    pendingFiles.remove(pending);

    // do copy
    this->copy(pending);
  }
}

/**
 * @brief Construct a new Controller object
 */
Controller::Controller(QObject *parent) : QObject(parent) {
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
    &watcher, &common::Watch::fileRename,
    this, &Controller::handleFileRename
  );

  connect(
    &watcher, &common::Watch::pathsChanged,
    this, &Controller::pathsChanged
  );

  // connect the timer
  connect(
    &timer, &QTimer::timeout,
    this, &Controller::processPendingFileUpdate
  );

  // start the timer
  timer.start(threshold / 2);
}

/**
 * @brief get the destination root
 */
QString Controller::getDestinationRoot() const {
  return destinationRoot.path();
}

/**
 * @brief set the destination root
 */
void Controller::setDestinationRoot(const QString &path) {
  destinationRoot = QDir(path);
}

/**
 * @brief Get threshold
 */
long long Controller::getThreshold() const {
  return threshold;
}

/**
 * @brief Set threshold
 */
void Controller::setThreshold(long long threshold) {
  threshold = threshold;
}

/**
 * @brief Add a path to watch
 *
 * @param path
 */
void Controller::addWatchPath(const QString &path, bool recursive) {
  watcher.addPath(path, recursive);
}

/**
 * @brief Get the Paths object
 */
QStringList Controller::watchPaths() const {
  return watcher.paths();
}

/**
 * @brief Remove a path from watch
 */
void Controller::removeWatchPath(const QString &path) {
  watcher.removePath(path);
}
}  // namespace srilakshmikanthanp::pulldog
