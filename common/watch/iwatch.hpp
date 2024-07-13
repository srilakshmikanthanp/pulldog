#pragma once  // #include only once see https://en.wikipedia.org/wiki/Pragma_once

// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QObject>
#include <QDir>

namespace srilakshmikanthanp::pulldog::common {
/**
 * @brief Currently a wrapper around QFileSystemWatcher
 */
class IWatch : public QObject {
 private:
  Q_DISABLE_COPY(IWatch)

 private: // Just for qt
  Q_OBJECT

 signals:
  void fileCreated(const QString &dir, const QString &file);
  void fileRemoved(const QString &dir, const QString &file);
  void fileUpdated(const QString &dir, const QString &file);

 signals:
  void fileRename(
    const QString directory,
    const QString oldFile,
    const QString newFile
  );

 signals:
  void pathRemoved(const QString &path);

 signals:
  void pathAdded(const QString &path);

 signals:
  void onError(const QString &error);

 public:
  /**
   * @brief Construct a new Watch object
   */
  IWatch(QObject *parent = nullptr);

  /**
   * @brief Destroy the Watch object
   */
  virtual ~IWatch() = default;

  /**
   * @brief Remove a path from watch
   *
   * @param path
   */
  virtual void removePath(const QString &path) = 0;

  /**
   * @brief paths
   */
  virtual QStringList paths() const = 0;

  /**
   * @brief Add a path to watch
   *
   * @param path
   */
  virtual void addPath(const QString &path, bool recursive = true) = 0;
};
}  // namespace srilakshmikanthanp::pulldog::common::watcher
