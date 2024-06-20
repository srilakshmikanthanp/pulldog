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
Locker::Locker(const QString file, types::LockMode mode, QObject *parent)
  : ILocker(parent), file(file), mode(mode) {
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

  if (mode == types::LockMode::WRITE) {
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
int Locker::lock(MSec timeout) {
  // if file not exists, return error
  if (!QFile::exists(file) && mode == types::LockMode::READ) {
    return -1;
  }

  // remaining time
  const auto sleepTime = MSec(100).count();
  QDeadlineTimer timer(timeout);

  while(!timer.hasExpired()) {
    auto result = this->tryLock();

    if(result == -1 || result != -2) {
      return result;
    }

    QThread::msleep(sleepTime);
  }

  return -1;
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
