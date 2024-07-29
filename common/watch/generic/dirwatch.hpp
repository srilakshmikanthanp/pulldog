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
#include "types/fileid/fileid.hpp"

namespace srilakshmikanthanp::pulldog::common {
class DirWatcher : public QObject {
 private:
  Q_DISABLE_COPY(DirWatcher)

 private: // Just for qt
  Q_OBJECT

 private:
  QString path;

 private:
  using FileInfo = QPair<QFileInfo, types::FileId>;

 private:
  QMap<QString, FileInfo> files;

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

 public:
  /**
   * @brief Construct a new Directory Watcher object
   *
   * @param path
   * @param parent
   */
  DirWatcher(const QString &path, QObject *parent = nullptr);

  /**
   * @brief Poll the directory return true if any change
   */
  bool poll();

  /**
   * @brief Get the Path object
   */
  QString getPath() const;

  /**
   * @brief Destroy the Directory Watcher object
   */
  ~DirWatcher() override = default;
};
} // namespace srilakshmikanthanp::pulldog::common
