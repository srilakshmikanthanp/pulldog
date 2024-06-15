#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QObject>
#include <QSharedPointer>

#include "common/copier/copier.hpp"
#include "common/watcher/watcher.hpp"

namespace srilakshmikanthanp::pulldog {
class Controller : public QObject {
 private: // Private members
  common::Watcher watcher;
  QDir destinationRoot;

 private:  // Just for qt
  Q_OBJECT

 private:  // slots
  void handleFileUpdate(const QString dir, const QString path);

 signals:
  void onFileCopy(QSharedPointer<common::Copier> copier);

 public:  // Public members
  /**
   * @brief Construct a new Controller object
   */
  Controller(QString destRoot, QObject *parent = nullptr);

  /**
   * @brief Destroy the Controller object
   */
  ~Controller() = default;

  /**
   * @brief Get the Paths object
   */
  QStringList paths() const;

  /**
   * @brief Remove a path from watch
   */
  void removePath(const QString &path);

  /**
   * @brief Add a path to watch
   */
  bool addPath(const QString &path, bool recursive = true);
};
}  // namespace srilakshmikanthanp::pulldog
