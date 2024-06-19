#pragma once  // #include only once see https://en.wikipedia.org/wiki/Pragma_once

// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QObject>
#include <QFile>
#include <QLockFile>

#include "common/locker/locker.hpp"
#include "models/transfer/transfer.hpp"

namespace srilakshmikanthanp::pulldog::common {
/**
 * @brief A Class that copies a file from one location to another
 * with a progress signal
 */
class Copier : public QObject {
 private:

  Q_DISABLE_COPY(Copier)

 private:  // Just for qt

  Q_OBJECT

 signals:
  void onCopyStart(const models::Transfer&);

 signals:
  void onCopy(const models::Transfer&, double progress);

 signals:
  void onCopyEnd(const models::Transfer&);

 signals:
  void onError(const QString&);

 public:

  /**
   * @brief Construct a new Copier object
   */
  Copier(QObject *parent = nullptr);

  /**
   * @brief Destroy the Copier object
   */
  ~Copier() = default;

  /**
   * @brief Copy the file
   */
  void copy(QString src, QString dest);
};
}  // namespace srilakshmikanthanp::pulldog::common::copier
