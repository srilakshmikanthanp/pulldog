#pragma once

// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QObject>
#include <QFileDialog>
#include <QPushButton>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

namespace srilakshmikanthanp::pulldog::ui::gui::components {
/**
 * @brief Progress bar
 */
class PathPick : public QWidget {
 private:
  Q_DISABLE_COPY(PathPick)

 private:
  Q_OBJECT

 private:
  QPushButton *button = new QPushButton(this);
  QLabel *label = new QLabel(this);

 private:
  QString path;

 private:
  /**
   * @brief Set up the language
   */
  void setupLanguage();

  /**
   * @brief on browse button clicked
   */
  void onBrowse();

 signals:
  /**
   * @brief signal to emit when the progress is changed
   */
  void pathChanged(QString path);

 public:
  /**
   * @brief Construct a new Progress object
   *
   * @param parent
   */
  PathPick(QString path, QWidget *parent = nullptr);

  /**
   * @brief Destroy the Progress object
   */
  ~PathPick() = default;

  /**
   * @brief get the progress
   *
   * @return double
   */
  QString getPath() const;

  /**
   * @brief Set the path
   */
  void setPath(QString path);
};
} // namespace srilakshmikanthanp::pulldog::ui::gui::components
