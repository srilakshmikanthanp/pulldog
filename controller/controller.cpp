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
  auto destFile = QDir(destinationRoot).filePath(path);
  auto srcFile = QDir(dir).filePath(path);

  // Create a key for the pending file update
  auto transfer = models::Transfer(srcFile, destFile);

  // add it to events
  QMetaObject::invokeMethod(
    &worker, [=] { worker.handleFileUpdate(transfer); }
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
 * @brief Handle the copy start
 */
void Controller::handleCopyStart(const models::Transfer &transfer) {
  QMutexLocker locker(&eventMutex);
  this->events.enqueue([=] { emit copyStart(transfer); });
}

/**
 * @brief Handle the copy
 */
void Controller::handleCopy(const models::Transfer &transfer, double progress) {
  QMutexLocker locker(&eventMutex);
  this->events.enqueue([=] { emit copy(transfer, progress); });
}

/**
 * @brief Handle the copy end
 */
void Controller::handleCopyEnd(const models::Transfer &transfer) {
  QMutexLocker locker(&eventMutex);
  this->events.enqueue([=] { emit copyEnd(transfer); });
}

/**
 * @brief Handle the copy canceled
 */
void Controller::handleCopyCanceled(const models::Transfer &transfer) {
  QMutexLocker locker(&eventMutex);
  this->events.enqueue([=] { emit copyCanceled(transfer); });
}

/**
 * @brief Handle the copy failed
 */
void Controller::handleCopyFailed(const models::Transfer &transfer, int error) {
  QMutexLocker locker(&eventMutex);
  this->events.enqueue([=] { emit copyFailed(transfer, error); });
}

/**
 * @brief Process the events
 */
void Controller::processEvents() {
  for (int i = 0; i < parallelEvents && !events.isEmpty(); i++) {
    events.dequeue()();
  }
}

/**
 * @brief Construct a new Controller object
 */
Controller::Controller(QObject *parent) : QObject(parent) {
  // connect the signals for worker this all are large
  // number of events so we need to store it in queue
  // and process it in interval
  connect(
    &worker, &common::Worker::onCopyStart,
    this, &Controller::onCopyStart,
    Qt::QueuedConnection
  );

  connect(
    &worker, &common::Worker::onCopy,
    this, &Controller::onCopy,
    Qt::QueuedConnection
  );

  connect(
    &worker, &common::Worker::onCopyEnd,
    this, &Controller::onCopyEnd,
    Qt::QueuedConnection
  );

  connect(
    &worker, &common::Worker::onCopyCanceled,
    this, &Controller::onCopyCanceled,
    Qt::QueuedConnection
  );

  connect(
    &worker, &common::Worker::onCopyFailed,
    this, &Controller::onCopyFailed,
    Qt::QueuedConnection
  );

  connect(
    &worker, &common::Worker::onError,
    this, &Controller::onError
  );

  // move the worker to the thread
  worker.moveToThread(&workerThread);

  // start the worker thread
  workerThread.start();

  // connect the signals for watcher
  connect(
    &watcher, &common::Watch::fileCreated,
    this, &Controller::handleFileUpdate,
    Qt::DirectConnection
  );

  connect(
    &watcher, &common::Watch::fileUpdated,
    this, &Controller::handleFileUpdate,
    Qt::DirectConnection
  );

  connect(
    &watcher, &common::Watch::fileRename,
    this, &Controller::handleFileRename,
    Qt::DirectConnection
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

  connect(
    &eventProcessor, &QTimer::timeout,
    this, &Controller::processEvents
  );

  this->eventProcessor.start(interval);
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
long long Controller::getWorkerThreshold() const {
  return worker.getThreshold();
}

/**
 * @brief Set threshold
 */
void Controller::setWorkerThreshold(long long threshold) {
  worker.setThreshold(threshold);
}

/**
 * @brief set the parallel events
 */
void Controller::setParallelEvents(int events) {
  parallelEvents = events;
}

/**
 * @brief get the parallel events
 */
int Controller::getParallelEvents() const {
  return parallelEvents;
}

/**
 * @brief set the interval
 */
void Controller::setProcessInterval(int interval) {
  trigger.setInterval(this->interval = interval);
}

/**
 * @brief get the interval
 */
int Controller::getProcessInterval() const {
  return interval;
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
void Controller::retry(const models::Transfer &transfer) {
  QMetaObject::invokeMethod(
    &worker, [=] { this->worker.retry(transfer); }
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
