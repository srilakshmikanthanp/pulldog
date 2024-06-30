#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QObject>
#include <QDir>
#include <QMutex>
#include <QMap>
#include <QThread>
#include <QFileInfo>
#include <QFile>
#include <QList>
#include <QSharedPointer>
#include <QTimer>
#include <QDirIterator>
#include <QMutexLocker>
#include <QThreadPool>

#include "common/copier/copier.hpp"
#include "common/locker/locker.hpp"
#include "models/transfer/transfer.hpp"

namespace srilakshmikanthanp::pulldog::common {
class Worker : public QObject {
 private: // Private members
  // Currently Coping files with copier object
  QMap<models::Transfer, common::Copier*> copingFiles;

  // Pending file update with last update time
  QMap<models::Transfer, long long> pendingFiles;

  // timer for checking pending file update
  QTimer timer;

  // mutex for updating pending file
  QMutex mutex;

  // waiting threshold for coping file
  long long threshold = 5000;

  // maximum concurrent copies
  int concurrentCopies = 20;

 private:  // Just for qt
  Q_OBJECT

 signals:
  void onCopyStart(const models::Transfer &transfer);

 signals:
  void onCopy(const models::Transfer &transfer, double progress);

 signals:
  void onCopyEnd(const models::Transfer &transfer);

 signals:
  void onCopyCancel(const models::Transfer &transfer);

 signals:
  void pathsChanged(const QString &path, bool isAdded);

 signals:
  void onError(const QString &error);

 private:
  /**
   * @brief Process the pending file update
   */
  void process(const models::Transfer &transfer);

  /**
   * @brief Slot for time process pending file update
   */
  void processPendingFileUpdate();

  /**
   * @brief slot to handle file rename
   */
  void copy(const models::Transfer &transfer);

  /**
   * @brief Is dest file upto date
   */
  bool isUptoDate(const models::Transfer &transfer);

 public:
  /**
   * @brief Construct a new File Processor object
   */
  Worker(QObject *parent = nullptr);

  /**
   * @brief Destroy the File Processor object
   */
  ~Worker() = default;

  /**
   * @brief Get threshold
   */
  long long getThreshold() const;

  /**
   * @brief Set threshold
   */
  void setThreshold(long long threshold);

  /**
   * @brief slot to handle file update
   */
  void handleFileUpdate(models::Transfer transfer);
};
}
