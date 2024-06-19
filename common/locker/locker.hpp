#pragma once  // #include only once see https://en.wikipedia.org/wiki/Pragma_once

// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QObject>
#include <QDir>
#include <chrono>

#ifdef _WIN32
#include "windows/lockerimpl.hpp"
#endif

#include "types/lockmode/lockmode.hpp"

namespace srilakshmikanthanp::pulldog::common {
class Locker : public QObject {
 private:
  LockerImpl locker;

 private:
  Q_DISABLE_COPY(Locker)

 private: // Just for qt
  Q_OBJECT

 private:
  using MSec = std::chrono::milliseconds;

 public:
  /**
   * @brief Construct a new Locker object
   */
  Locker(const QString, types::LockMode mode = types::LockMode::READ, QObject *parent = nullptr);

  /**
   * @brief Destroy the Locker object
   */
  ~Locker();

  /**
   * @brief is locked
   */
  bool isLocked() const;

  /**
   * @brief Try to lock a file
   */
  int tryLock();

  /**
   * @brief Lock a file
   *
   * @param file
   */
  int lock(MSec timeout = MSec::max());

  /**
   * @brief Unlock a file
   *
   * @param file
   */
  void unlock();
};
}  // namespace srilakshmikanthanp::pulldog::common
