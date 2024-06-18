// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "watch.hpp"

namespace srilakshmikanthanp::pulldog::common {
/**
 * @brief Construct a new Watcher:: Watcher object
 *
 * @param parent
 */
Watch::Watch(QObject *parent) : QObject(parent) {
  connect(
    &this->watcher,
    &WatchImpl::fileCreated,
    this,
    &Watch::fileCreated
  );

  connect(
    &this->watcher,
    &WatchImpl::fileRemoved,
    this,
    &Watch::fileRemoved
  );

  connect(
    &this->watcher,
    &WatchImpl::fileUpdated,
    this,
    &Watch::fileUpdated
  );

  connect(
    &this->watcher,
    &WatchImpl::fileMoved,
    this,
    &Watch::fileMoved
  );

  connect(
    &this->watcher,
    &WatchImpl::onError,
    this,
    &Watch::onError
  );

  connect(
    &this->watcher,
    &WatchImpl::pathsChanged,
    this,
    &Watch::pathsChanged
  );
}

/**
 * @brief paths
 *
 * @return QStringList
 */
QStringList Watch::paths() const {
  return this->watcher.paths();
}

/**
 * @brief Add a path to watch
 *
 * @param path
 */
void Watch::addPath(const QString &path, bool recursive) {
  return this->watcher.addPath(path, recursive);
}

/**
 * @brief Remove a path from watch
 *
 * @param path
 */
void Watch::removePath(const QString &path) {
  this->watcher.removePath(path);
}
}  // namespace srilakshmikanthanp::pulldog::common::watcher
