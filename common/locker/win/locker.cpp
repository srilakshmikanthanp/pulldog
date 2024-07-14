// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "locker.hpp"

namespace srilakshmikanthanp::pulldog::common {
/**
 * @brief Construct a new ILocker object
 *
 * @param file
 * @param parent
 */
Locker::Locker(const QString file, types::LockMode mode, types::LockType type, QObject *parent)
  : ILocker(parent), file(file), mode(mode), type(type) {
  // Do nothing
}

/**
 * @brief Destroy the ILocker object
 */
Locker::~Locker() {
  this->unlock();
}

int Locker::tryLock() {
  auto shareMode = FILE_SHARE_READ;
  auto openMode = OPEN_EXISTING;

  if (mode == types::LockMode::EXCLUSIVE) {
    shareMode = 0;
  }

  if (type == types::LockType::WRITE) {
    openMode = CREATE_ALWAYS;
  }

  this->hFile = CreateFile(
    file.toStdWString().c_str(),
    GENERIC_READ | GENERIC_WRITE,
    shareMode,
    NULL,
    openMode,
    FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS,
    NULL
  );

  if(this->hFile != INVALID_HANDLE_VALUE) {
    return _open_osfhandle((intptr_t)hFile, 0);
  }

  switch (auto lastError = GetLastError()) {
    case ERROR_SHARING_VIOLATION:
    case ERROR_ALREADY_EXISTS:
    case ERROR_FILE_EXISTS:
      return Error::RECOVERABLE;
    case ERROR_ACCESS_DENIED:
    default:
      return Error::UNRECOVERABLE;
  }
}

/**
 * @brief Lock a file
 */
int Locker::lock(MSec timeout) {
  // if file not exists, return error
  if (!QFile::exists(file) && mode == types::LockMode::SHARE) {
    return Error::UNRECOVERABLE;
  }

  // remaining time
  const auto sleepTime = MSec(100).count();
  QDeadlineTimer timer(timeout);

  while(!timer.hasExpired()) {
    auto result = this->tryLock();

    if(result != Error::RECOVERABLE) {
      return result;
    }

    QThread::msleep(sleepTime);
  }

  return Error::UNRECOVERABLE;
}

/**
 * @brief is locked
 */
bool Locker::isLocked() const {
  return hFile != INVALID_HANDLE_VALUE;
}

/**
 * @brief Unlock a file
 */
void Locker::unlock() {
  if(this->isLocked()) {
    CloseHandle(hFile);
    hFile = INVALID_HANDLE_VALUE;
  }
}
}  // namespace srilakshmikanthanp::pulldog::common
