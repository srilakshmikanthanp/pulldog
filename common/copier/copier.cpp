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
Copier::Copier(QObject *parent): QObject(parent) {}

/**
 * @brief start copying
 */
void Copier::copy(QString source, QString destination) {
  // try to lock the destination file
  Locker destLock(destination, types::LockMode::WRITE);

  // if failed to lock
  int fd = destLock.lock();

  if (fd < 0) {
    emit onError("Failed to lock destination file " + destination);
    return;
  }

  // open the destination file
  QFile dest;
  if (!dest.open(fd, QIODevice::WriteOnly)) {
    emit onError("Failed to open destination file " + destination);
    return;
  }

  // try to lock the source file
  Locker srcLock(source, types::LockMode::READ);

  // if failed to lock
  fd = srcLock.lock();

  if (fd < 0) {
    emit onError("Failed to lock source file " + source);
    return;
  }

  // open the source file
  QFile src;
  if (!src.open(fd, QIODevice::ReadOnly)) {
    emit onError("Failed to open source file " + source);
    return;
  }

  // buffer size
  const int bufferSize = 1024 * 1024;

  // get the size of the file
  qint64 size = src.size();
  qint64 read = 0;
  double progress = 0;

  // Transfer Object
  models::Transfer transfer(source, destination);

  // emit the started signal
  emit onCopyStart(transfer);

  // read and write
  while (read < size) {
    auto data = src.read(bufferSize);
    read      += data.size();
    progress  = (read * 100) / size;
    dest.write(data);
    emit onCopy(transfer, progress);
  }

  // emit the finished signal
  emit onCopyEnd(transfer);
}
}  // namespace srilakshmikanthanp::pulldog::common
