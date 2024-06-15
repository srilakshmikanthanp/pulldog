#pragma once  // #include only once see https://en.wikipedia.org/wiki/Pragma_once

// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QObject>
#include <QFile>

namespace srilakshmikanthanp::pulldog::common {
/**
 * @brief A Class that copies a file from one location to another
 * with a progress signal
 */
class Copier : public QObject {
 private:
  QString source, destination;
  std::atomic<double> progress = 0;
  int bufferSize = 1024;

 private:

  Q_DISABLE_COPY(Copier)

 private:  // Just for qt

  Q_OBJECT

 signals:
  void onChange(double progress);

 signals:
  void started(double progress);

 signals:
  void finished(double progress);

 public:

  /**
   * @brief Construct a new Copier object
   */
  Copier(QString src, QString dest, QObject *parent = nullptr);

  /**
   * @brief Destroy the Copier object
   */
  ~Copier() = default;

  /**
   * @brief Copy the file
   */
  void start();

  /**
   * @brief get the progress
   */
  double getProgress() const;

  /**
   * @brief get the source
   */
  QString getSource() const;

  /**
   * @brief get the destination
   */
  QString getDestination() const;
};
}  // namespace srilakshmikanthanp::pulldog::common::copier
