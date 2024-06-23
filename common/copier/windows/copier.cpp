// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "copier.hpp"

namespace srilakshmikanthanp::pulldog::common {
/**
 * @brief Construct a new CopierPrivate object
 */
CopierPrivate::CopierPrivate(models::Transfer transfer, common::Copier *copier)
: transfer(transfer),
  copier(copier) {
  // Do nothing
}

/**
 * Start
 */
void CopierPrivate::start() {
  // emit the started signal
  emit copier->onCopyStart(transfer);

  // copy file blocking
  if (CopyFileEx(
    reinterpret_cast<LPCWSTR>(transfer.getFrom().utf16()),
    reinterpret_cast<LPCWSTR>(transfer.getTo().utf16()),
    copyFileCallBack,
    this,
    &cancelFlag,
    COPY_FILE_RESTARTABLE | COPY_FILE_NO_BUFFERING
  )) {
    emit copier->onCopyEnd(transfer);
    return;
  }

  // get the error
  auto error = GetLastError();
  auto msg = QString("Failed copy %1 to %2 : %3").arg(transfer.getFrom(), transfer.getTo(), QString::number(error));
  emit copier->onError(msg);

  // emit cancel signal
  emit copier->onCopyCancel(transfer);
}

/**
 * @brief Cancel the copy
 */
void CopierPrivate::cancel() {
  QMutexLocker locker(&mutex);

  if(!isCompleted) {
    this->cancelFlag = true;
  }
}

/**
 * @brief Copy file call back from CopyFileEx
 */
DWORD CALLBACK CopierPrivate::copyFileCallBack(
  LARGE_INTEGER totalFileSize,
  LARGE_INTEGER totalBytesTransferred,
  LARGE_INTEGER streamSize,
  LARGE_INTEGER streamBytesTransferred,
  DWORD dwStreamNumber,
  DWORD dwCallbackReason,
  HANDLE hSourceFile,
  HANDLE hDestinationFile,
  LPVOID lpData
) {
  auto copierPrivate = reinterpret_cast<CopierPrivate *>(lpData);
  auto progress = (static_cast<double>(totalBytesTransferred.QuadPart) / totalFileSize.QuadPart) * 100;
  copierPrivate->copier->emit onCopy(copierPrivate->transfer, progress);

  QMutexLocker locker(&copierPrivate->mutex);

  // if cancel flag is set return 1 to cancel the copy
  if (copierPrivate->cancelFlag) {
    return PROGRESS_CANCEL;
  }

  // return 0 to continue the copy
  return PROGRESS_CONTINUE;
}

/**
 * @brief Construct a new Copier object
 *
 * @param src
 * @param dest
 * @param parent
 */
Copier::Copier(models::Transfer transfer, QObject *parent)
: ICopier(parent), privateInstance(transfer, this) {
  privateInstance.moveToThread(&thread);
  thread.start();
}

/**
 * @brief Destroy the Copier object
 */
Copier::~Copier() {
  thread.quit();
  thread.wait();
}

/**
 * @brief start
 */
void Copier::start() {
  QMetaObject::invokeMethod(&privateInstance, &CopierPrivate::start);
}

/**
 * @brief Cancel the copy
 */
void Copier::cancel() {
  privateInstance.cancel();
}
} // namespace srilakshmikanthanp::pulldog::common
