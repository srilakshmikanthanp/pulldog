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
  this->settings->beginGroup(this->filesGroup);
  this->settings->setValue(this->paths, paths);
  this->settings->endGroup();
}

/**
 * @brief Get all the paths
 */
QStringList Storage::getPaths() {
  this->settings->beginGroup(this->filesGroup);
  auto paths = this->settings->value(this->paths).toStringList();
  this->settings->endGroup();
  return paths;
}

/**
 * @brief Remove all the paths
 */
void Storage::clearPaths() {
  this->settings->beginGroup(this->filesGroup);
  this->settings->remove(this->paths);
  this->settings->endGroup();
}

/**
 * @brief Add Path to store
 */
void Storage::addPath(const QString& path) {
  this->settings->beginGroup(this->filesGroup);
  auto paths = this->settings->value(this->paths).toStringList();
  paths.append(path);
  this->settings->setValue(this->paths, paths);
  this->settings->endGroup();
}

/**
 * @brief Remove path from store
 */
void Storage::removePath(const QString& path) {
  this->settings->beginGroup(this->filesGroup);
  auto paths = this->settings->value(this->paths).toStringList();
  paths.removeAll(path);
  this->settings->setValue(this->paths, paths);
  this->settings->endGroup();
}

/**
 * @brief Instance of the storage
 */
Storage& Storage::instance() {
  static Storage instance;
  return instance;
}
}  // namespace srilakshmikanthanp::pulldog::storage
