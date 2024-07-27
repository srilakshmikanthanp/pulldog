// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "worker.hpp"

namespace srilakshmikanthanp::pulldog::common {
/**
 * @brief start to Copy the file with copier object
 */
void Worker::checkAndCopy(const models::Transfer &transfer) {
  if(!copingFiles.contains(transfer)) {
    this->copy(transfer);
    return;
  }

  auto copier = copingFiles[transfer];

  // on cancel or end start copy
  connect(
    copier, &common::Copier::onCopyCanceled,
    [=] { this->copy(transfer); }
  );

  connect(
    copier, &common::Copier::onCopyEnd,
    [=] { this->copy(transfer); }
  );

  connect(
    copier, &common::Copier::onCopyFailed,
    [=] { this->copy(transfer); }
  );

  // cancel the copy
  copier->cancel();
}

/**
 * @brief slot to handle file rename
 */
void Worker::copy(const models::Transfer &transfer) {
  auto srcFile = transfer.getFrom();
  auto destFile = transfer.getTo();

  // create all parent directories
  if(!QDir().mkpath(QFileInfo(destFile).dir().path())) {
    emit onCopyFailed(transfer);
    return;
  }

  // create a copier object
  auto copier = new common::Copier(transfer);

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
    copier, &common::Copier::onCopyCanceled,
    this, &Worker::onCopyCanceled
  );

  connect(
    copier, &common::Copier::onCopyFailed,
    this, &Worker::onCopyFailed
  );

  connect(
    copier, &common::Copier::onError,
    this, &Worker::onError
  );

  const auto cleaner = [this, transfer, copier]() {
    copingFiles.remove(transfer);
    copier->deleteLater();
  };

  // to remove the copier from the map
  connect(copier, &common::Copier::onCopyCanceled, cleaner);
  connect(copier, &common::Copier::onCopyEnd, cleaner);
  connect(copier, &common::Copier::onCopyFailed, cleaner);

  // add the copier to the coping files
  copingFiles[transfer] = copier;

  // start the copier on thread pool
  QThreadPool::globalInstance()->start([copier]() {
    copier->start();
  });
}

/**
 * @brief Process the pending file update and return the status
 */
Worker::ProcessStatus Worker::process(const models::Transfer &pending) {
  // extract the source and destination file
  auto srcFile = pending.getFrom();
  auto srcInfo = QFileInfo(srcFile);

  // ignore if it is not exists or it is a directory
  if(srcInfo.isDir()) {
    return ProcessStatus::Directory;
  }

  // create an locker object
  common::Locker locker(
    srcFile, types::LockMode::EXCLUSIVE, types::LockType::READ
  );

  // lock
  auto status = locker.tryLock();

  // if it is non recoverable
  if(status == common::ILocker::Error::UNRECOVERABLE) {
    return ProcessStatus::Error;
  }

  // if it is recoverable
  if(status == common::ILocker::Error::RECOVERABLE) {
    return ProcessStatus::Retry;
  }

  // unlock the file
  locker.unlock();

  // do copy
  this->checkAndCopy(pending);

  // return success
  return ProcessStatus::Success;
}

/**
 * @brief Process the pending file update
 */
void Worker::processPendingFileUpdate() {
  // failed files but we can retry again
  QQueue<models::Transfer> failed;

  // lock the mutex for pending files
  QMutexLocker locker(&mutex);

  // process files
  for (auto pending: pendingFiles) {
    switch (this->process(pending)) {
    case ProcessStatus::Error:
      emit onCopyFailed(pending);
      break;
    case ProcessStatus::Retry:
      failed.enqueue(pending);
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
 * @brief Retry a transfer
 */
void Worker::retryTransfer(const models::Transfer &transfer) {
  this->checkAndCopy(transfer);
}

/**
 * @brief slot to handle file update
 */
void Worker::handleFileUpdate(models::Transfer transfer) {
  QMutexLocker locker(&mutex);
  pendingFiles.enqueue(transfer);
}
} // namespace srilakshmikanthanp::pulldog::common
