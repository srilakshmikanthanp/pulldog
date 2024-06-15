#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QObject>
#include <QSettings>

namespace srilakshmikanthanp::pulldog::storage {
class Storage : public QObject {
 private:  // settings

  QSettings *settings = new QSettings("srilakshmikanthanp", "pulldog");

 private: // groups
  const char* configGroup  = "config";

 private: // keys
  const char* paths = "paths";

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
   * @brief Instance of the storage
   */
  static Storage& instance();
};
}  // namespace srilakshmikanthanp::clipbirdesk::storage
