// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "copier.hpp"

namespace srilakshmikanthanp::pulldog::common {
/**
 * @brief Copy file call back from CopyFileEx
 */
DWORD CALLBACK Copier::copyFileCallBack(
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
  auto progress = (static_cast<double>(totalBytesTransferred.QuadPart) / totalFileSize.QuadPart) * 100;
  auto copier = reinterpret_cast<Copier *>(lpData);
  QMutexLocker locker(&copier->mutex);

  // check if handle is valid since the network drive may disconnected
  BY_HANDLE_FILE_INFORMATION info;

  // get the file information
  if (
    !GetFileInformationByHandle(hDestinationFile, &info) ||
    !GetFileInformationByHandle(hSourceFile, &info)
  ) {
    return PROGRESS_STOP;
  }

  // if cancel flag is set return 1 to cancel the copy
  if (copier->cancelFlag) {
    return PROGRESS_CANCEL;
  }

  // emit progress
  copier->emit onCopy(copier->transfer, progress);

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
: ICopier(parent), transfer(transfer) {
  // Do nothing
}

/**
 * @brief start
 */
void Copier::start() {
  // emit the started signal
  emit this->onCopyStart(transfer);

  // copy file blocking
  if (this->jobDone = CopyFileEx(
    reinterpret_cast<LPCWSTR>(transfer.getFrom().utf16()),
    reinterpret_cast<LPCWSTR>(transfer.getTo().utf16()),
    copyFileCallBack,
    this,
    &cancelFlag,
    COPY_FILE_FAIL_IF_EXISTS  |
    COPY_FILE_RESTARTABLE     |
    COPY_FILE_NO_BUFFERING
  )) {
    return emit this->onCopyEnd(transfer);
  }

  // if canceled
  if (cancelFlag) {
    return emit this->onCopyCanceled(transfer);
  }

  // get the error
  auto error = GetLastError();
  auto msg = QString("Failed copy %1 to %2 : %3").arg(transfer.getFrom(), transfer.getTo(), QString::number(error));
  emit this->onError(msg);

  // emit cancel signal
  emit this->onCopyFailed(transfer);
}

/**
 * @brief Cancel the copy
 */
void Copier::cancel() {
  QMutexLocker locker(&mutex);
  if(!jobDone) {
    this->cancelFlag = true;
  }
}

/**
 * @brief Is Cancelled
 */
bool Copier::isCancelled() const {
  return cancelFlag;
}
} // namespace srilakshmikanthanp::pulldog::common
