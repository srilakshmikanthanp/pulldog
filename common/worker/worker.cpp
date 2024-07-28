// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "worker.hpp"

namespace srilakshmikanthanp::pulldog::common {
/**
 * @brief start to Copy the file with copier object
 */
Worker::CopyStatus Worker::copy(const models::Transfer &transfer) {
  // create all parent directories
  if(!QDir().mkpath(QFileInfo(transfer.getTo()).dir().path())) {
    emit onCopyFailed(transfer);
    return CopyStatus::Error;
  }

  // class of copier that inherits QRunnable
  struct CopierRunnable : common::Copier, QRunnable {
    void run() override { start(); }
    using Copier::Copier;
  };

  // cleaner for the copier
  const auto remover = [this, transfer]() {
    QMutexLocker locker(&copingMutex);
    copingFiles.remove(transfer);
  };

  // lock the mutex
  QMutexLocker locker(&copingMutex);

  // if already coping
  if(copingFiles.contains(transfer)) {
    return CopyStatus::Retry;
  }

  // create a copier object
  auto copier = new CopierRunnable(transfer);

  // connect the signals
  connect(
    copier, &common::Copier::onCopyCanceled,
    this, &Worker::onCopyCanceled
  );

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
    copier, &common::Copier::onCopyFailed,
    this, &Worker::onCopyFailed
  );

  connect(
    copier, &common::Copier::onError,
    this, &Worker::onError
  );

  // to remove the copier from the map
  connect(copier, &common::Copier::onCopyCanceled, remover);
  connect(copier, &common::Copier::onCopyEnd, remover);
  connect(copier, &common::Copier::onCopyFailed, remover);

  // add the copier to the coping files
  copingFiles[transfer] = copier;

  // start the copy
  QThreadPool::globalInstance()->start(copier);

  // return success
  return CopyStatus::Success;
}

/**
 * @brief Process the pending file update and return the status
 */
Worker::CopyStatus Worker::process(const models::Transfer &pending) {
  // extract the source and destination file
  auto srcFile = pending.getFrom();
  auto srcInfo = QFileInfo(srcFile);

  // ignore if it is not exists or it is a directory
  if(srcInfo.isDir()) {
    return CopyStatus::Directory;
  }

  // using alias
  using Error = common::ILocker::Error;
  using Mode  = types::LockMode;
  using Type  = types::LockType;

  // create an locker object
  common::Locker locker(
    srcFile, Mode::EXCLUSIVE, Type::READ
  );

  // lock
  auto status = locker.tryLock();

  // if it is non recoverable
  if(status == Error::UNRECOVERABLE) {
    return CopyStatus::Error;
  }

  // if it is recoverable
  if(status == Error::RECOVERABLE) {
    return CopyStatus::Retry;
  }

  // unlock the file
  locker.unlock();

  // do copy
  return this->copy(pending);
}

/**
 * @brief Process the pending file update
 */
void Worker::processPendingFileUpdate() {
  // lock the mutex for pending files
  QMutexLocker locker(&pendingMutex);
  QQueue<models::Transfer> failed;

  for (auto pending: pendingFiles) {
    switch (this->process(pending)) {
    case CopyStatus::Retry:
      failed.enqueue(pending);
      break;
    case CopyStatus::Error:
      emit onCopyFailed(pending);
      break;
    }
  }

  // swap the failed files
  pendingFiles.swap(failed);
}

Worker::Worker(QObject *parent) : QObject(parent) {
  // connect the timer
  connect(
    &timer, &QTimer::timeout, this, &Worker::processPendingFileUpdate
  );

  // start the timer
  timer.start(threshold);
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
  timer.setInterval(this->threshold = threshold);
}

/**
 * @brief Retry a transfer
 */
void Worker::retry(const models::Transfer &transfer) {
  this->copy(transfer);
}

/**
 * @brief slot to handle file update
 */
void Worker::handleFileUpdate(models::Transfer transfer) {
  QMutexLocker locker(&pendingMutex);
  pendingFiles.enqueue(transfer);
  locker.unlock();
}
} // namespace srilakshmikanthanp::pulldog::common
