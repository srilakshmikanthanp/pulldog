// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "storage.hpp"

namespace srilakshmikanthanp::pulldog::storage {
/**
 * @brief Construct a new SQLStore object
 *
 * @param parent
 */
Storage::Storage(QObject* parent) : QObject(parent) {
  this->settings->setParent(this);
}

/**
 * @brief Set the paths
 */
void Storage::setPaths(const QStringList& paths) {
  this->settings->beginGroup(this->watchGroup);
  this->settings->setValue(this->paths, paths);
  this->settings->endGroup();
}

/**
 * @brief Get all the paths
 */
QStringList Storage::getPaths() {
  this->settings->beginGroup(this->watchGroup);
  auto paths = this->settings->value(this->paths).toStringList();
  this->settings->endGroup();
  return paths;
}

/**
 * @brief Remove all the paths
 */
void Storage::clearPaths() {
  this->settings->beginGroup(this->watchGroup);
  this->settings->remove(this->paths);
  this->settings->endGroup();
}

/**
 * @brief Add Path to store
 */
void Storage::addPath(const QString& path) {
  this->settings->beginGroup(this->watchGroup);
  auto paths = this->settings->value(this->paths).toStringList();
  if(!paths.contains(path)) paths.append(path);
  this->settings->setValue(this->paths, paths);
  this->settings->endGroup();
  emit onPathAdded(path);
}

/**
 * @brief Remove path from store
 */
void Storage::removePath(const QString& path) {
  this->settings->beginGroup(this->watchGroup);
  auto paths = this->settings->value(this->paths).toStringList();
  paths.removeAll(path);
  this->settings->setValue(this->paths, paths);
  this->settings->endGroup();
  emit onPathRemoved(path);
}

/**
 * @brief Get the Download Path
 */
QString Storage::getDownloadPath() {
  this->settings->beginGroup(this->downloadGroup);
  auto path = settings->value(this->downloadPath);
  this->settings->endGroup();

  if (path.isNull() || path.toString().isEmpty()) {
    return constants::getDownloadPath();
  }

  return path.toString();
}

/**
 * @brief Set the Download Path
 */
void Storage::setDownloadPath(const QString& path) {
  this->settings->beginGroup(this->downloadGroup);
  this->settings->setValue(this->downloadPath, path);
  this->settings->endGroup();
  auto test = getDownloadPath();
  emit onDownloadPathChanged(path);
}

/**
 * @brief Instance of the storage
 */
Storage& Storage::instance() {
  static Storage instance;
  return instance;
}
}  // namespace srilakshmikanthanp::pulldog::storage
