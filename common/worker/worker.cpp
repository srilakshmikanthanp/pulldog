// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "worker.hpp"

namespace srilakshmikanthanp::pulldog::common {
/**
 * @brief start to Copy the file with copier object
 */
void Worker::copy(const models::Transfer &transfer) {
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
    this, &Worker::onCopyStart
  );

  connect(
    copier, &common::Copier::onCopy,
    this, &Worker::onCopy
  );

  connect(
    copier, &common::Copier::onCopyEnd,
    this, &Worker::onCopyEnd
  );

  connect(
    copier, &common::Copier::onCopyCancel,
    this, &Worker::onCopyCancel
  );

  connect(
    copier, &common::Copier::onError,
    this, &Worker::onError
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

  // start the copier on thread pool
  QThreadPool::globalInstance()->start([copier]() {
    copier->start();
  });
}

/**
 * @brief Process the pending file update
 */
void Worker::process(const models::Transfer &pending) {
  // extract the source and destination file
  auto srcFile = pending.getFrom();

  // ignore if it is not exists or it is a directory
  if(!QFileInfo(srcFile).exists() || QFileInfo(srcFile).isDir()) {
    pendingFiles.remove(pending);
    return;
  }

  // create an locker object
  common::Locker locker(srcFile, types::LockMode::READ);

  // lock
  auto status = locker.tryLock();

  // if it is recoverable, continue
  if(status == common::ILocker::Error::UNRECOVERABLE) {
    emit onError("Failed to lock the file " + srcFile);
    pendingFiles.remove(pending);
    return;
  }

  // if it is recoverable, continue
  if(status == common::ILocker::Error::RECOVERABLE) {
    return;
  }

  // unlock the file
  locker.unlock();

  // remove the pending file update
  pendingFiles.remove(pending);

  // do copy
  this->copy(pending);
}

/**
 * @brief Process the pending file update
 */
void Worker::processPendingFileUpdate() {
  QMutexLocker locker(&mutex); // lock the mutex
  auto keys = pendingFiles.keys();
  for(auto pending: keys) {
    process(pending);
  }
}

Worker::Worker(QObject *parent) : QObject(parent) {
  // connect the timer
  connect(
    &timer, &QTimer::timeout, this, &Worker::processPendingFileUpdate
  );

  // start the timer
  timer.start(threshold / 2);
}

/**
 * @brief Get threshold
 */
long long Worker::getThreshold() const {
  return threshold;
}

/**
 * @brief Set threshold
 */
void Worker::setThreshold(long long threshold) {
  timer.setInterval((this->threshold = threshold) / 2);
}

/**
 * @brief slot to handle file update
 */
void Worker::handleFileUpdate(models::Transfer transfer) {
  QMutexLocker locker(&mutex);
  auto time = QDateTime::currentMSecsSinceEpoch();
  pendingFiles[transfer] = time;
}
} // namespace srilakshmikanthanp::pulldog::common
