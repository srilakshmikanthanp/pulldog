#pragma once  // #include only once see https://en.wikipedia.org/wiki/Pragma_once

// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QObject>
#include <QDir>
#include <chrono>

namespace srilakshmikanthanp::pulldog::common {
class ILocker : public QObject {
 private:
  Q_DISABLE_COPY(ILocker)

 private: // Just for qt
  Q_OBJECT

 private:
  using MSec = std::chrono::milliseconds;

 public:
  /**
   * @brief Lock mode
   */
  enum class LockMode {
    SHARE, EXCLUSIVE
  };

  /**
   * @brief Lock type
   */
  enum class LockType {
    READ, WRITE
  };

  enum Error {
    RECOVERABLE = -1,
    UNRECOVERABLE = -2
  };

 public:
  /**
   * @brief Construct a new ILocker object
   */
  ILocker(QObject *parent = nullptr);

  /**
   * @brief Destroy the ILocker object
   */
  virtual ~ILocker() = default;

  /**
   * @brief is locked
   */
  virtual bool isLocked() const = 0;

  /**
   * @brief Try to lock a file
   */
  virtual int tryLock() = 0;

  /**
   * @brief Lock a file
   *
   * @param file
   */
  virtual int lock(MSec timeout = MSec::max()) = 0;

  /**
   * @brief Unlock a file
   *
   * @param file
   */
  virtual void unlock() = 0;
};
}  // namespace srilakshmikanthanp::pulldog::common
