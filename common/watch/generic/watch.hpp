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

#include "common/watch/generic/dirwatch.hpp"
#include "common/watch/iwatch.hpp"
#include "common/watch/win/watch.hpp"
#include "types/fileid/fileid.hpp"

namespace srilakshmikanthanp::pulldog::common {
class GenericWatch : public IWatch {
 private:
  Q_DISABLE_COPY(GenericWatch)

 private: // Just for qt
  Q_OBJECT

 private:
  static inline const char* pollIntervalKey = "pollInterval";
  static inline const char* lastPollKey = "lastPoll";
  static inline const int pollInterval = 10000;
  static inline const int maxPollInterval = 60000;

 private:
  QList<DirWatcher*> directories;
  mutable QMutex mutex;
  QTimer poller;
  QThread pollerThread;

 private:
  /**
   * @brief Poll the directory on its own thread
   */
  void pollDirectory(DirWatcher *dir);

  /**
   * @brief delegates the poll to the directories
   */
  void poll();

 public:
  /**
   * @brief Construct a new Watch object
   */
  GenericWatch(QObject *parent = nullptr);

  /**
   * @brief Destroy the Watch object
   */
  ~GenericWatch();

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
