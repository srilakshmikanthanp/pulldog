// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "locker.hpp"

namespace srilakshmikanthanp::pulldog::common {
/**
 * @brief Construct a new Locker object
 *
 * @param file
 * @param parent
 */
Locker::Locker(const QString file, types::LockMode mode, QObject *parent) : QObject(parent), locker(file, mode) {
  // Do nothing
}

/**
 * @brief Destroy the Locker object
 */
Locker::~Locker() {
  locker.unlock();
}

/**
 * @brief is locked
 */
bool Locker::isLocked() const {
  return locker.isLocked();
}

/**
 * @brief Try to lock a file
 */
int Locker::tryLock() {
  return locker.tryLock();
}

/**
 * @brief Lock a file
 */
int Locker::lock(MSec timeout) {
  return locker.lock(timeout);
}

/**
 * @brief Unlock a file
 */
void Locker::unlock() {
  locker.unlock();
}
}  // namespace srilakshmikanthanp::pulldog::common
