#pragma once  // #include only once see https://en.wikipedia.org/wiki/Pragma_once

// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <efsw/FileWatcher.cpp>
#include <QObject>

namespace srilakshmikanthanp::pulldog::common {
/**
 * @brief Currently a wrapper around QFileSystemWatcher
 */
class Watcher : public QObject, private efsw::FileWatchListener {
 private:
  efsw::FileWatcher *watcher = new efsw::FileWatcher();

 private:
  Q_DISABLE_COPY(Watcher)

 private: // Just for qt
  Q_OBJECT

 signals:
  void fileCreated(const QString dir, const QString file);
  void fileRemoved(const QString dir, const QString file);
  void fileChanged(const QString dir, const QString file);

 signals:
  void fileMoved(
    const QString directory,
    const QString oldFile,
    const QString newFile
  );

 public:
  /**
   * @brief Construct a new Watcher object
   */
  Watcher(QObject *parent = nullptr);

  /**
   * @brief Destroy the Watcher object
   */
  ~Watcher();

  /**
   * @brief paths
   */
  QStringList paths() const;

  /**
   * @brief Add a path to watch
   *
   * @param path
   */
  bool addPath(const QString &path, bool recursive = true);

  /**
   * @brief Remove a path from watch
   *
   * @param path
   */
  void removePath(const QString &path);

 private:
  void handleFileAction(
    efsw::WatchID watchId,
    const std::string& dir,
    const std::string& filename,
    efsw::Action action,
    std::string oldFilename
  ) override;
};
}  // namespace srilakshmikanthanp::pulldog::common::watcher
