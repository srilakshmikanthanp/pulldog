// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "copier.hpp"

namespace srilakshmikanthanp::pulldog::common {
/**
 * @brief Construct a new Copier object
 *
 * @param src
 * @param dest
 * @param parent
 */
Copier::Copier(QObject *parent) : ICopier(parent) {}

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
  Copier *copier = reinterpret_cast<Copier *>(lpData);
  auto from = utility::getFileNameFromHandle(hSourceFile);
  auto to = utility::getFileNameFromHandle(hDestinationFile);
  auto transfer = models::Transfer(from, to);
  auto progress = static_cast<double>(totalBytesTransferred.QuadPart) / totalFileSize.QuadPart;
  copier->emit onCopy(transfer, progress);

  // if the copy is completed
  if (totalBytesTransferred.QuadPart == totalFileSize.QuadPart) {
    copier->emit onCopyEnd(transfer);
  }

  // return 0 to continue the copy
  return 0;
}

/**
 * @brief start copying
 */
void Copier::copy(QString source, QString destination) {
  models::Transfer transfer(source, destination);

  // emit the started signal
  emit onCopyStart(transfer);

  // copy file
  if (!CopyFileEx(
    reinterpret_cast<LPCWSTR>(source.utf16()),
    reinterpret_cast<LPCWSTR>(destination.utf16()),
    copyFileCallBack,
    this,
    FALSE,
    COPY_FILE_RESTARTABLE | COPY_FILE_NO_BUFFERING
  )) {
    auto error = GetLastError();
    emit onError("Failed to copy file " + source + " to " + destination + " Error: " + QString::number(error));
    return;
  }
}
} // namespace srilakshmikanthanp::pulldog::common
