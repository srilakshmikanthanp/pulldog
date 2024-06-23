#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QObject>
#include <QSettings>

#include "constants/constants.hpp"

namespace srilakshmikanthanp::pulldog::storage {
class Storage : public QObject {
 private:  // settings

  QSettings *settings = new QSettings("srilakshmikanthanp", "pulldog");

 private: // groups
  const char* filesGroup  = "files";

 private: // keys
  const char* downloadPath = "downloadPath";
  const char* paths = "paths";

 signals:
  void onDownloadPathChanged(const QString& path);
  void onPathAdded(const QString& path);
  void onPathRemoved(const QString& path);

 private:  // qt

  Q_OBJECT

 private:  // constructor

  /**
   * @brief Construct a new SQLStore object
   */
  Storage(QObject *parent = nullptr);

 private:  // constructor

  Q_DISABLE_COPY_MOVE(Storage)

 public:  // methods

  /**
   * @brief Destroy the SQLStore object
   */
  virtual ~Storage() = default;

  /**
   * @brief Set the paths
   */
  void setPaths(const QStringList& paths);

  /**
   * @brief Get all the paths
   */
  QStringList getPaths();

  /**
   * @brief Remove all the paths
   */
  void clearPaths();

  /**
   * @brief Add Path to store
   */
  void addPath(const QString& path);

  /**
   * @brief Remove path from store
   */
  void removePath(const QString& path);

  /**
   * @brief Get the Download Path
   */
  QString getDownloadPath();

  /**
   * @brief Set the Download Path
   */
  void setDownloadPath(const QString& path);

  /**
   * @brief Instance of the storage
   */
  static Storage& instance();
};
}  // namespace srilakshmikanthanp::clipbirdesk::storage
