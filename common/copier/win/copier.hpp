#pragma once  // #include only once see https://en.wikipedia.org/wiki/Pragma_once

// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QFile>
#include <QLockFile>
#include <QObject>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>

#define NOMINMAX
#include <windows.h>
#undef NOMINMAX

#include <atomic>

#include "common/copier/icopier.hpp"
#include "common/locker/locker.hpp"
#include "models/transfer/transfer.hpp"
#include "utility/functions/functions.hpp"

namespace srilakshmikanthanp::pulldog::common {
/**
 * @brief A Class that copies a file from one location to another
 * with a progress signal
 */
class Copier : public ICopier {
 private:

  Q_DISABLE_COPY(Copier)

 private:  // Just for qt

  Q_OBJECT

 private:
  std::atomic<bool> jobDone = false;
  const models::Transfer transfer;
  BOOL cancelFlag = false;
  QMutex mutex;

 public:

  /**
   * @brief Construct a new Copier object
   */
  Copier(models::Transfer transfer, QObject *parent = nullptr);

  /**
   * @brief Destroy the Copier object
   */
  virtual ~Copier() = default;

  /**
   * @brief start
   */
  void start() override;

  /**
   * @brief Cancel the copy
   */
  void cancel() override;

  /**
   * @brief is Cancelled
   */
  bool isCancelled() const;

 private:
  /**
   * @brief Copy file call back from CopyFileEx
   */
  static DWORD CALLBACK copyFileCallBack(
    LARGE_INTEGER totalFileSize,
    LARGE_INTEGER totalBytesTransferred,
    LARGE_INTEGER streamSize,
    LARGE_INTEGER streamBytesTransferred,
    DWORD dwStreamNumber,
    DWORD dwCallbackReason,
    HANDLE hSourceFile,
    HANDLE hDestinationFile,
    LPVOID lpData
  );
};
}  // namespace srilakshmikanthanp::pulldog::common::copier
