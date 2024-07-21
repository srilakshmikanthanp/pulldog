#pragma once  // #include only once see https://en.wikipedia.org/wiki/Pragma_once

// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QObject>
#include <QDir>
#include <QMap>
#include <QMutex>
#include <QThread>
#include <QFileInfo>
#include <QTimer>

#include <filesystem>

#include "common/watch/iwatch.hpp"
#include "common/watch/win/watch.hpp"

namespace srilakshmikanthanp::pulldog::common {
class DirectoryWatcher : public QObject {
 private:
  Q_DISABLE_COPY(DirectoryWatcher)

 private: // Just for qt
  Q_OBJECT

 private:
  QString path;

 private:
  QMap<QString, QFileInfo> files;


 signals:
  void fileCreated(const QString &dir, const QString &file);
  void fileRemoved(const QString &dir, const QString &file);
  void fileUpdated(const QString &dir, const QString &file);

 public:
  /**
   * @brief Construct a new Directory Watcher object
   *
   * @param path
   * @param parent
   */
  DirectoryWatcher(const QString &path, QObject *parent = nullptr);

  /**
   * @brief Poll the directory
   */
  void poll();

  /**
   * @brief Get the Path object
   */
  QString getPath() const;

  /**
   * @brief Destroy the Directory Watcher object
   */
  ~DirectoryWatcher() override = default;
};

// --------------------------------------------------------------------------------

class GenericWatch : public IWatch {
 private:
  Q_DISABLE_COPY(GenericWatch)

 private: // Just for qt
  Q_OBJECT

 private:
  constexpr static int pollInterval = 1000;

 private:
  QList<DirectoryWatcher*> directories;
  mutable QMutex mutex;
  QTimer poller;
  QThread watcherThread;
  QThread pollerThread;

 private:
  void poll();

 public:
  /**
   * @brief Construct a new Watch object
   */
  GenericWatch(QObject *parent = nullptr);

  /**
   * @brief Destroy the Watch object
   */
  ~GenericWatch() override = default;

  /**
   * @brief Remove a path from watch
   *
   * @param path
   */
  void removePath(const QString &path);

  /**
   * @brief paths
   */
  QStringList paths() const;

  /**
   * @brief Add a path to watch
   *
   * @param path
   */
  void addPath(const QString &path, bool recursive = true);
};
} // namespace srilakshmikanthanp::pulldog::common
