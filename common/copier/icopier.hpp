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
class ICopier : public QObject {
 private:

  Q_DISABLE_COPY(ICopier)

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
   * @brief Construct a new ICopier object
   */
  ICopier(QObject *parent = nullptr);

  /**
   * @brief Destroy the ICopier object
   */
  ~ICopier() = default;

  /**
   * @brief Copy the file
   */
  virtual void copy(QString src, QString dest) = 0;
};
}  // namespace srilakshmikanthanp::pulldog::common::copier
