// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "progress.hpp"

namespace srilakshmikanthanp::pulldog::ui::gui::components {
/**
 * @brief Construct a new Progress object
 *
 * @param transfer
 * @param parent
 */
Progress::Progress(models::Transfer transfer, QWidget *parent)
    : QWidget(parent), transfer(transfer) {
  // create progress bar
  progressBar = new QProgressBar(this);
  progressBar->setRange(0, 100);
  progressBar->setValue(0);

  // create label
  auto from    = new QLabel(transfer.getFrom(), this);
  auto to      = new QLabel(transfer.getTo(), this);

  // create layout this shows copy from and copy to with progress bar
  auto hlayout = new QHBoxLayout(this);
  hlayout->addWidget(from);
  hlayout->addWidget(new QLabel("->", this));
  hlayout->addWidget(to);

  // vertical layout
  auto vlayout = new QVBoxLayout(this);
  vlayout->addLayout(hlayout);
  vlayout->addWidget(progressBar);

  // set layout
  setLayout(vlayout);
}

/**
 * @brief Destroy the Progress object
 */
Progress::~Progress() {
  progressBar->deleteLater();
}

/**
 * @brief set the progress
 *
 * @param progress
 */
void Progress::setProgress(double progress) {
  progressBar->setValue(progress);
}

/**
 * @brief get the progress
 *
 * @return double
 */
double Progress::getProgress() const {
  return progressBar->value();
}

/**
 * @brief get the transfer
 *
 * @return models::Transfer
 */
models::Transfer Progress::getTransfer() const {
  return transfer;
}
}  // namespace srilakshmikanthanp::pulldog::ui::gui::components
