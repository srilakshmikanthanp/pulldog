// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "controller.hpp"

namespace srilakshmikanthanp::pulldog {
/**
 * @brief start to Copy the file with copier object
 */
void FileProcessor::copy(const models::Transfer &transfer) {
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
    this, &FileProcessor::onCopyStart
  );

  connect(
    copier, &common::Copier::onCopy,
    this, &FileProcessor::onCopy
  );

  connect(
    copier, &common::Copier::onCopyEnd,
    this, &FileProcessor::onCopyEnd
  );

  connect(
    copier, &common::Copier::onCopyCancel,
    this, &FileProcessor::onCopyCancel
  );

  connect(
    copier, &common::Copier::onError,
    this, &FileProcessor::onError
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
void FileProcessor::processPendingFileUpdate() {
  // get the current time
  auto currentTime = QDateTime::currentMSecsSinceEpoch();

  // if concurrent copies is full, return
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

    // lock
    auto status = locker.tryLock();

    // if it is recoverable, continue
    if(status == common::ILocker::Error::UNRECOVERABLE) {
      emit onError("Failed to lock the file " + srcFile);
      continue;
    }

    // if it is recoverable, continue
    if(status == common::ILocker::Error::RECOVERABLE) {
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

FileProcessor::FileProcessor(QObject *parent) : QObject(parent) {
  // connect the timer
  connect(
    &timer, &QTimer::timeout, this, &FileProcessor::processPendingFileUpdate
  );

  // start the timer
  timer.start(threshold / 2);
}

/**
 * @brief slot to handle file update
 */
void FileProcessor::handleFileUpdate(models::Transfer transfer) {
  pendingFiles[transfer] = QDateTime::currentMSecsSinceEpoch();
}

/**
 * @brief Get threshold
 */
long long FileProcessor::getThreshold() const {
  return threshold;
}

/**
 * @brief Set threshold
 */
void FileProcessor::setThreshold(long long threshold) {
  timer.setInterval((this->threshold = threshold) / 2);
}

/**
 * @brief Get the Concurrent Copies
 */
int FileProcessor::getConcurrentCopies() const {
  return concurrentCopies;
}

/**
 * @brief Set the Concurrent Copies
 */
void FileProcessor::setConcurrentCopies(int concurrentCopies) {
  this->concurrentCopies = concurrentCopies;
}

// ------------------------------------------------------------------------------------

/**
 * @brief slot to handle file update
 */
void Controller::handleFileUpdate(const QString dir, const QString path) {
  // get the destination file path from the destination root
  auto destFile = destinationRoot.filePath(QFileInfo(dir).fileName() + "/" + path);
  auto srcFile = QDir(dir).filePath(path);

  // ignore if it is not exists
  if(!QFileInfo(srcFile).exists()) {
    return;
  }

  // if it is a directory, return
  if(QFileInfo(srcFile).isDir()) {
    return;
  }

  // Create a key for the pending file update
  auto key = models::Transfer(srcFile, destFile);

  // call the file processor on that thread
  QMetaObject::invokeMethod(
    &fileProcessor, [=] { this->fileProcessor.handleFileUpdate(key); }
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
    &fileProcessor, &FileProcessor::onCopyStart,
    this, &Controller::onCopyStart
  );

  connect(
    &fileProcessor, &FileProcessor::onCopy,
    this, &Controller::onCopy
  );

  connect(
    &fileProcessor, &FileProcessor::onCopyEnd,
    this, &Controller::onCopyEnd
  );

  connect(
    &fileProcessor, &FileProcessor::onCopyCancel,
    this, &Controller::onCopyCancel
  );

  connect(
    &fileProcessor, &FileProcessor::onError,
    this, &Controller::onError
  );

  // move the file processor to the thread
  fileProcessor.moveToThread(&fileProcessorThread);

  // start the file processor thread
  fileProcessorThread.start();

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

  connect(
    &watcher, &common::Watch::onError,
    this, &Controller::onError
  );
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
  return fileProcessor.getThreshold();
}

/**
 * @brief Set threshold
 */
void Controller::setThreshold(long long threshold) {
  fileProcessor.setThreshold(threshold);
}

/**
 * @brief Get the Concurrent Copies
 */
int Controller::getConcurrentCopies() const {
  return fileProcessor.getConcurrentCopies();
}

/**
 * @brief Set the Concurrent Copies
 */
void Controller::setConcurrentCopies(int concurrentCopies) {
  fileProcessor.setConcurrentCopies(concurrentCopies);
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
