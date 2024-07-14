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

#include "common/locker/ilocker.hpp"
#include "types/lockmode/lockmode.hpp"

namespace srilakshmikanthanp::pulldog::common {
class Locker : public ILocker {
 private:
  Q_DISABLE_COPY(Locker)

 private:
  using LockMode = types::LockMode;
  using MSec = std::chrono::milliseconds;
  using LockType = types::LockType;


 private:
  HANDLE hFile = INVALID_HANDLE_VALUE;
  types::LockMode mode;
  types::LockType type;
  QString file;

 private: // Just for qt
  Q_OBJECT

 public:
  /**
   * @brief Construct a new ILocker object
   */
  Locker(
    const QString,
    LockMode mode = types::LockMode::SHARE,
    LockType type = types::LockType::READ,
    QObject *parent = nullptr
  );

  /**
   * @brief Destroy the ILocker object
   */
  ~Locker();

  /**
   * @brief is locked
   */
  bool isLocked() const override;

  /**
   * @brief Try to lock a file
   */
  int tryLock() override;

  /**
   * @brief Lock a file
   *
   * @param file
   */
  int lock(MSec timeout = MSec::max()) override;

  /**
   * @brief Unlock a file
   *
   * @param file
   */
  void unlock() override;
};
}  // namespace srilakshmikanthanp::pulldog::common
#endif
