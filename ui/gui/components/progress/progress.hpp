#pragma once

// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QObject>
#include <QProgressBar>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

#include "models/transfer/transfer.hpp"

namespace srilakshmikanthanp::pulldog::ui::gui::components {
/**
 * @brief Progress bar
 */
class Progress : public QWidget {
 private:
  Q_DISABLE_COPY(Progress)

 private:
  Q_OBJECT

 private:
  QProgressBar *progressBar;
  models::Transfer transfer;

 public:
  /**
   * @brief Construct a new Progress object
   *
   * @param parent
   */
  Progress(models::Transfer transfer, QWidget *parent = nullptr);

  /**
   * @brief Destroy the Progress object
   */
  ~Progress();

  /**
   * @brief set the progress
   *
   * @param progress
   */
  void setProgress(double progress);

  /**
   * @brief get the progress
   *
   * @return double
   */
  double getProgress() const;

  /**
   * @brief get the transfer
   *
   * @return models::Transfer
   */
  models::Transfer getTransfer() const;
};
} // namespace srilakshmikanthanp::pulldog::ui::gui::components
