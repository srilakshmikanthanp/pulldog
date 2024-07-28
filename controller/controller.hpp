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

#include "common/copier/copier.hpp"
#include "common/locker/locker.hpp"
#include "common/watch/watch.hpp"
#include "common/worker/worker.hpp"
#include "models/transfer/transfer.hpp"
#include "store/storage.hpp"

namespace srilakshmikanthanp::pulldog {
class Controller : public QObject {
 private: // Private members
  QQueue<models::Transfer> events;

 private: // Private members
  std::atomic<int> parallelEvents = 10;
  std::atomic<int> interval = 1000;

 private: // Private members
  common::Watch watcher;
  QThread watcherThread;
  QDir destinationRoot;
  common::Worker worker;
  QThread workerThread;
  QTimer trigger;

 private:  // Just for qt
  Q_OBJECT

 private:  // slots
  void handleFileUpdate(const QString dir, const QString path);

 private:
  void handleFileRename(
    const QString directory,
    const QString oldFile,
    const QString newFile
  );

 private:
  void processEvents();

 signals:
  void onCopyStart(const models::Transfer &transfer);

 signals:
  void onCopy(const models::Transfer &transfer, double progress);

 signals:
  void onCopyEnd(const models::Transfer &transfer);

 signals:
  void onCopyCanceled(const models::Transfer &transfer);

 signals:
  void onCopyFailed(const models::Transfer &transfer);

 signals:
  void pathAdded(const QString &path);

 signals:
  void pathRemoved(const QString &path);

 signals:
  void onError(const QString &error);

 private:
  Q_SIGNAL void copy(const QString &src, const QString &dest);

 public:  // Public members
  /**
   * @brief Construct a new Controller object
   */
  Controller(QObject *parent = nullptr);

  /**
   * @brief Destroy the Controller object
   */
  ~Controller();

  /**
   * @brief get the destination root
   */
  QString getDestinationRoot() const;

  /**
   * @brief set the destination root
   */
  void setDestinationRoot(const QString &path);

  /**
   * @brief Get threshold
   */
  long long getWorkerThreshold() const;

  /**
   * @brief Set threshold
   */
  void setWorkerThreshold(long long threshold);

  /**
   * @brief set the parallel events
   */
  void setParallelEvents(int events);

  /**
   * @brief get the parallel events
   */
  int getParallelEvents() const;

  /**
   * @brief set the interval
   */
  void setProcessInterval(int interval);

  /**
   * @brief get the interval
   */
  int getProcessInterval() const;

  /**
   * @brief Get the Paths object
   */
  QStringList watchPaths() const;

  /**
   * @brief Remove a path from watch
   */
  void removeWatchPath(const QString &path);

  /**
   * @brief Retry a transfer
   */
  void retry(const models::Transfer &transfer);

  /**
   * @brief Add a path to watch
   */
  void addWatchPath(const QString &path, bool recursive = true);
};
}  // namespace srilakshmikanthanp::pulldog
