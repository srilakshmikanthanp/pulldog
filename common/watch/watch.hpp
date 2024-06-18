#pragma once  // #include only once see https://en.wikipedia.org/wiki/Pragma_once

// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QObject>
#include <QDir>

#ifdef _WIN32
#include "windows/watchimpl.hpp"
#endif

namespace srilakshmikanthanp::pulldog::common {
/**
 * @brief Currently a wrapper around QFileSystemWatcher
 */
class Watch : public QObject {
 private:
  WatchImpl watcher;

 private:
  Q_DISABLE_COPY(Watch)

 private: // Just for qt
  Q_OBJECT

 signals:
  void fileCreated(const QString &dir, const QString &file);
  void fileRemoved(const QString &dir, const QString &file);
  void fileUpdated(const QString &dir, const QString &file);

 signals:
  void fileMoved(
    const QString directory,
    const QString oldFile,
    const QString newFile
  );

 signals:
  void pathsChanged(const QString &path, bool isAdded);

 signals:
  void onError(const QString &error);

 public:
  /**
   * @brief Construct a new Watch object
   */
  Watch(QObject *parent = nullptr);

  /**
   * @brief Destroy the Watch object
   */
  ~Watch() = default;

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
}  // namespace srilakshmikanthanp::pulldog::common::watcher
