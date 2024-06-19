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

#include "common/copier/copier.hpp"
#include "common/locker/locker.hpp"
#include "common/watch/watch.hpp"
#include "models/transfer/transfer.hpp"

namespace srilakshmikanthanp::pulldog {
class Controller : public QObject {
 private: // Private members
  common::Copier copier;
  QDir destinationRoot;
  QThread thread;
  common::Watch watcher;

 private:
  // Pending file update with last update time
  QMap<QPair<QString, QString>, long long> pendingFileUpdate;

 private:
  // timer for checking pending file update
  QTimer timer;

 private:
  long long THRESHOLD = 10000;

 private:  // Just for qt
  Q_OBJECT

 private:  // slots
  void handleFileUpdate(const QString dir, const QString path);

 signals:
  void onCopyStart(const models::Transfer &transfer);

 signals:
  void onCopy(const models::Transfer &transfer, double progress);

 signals:
  void onCopyEnd(const models::Transfer &transfer);

 signals:
  void pathsChanged(const QString &path, bool isAdded);

 signals:
  void onError(const QString &error);

 private:
  Q_SIGNAL void copy(const QString &src, const QString &dest);

 private:
  void processPendingFileUpdate();

 private:  // Private members
  /**
   * @brief Construct a new Controller object
   */
  Controller(const QString &destRoot, QObject *parent = nullptr);

 public:  // Public members
  /**
   * @brief Destroy the Controller object
   */
  ~Controller();

  /**
   * @brief get the destination root
   */
  QString getDestinationRoot() const;

  /**
   * @brief Get the Paths object
   */
  QStringList paths() const;

  /**
   * @brief Remove a path from watch
   */
  void removePath(const QString &path);

  /**
   * @brief Add a path to watch
   */
  void addPath(const QString &path, bool recursive = true);

  /**
   * @brief Instance of the controller
   */
  static Controller& instance(const QString &destRoot, QObject *parent = nullptr);
};
}  // namespace srilakshmikanthanp::pulldog
