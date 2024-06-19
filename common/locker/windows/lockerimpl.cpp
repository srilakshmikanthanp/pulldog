// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "lockerimpl.hpp"

namespace srilakshmikanthanp::pulldog::common {
/**
 * @brief Construct a new Locker object
 *
 * @param file
 * @param parent
 */
LockerImpl::LockerImpl(const QString file, LockMode mode, QObject *parent)
  : QObject(parent), file(file), mode(mode) {
  // Do nothing
}

/**
 * @brief Destroy the Locker object
 */
LockerImpl::~LockerImpl() {
  this->unlock();
}

int LockerImpl::tryLock() {
  auto shareMode = FILE_SHARE_READ;
  auto openMode = OPEN_EXISTING;

  if (mode == LockMode::WRITE) {
    openMode = CREATE_ALWAYS;
    shareMode = 0;
  }

  this->hFile = CreateFile(
    file.toStdWString().c_str(),
    GENERIC_READ | GENERIC_WRITE,
    shareMode,
    NULL,
    openMode,
    FILE_ATTRIBUTE_NORMAL,
    NULL
  );

  if(this->hFile != INVALID_HANDLE_VALUE) {
    return _open_osfhandle((intptr_t)hFile, 0);
  }

  switch (auto lastError = GetLastError()) {
    case ERROR_SHARING_VIOLATION:
    case ERROR_ALREADY_EXISTS:
    case ERROR_FILE_EXISTS:
      return -2;
    case ERROR_ACCESS_DENIED:
      return -1;
    default:
      return -1;
  }
}

/**
 * @brief Lock a file
 */
int LockerImpl::lock(MSec timeout) {
  // if file not exists, return error
  if (!QFile::exists(file) && mode == LockMode::READ) {
    return -1;
  }

  // remaining time
  const auto sleepTime = MSec(100).count();
  QDeadlineTimer timer(timeout);

  while(!timer.hasExpired()) {
    auto result = this->tryLock();

    if(result == -1 or result != -2) {
      return result;
    }

    QThread::msleep(sleepTime);
  }

  return -1;
}

/**
 * @brief is locked
 */
bool LockerImpl::isLocked() const {
  return hFile != INVALID_HANDLE_VALUE;
}

/**
 * @brief Unlock a file
 */
void LockerImpl::unlock() {
  if(this->isLocked()) {
    CloseHandle(hFile);
    hFile = INVALID_HANDLE_VALUE;
  }
}
}  // namespace srilakshmikanthanp::pulldog::common
