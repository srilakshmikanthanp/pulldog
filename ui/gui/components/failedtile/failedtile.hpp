// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once  // header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QFileDialog>
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QLabel>
#include <QObject>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include "models/transfer/transfer.hpp"

namespace srilakshmikanthanp::pulldog::ui::gui::components {
/**
 * @brief Failed Tile component is a tile to show failed transfers
 */
class FailedTile : public QWidget {
 private:

  Q_DISABLE_COPY(FailedTile)

 private:

  Q_OBJECT

 private:

  QLabel *label       = new QLabel();
  QPushButton *retry  = new QPushButton();
  QPushButton *ignore = new QPushButton();

 private:

  models::Transfer transfer;

 private:

  /**
   * @brief Set up the language
   */
  void setupLanguage();

 signals:
  /**
   * @brief signal to emit when the failed tile is ignored
   */
  void onIgnoreRequested(models::Transfer transfer);

 signals:
  /**
   * @brief signal to emit when the failed tile is retried
   */
  void onRetryRequested(models::Transfer transfer);

 public:

  /**
   * @brief Construct a new Failed Tile object
   *
   * @param path
   */
  FailedTile(models::Transfer transfer, QWidget *parent = nullptr);

  /**
   * @brief get the transfer
   */
  models::Transfer getTransfer();

  /**
   * @brief Destroy the Failed Tile object
   */
  ~FailedTile() = default;
};
}  // namespace srilakshmikanthanp::pulldog::ui::gui::components
