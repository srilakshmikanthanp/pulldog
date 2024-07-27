// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "controller.hpp"

namespace srilakshmikanthanp::pulldog {
/**
 * @brief slot to handle file update
 */
void Controller::handleFileUpdate(const QString dir, const QString path) {
  // get the destination file path from the destination root
  qDebug() << "File Update: " << destinationRoot << " " << path;
  auto fileName = QFileInfo(path).fileName();
  auto destFile = QDir(destinationRoot).filePath(fileName);
  auto srcFile = QDir(dir).filePath(path);

  // Create a key for the pending file update
  auto key = models::Transfer(srcFile, destFile);

  // call the file processor on that thread
  QMetaObject::invokeMethod(
    &worker, [=] { this->worker.handleFileUpdate(key); }
  );
}

/**
 * @brief Handle the file rename
 */
void Controller::handleFileRename(
  const QString directory,
  const QString oldFile,
  const QString newFile
) {
  this->handleFileUpdate(directory, newFile);
}

/**
 * @brief Construct a new Controller object
 */
Controller::Controller(QObject *parent) : QObject(parent) {
  connect(
    &worker, &common::Worker::onCopyStart,
    this, &Controller::onCopyStart
  );

  connect(
    &worker, &common::Worker::onCopy,
    this, &Controller::onCopy
  );

  connect(
    &worker, &common::Worker::onCopyEnd,
    this, &Controller::onCopyEnd
  );

  connect(
    &worker, &common::Worker::onCopyCanceled,
    this, &Controller::onCopyCanceled
  );

  connect(
    &worker, &common::Worker::onCopyFailed,
    this, &Controller::onCopyFailed
  );

  connect(
    &worker, &common::Worker::onError,
    this, &Controller::onError
  );

  // move the worker to the thread
  worker.moveToThread(&workerThread);

  // start the worker thread
  workerThread.start();

  // connect the signals need to do it on watcher thread
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
    &watcher, &common::Watch::pathAdded,
    this, &Controller::pathAdded
  );

  connect(
    &watcher, &common::Watch::pathRemoved,
    this, &Controller::pathRemoved
  );

  connect(
    &watcher, &common::Watch::onError,
    this, &Controller::onError
  );

  // move watcher
  watcher.moveToThread(&watcherThread);

  // start
  watcherThread.start();
}

/**
 * @brief Destroy the Controller object
 */
Controller::~Controller() {
  for(auto thread: {&watcherThread, &workerThread}) {
    thread->quit();
    thread->wait();
  }
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
  return worker.getThreshold();
}

/**
 * @brief Set threshold
 */
void Controller::setThreshold(long long threshold) {
  worker.setThreshold(threshold);
}

/**
 * @brief Add a path to watch
 *
 * @param path
 */
void Controller::addWatchPath(const QString &path, bool recursive) {
  QMetaObject::invokeMethod(
    &watcher, [=] { this->watcher.addPath(path, recursive); }
  );
}

/**
 * @brief Get the Paths object
 */
QStringList Controller::watchPaths() const {
  return watcher.paths();
}

/**
 * @brief Retry a transfer
 */
void Controller::retryTransfer(const models::Transfer &transfer) {
  QMetaObject::invokeMethod(
    &worker, [=] { this->worker.retryTransfer(transfer); }
  );
}

/**
 * @brief Remove a path from watch
 */
void Controller::removeWatchPath(const QString &path) {
  QMetaObject::invokeMethod(
    &watcher, [=] { this->watcher.removePath(path); }
  );
}
}  // namespace srilakshmikanthanp::pulldog
