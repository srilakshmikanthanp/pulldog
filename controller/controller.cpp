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

  // copy the file
  emit copy(srcFile, destFile);
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

  // start the thread
  thread.start();
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

/**
 * @brief Instance of the controller
 */
Controller& Controller::instance(const QString &destRoot, QObject *parent) {
  static Controller controller(destRoot, parent);
  return controller;
}
}  // namespace srilakshmikanthanp::pulldog
