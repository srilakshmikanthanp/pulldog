#pragma once  // #include only once see https://en.wikipedia.org/wiki/Pragma_once
#ifdef _WIN32

// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QObject>
#include <QDir>
#include <QDeadlineTimer>
#include <QThread>

#define NOMINMAX
#include <windows.h>
#undef NOMINMAX
#include <corecrt_io.h>
#include <chrono>

#include "types/lockmode/lockmode.hpp"

namespace srilakshmikanthanp::pulldog::common {
class LockerImpl : public QObject {
 private:
  Q_DISABLE_COPY(LockerImpl)

 private:
  using MSec = std::chrono::milliseconds;

 private:
  HANDLE hFile = INVALID_HANDLE_VALUE;
  LockMode mode;
  QString file;

 private:
  int tryLock();

 private: // Just for qt
  Q_OBJECT

 public:
  /**
   * @brief Construct a new Locker object
   */
  LockerImpl(const QString, LockMode mode, QObject *parent = nullptr);

  /**
   * @brief Destroy the Locker object
   */
  ~LockerImpl();

  /**
   * @brief is locked
   */
  bool isLocked() const;

  /**
   * @brief Lock a file
   *
   * @param file
   */
  int lock(MSec timeout = MSec::max());;

  /**
   * @brief Unlock a file
   *
   * @param file
   */
  void unlock();
};
}  // namespace srilakshmikanthanp::pulldog::common
#endif
