#pragma once  // #include only once see https://en.wikipedia.org/wiki/Pragma_once
#ifdef _WIN32 // only windows

// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QObject>
#include <QThread>
#include <QDir>
#include <QMutex>
#include <QMutexLocker>

#define NOMINMAX
#include <windows.h>
#undef NOMINMAX

#include "common/watch/iwatch.hpp"
#include "utility/functions/functions.hpp"

namespace srilakshmikanthanp::pulldog::common {
/**
 * @brief Currently a wrapper around QFileSystemWatcher
 */
class Watch : public IWatch {
 private:
  // structure to hold the directory Info
  struct DirWatch {
    OVERLAPPED overlapped;
    QString baseDir;
    HANDLE handle;
    QString oldFileName;
    bool recursive;
    alignas(4) uint8_t buffer[1024];
    Watch* watcher;
  };

 private:
  // process the file info from ReadDirectoryChangesW
  void processFileInfo(DirWatch *dir, const FILE_NOTIFY_INFORMATION *fileInfo);

  // get the file name from the handle
  QString getFileNameFromHandle(HANDLE handle) const;

 private:
  // callback function for ReadDirectoryChangesW
  static void CALLBACK DirectoryChangesCallback(
    DWORD errorCode,
    DWORD numberOfBytesTransfered,
    LPOVERLAPPED overlapped
  );

 private:
  // list of handles watched by ReadDirectoryChangesW
  QList<DirWatch*> directories;

 public:
  /**
   * @brief Construct a new IWatch object
   */
  Watch(QObject *parent = nullptr);

  /**
   * @brief Destroy the IWatch object
   */
  ~Watch();

  /**
   * @brief Remove a path from watch
   *
   * @param path
   */
  void removePath(const QString &path) override;

  /**
   * @brief paths
   */
  QStringList paths() const override;

  /**
   * @brief Add a path to watch
   *
   * @param path
   */
  void addPath(const QString &path, bool recursive = true) override;
};
}  // namespace srilakshmikanthanp::pulldog::common::watcher
#endif  // _WIN32
