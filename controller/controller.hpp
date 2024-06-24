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
#include "models/transfer/transfer.hpp"
#include "store/storage.hpp"

namespace srilakshmikanthanp::pulldog {
class FileProcessor : public QObject {
 private: // Private members
  // Currently Coping files with copier object
  QMap<models::Transfer, common::Copier*> copingFiles;

  // Pending file update with last update time
  QMap<models::Transfer, long long> pendingFiles;

  // timer for checking pending file update
  QTimer timer;

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
   * @brief slot to handle file rename
   */
  void copy(const models::Transfer &transfer);

  /**
   * @brief Slot for time process pending file update
   */
  void processPendingFileUpdate();

 public:
  /**
   * @brief Construct a new File Processor object
   */
  FileProcessor(QObject *parent = nullptr);

  /**
   * @brief Destroy the File Processor object
   */
  ~FileProcessor() = default;

  /**
   * @brief slot to handle file update
   */
  void handleFileUpdate(models::Transfer transfer);

  /**
   * @brief Get threshold
   */
  long long getThreshold() const;

  /**
   * @brief Set threshold
   */
  void setThreshold(long long threshold);

  /**
   * @brief Get the Concurrent Copies
   */
  int getConcurrentCopies() const;

  /**
   * @brief Set the Concurrent Copies
   */
  void setConcurrentCopies(int concurrentCopies);
};

//------------------------------------------------------------------------

class Controller : public QObject {
 private: // Private members
  common::Watch watcher;
  QDir destinationRoot;
  FileProcessor fileProcessor;
  QThread fileProcessorThread;

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
  Q_SIGNAL void copy(const QString &src, const QString &dest);

 public:  // Public members
  /**
   * @brief Construct a new Controller object
   */
  Controller(QObject *parent = nullptr);

  /**
   * @brief Destroy the Controller object
   */
  ~Controller() = default;

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
  long long getThreshold() const;

  /**
   * @brief Set threshold
   */
  void setThreshold(long long threshold);

  /**
   * @brief Get the Concurrent Copies
   */
  int getConcurrentCopies() const;

  /**
   * @brief Set the Concurrent Copies
   */
  void setConcurrentCopies(int concurrentCopies);

  /**
   * @brief Get the Paths object
   */
  QStringList watchPaths() const;

  /**
   * @brief Remove a path from watch
   */
  void removeWatchPath(const QString &path);

  /**
   * @brief Add a path to watch
   */
  void addWatchPath(const QString &path, bool recursive = true);
};
}  // namespace srilakshmikanthanp::pulldog
