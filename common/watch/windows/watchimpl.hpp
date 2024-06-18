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

#include <windows.h>

namespace srilakshmikanthanp::pulldog::common {
/**
 * @brief Currently a wrapper around QFileSystemWatcher
 */
class WatchImpl : public QObject {
 private:
  Q_DISABLE_COPY(WatchImpl)

 private: // Just for qt
  Q_OBJECT

 signals:
  void fileCreated(const QString &dir, const QString &file);

 signals:
  void fileRemoved(const QString &dir, const QString &file);

 signals:
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
  WatchImpl(QObject *parent = nullptr);

  /**
   * @brief Destroy the Watch object
   */
  ~WatchImpl();

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

 private:
  // structure to hold the directory Info
  struct DirWatch {
    OVERLAPPED overlapped;
    QString baseDir;
    HANDLE handle;
    QString oldFileName;
    bool recursive;
    alignas(4) uint8_t buffer[1024];
    WatchImpl* watcher;
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
};
}  // namespace srilakshmikanthanp::pulldog::common::watcher
#endif  // _WIN32
