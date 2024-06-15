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
Copier::Copier(QString src, QString dest, QObject *parent)
    : QObject(parent), source(src), destination(dest) {}

/**
 * @brief get the progress
 */
double Copier::getProgress() const {
  return progress;
}

/**
 * @brief get the source
 */
QString Copier::getSource() const {
  return source;
}

/**
 * @brief get the destination
 */
QString Copier::getDestination() const {
  return destination;
}

/**
 * @brief start copying
 */
void Copier::start() {
  // open the destination file
  QFile dest(destination);
  if (!dest.open(QIODevice::WriteOnly)) {
    return;
  }

  // open the source file
  QFile src(source);
  if (!src.open(QIODevice::ReadOnly)) {
    return;
  }

  // get the size of the file
  qint64 size = src.size();
  qint64 read = 0;

  // emit the started signal
  emit started(progress);

  // read and write
  while (read < size) {
    auto data = src.read(bufferSize);
    read      += data.size();
    progress  = (read * 100) / size;
    dest.write(data);
    emit onChange(progress);
  }

  // emit the finished signal
  emit finished(progress);
}
}  // namespace srilakshmikanthanp::pulldog::common
