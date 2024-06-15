// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "controller.hpp"

namespace srilakshmikanthanp::pulldog {
void Controller::handleFileUpdate(const QString dir, const QString path) {
  // get the destination file path from the destination root
  auto destFile = destinationRoot.filePath(path);

  // create all parent directories
  if(!QDir().mkpath(QFileInfo(destFile).dir().path())) {
    qWarning() << "Failed to create parent directories for " << destFile;
    return;
  }

  // create a file object
  auto file = QFile(destFile);

  // delete the file if it exists
  if(file.exists() && !file.remove()) {
    qWarning() << "Failed to remove " << destFile;
    return;
  }

  // create a Thread and copier object
  auto copier = new common::Copier(path, destFile);
  auto thread = new QThread();

  // move the copier object to the thread
  copier->moveToThread(thread);

  // connect the signals
  connect(
    thread, &QThread::started,
    copier, &common::Copier::start
  );

  connect(
    thread, &QThread::finished,
    thread, &QThread::deleteLater
  );

  // create a copier object and emit the signal
  emit onFileCopy(copier);
}

/**
 * @brief Construct a new Controller object
 */
Controller::Controller(QString destRoot, QObject *parent) : QObject(parent) {
  // set the destination root directory
  this->destinationRoot = QDir(destRoot);

  // connect the signals
  connect(
    &watcher, &common::Watcher::fileCreated,
    this, &handleFileCreatedOrUpdated
  );
}

/**
 * @brief Add a path to watch
 *
 * @param path
 */
bool Controller::addPath(const QString &path, bool recursive = true) {
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
