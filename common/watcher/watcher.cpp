// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "Watcher.hpp"

namespace srilakshmikanthanp::pulldog::common {
/**
 * @brief Construct a new Watcher:: Watcher object
 *
 * @param parent
 */
Watcher::Watcher(QObject *parent) : QObject(parent) {
  watcher->watch();
}

/**
 * @brief Destroy the Watcher:: Watcher object
 */
Watcher::~Watcher() {
  delete watcher;
}

/**
 * @brief paths
 *
 * @return QStringList
 */
QStringList Watcher::paths() const {
  auto paths = QStringList();

  for (const auto &path : watcher->directories()) {
    paths.append(QString::fromStdString(path));
  }

  return paths;
}

/**
 * @brief Add a path to watch
 *
 * @param path
 */
bool Watcher::addPath(const QString &path, bool recursive) {
  return watcher->addWatch(path.toStdString(), this, recursive);
}

/**
 * @brief Remove a path from watch
 *
 * @param path
 */
void Watcher::removePath(const QString &path) {
  return watcher->removeWatch(path.toStdString());
}

/**
 * @brief handleFileAction
 *
 * @param watchId
 * @param dir
 * @param filename
 * @param action
 * @param oldFilename
 */
void Watcher::handleFileAction(
  efsw::WatchID watchId,
  const std::string &dir,
  const std::string &filename,
  efsw::Action action,
  std::string oldFile
) {
  switch (action) {
  case efsw::Actions::Modified:
    emit fileChanged(
      QString::fromStdString(dir),
      QString::fromStdString(filename)
    );
    break;
  case efsw::Actions::Add:
    emit fileCreated(
      QString::fromStdString(dir),
      QString::fromStdString(filename)
    );
    break;
  case efsw::Actions::Delete:
    emit fileRemoved(
      QString::fromStdString(dir),
      QString::fromStdString(filename)
    );
    break;
  case efsw::Actions::Moved:
    emit fileMoved(
      QString::fromStdString(dir),
      QString::fromStdString(oldFile),
      QString::fromStdString(filename)
    );
    break;
  }
}
}  // namespace srilakshmikanthanp::pulldog::common::watcher
