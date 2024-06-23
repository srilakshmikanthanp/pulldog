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
  progressBar = new QProgressBar();
  progressBar->setRange(0, 100);
  progressBar->setValue(0);

  // center align progress text in progress bar
  progressBar->setAlignment(Qt::AlignCenter);

  // create label
  from    = new QLabel(transfer.getFrom(), this);
  to      = new QLabel(transfer.getTo(), this);

  // create layout this shows copy from and copy to with progress bar
  auto hLayout = new QHBoxLayout();
  hLayout->addWidget(from);
  hLayout->addWidget(new QLabel("to"));
  hLayout->addWidget(to);

  hLayout->setAlignment(Qt::AlignLeft);
  hLayout->setSpacing(5);

  // vertical layout
  auto vLayout = new QVBoxLayout();
  vLayout->addLayout(hLayout);
  vLayout->addWidget(progressBar);

  // add stretch
  vLayout->addStretch();

  // set layout
  setLayout(vLayout);
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

/**
 * @brief Paint event
 */
void Progress::paintEvent(QPaintEvent *event) {
  auto fromText = transfer.getFrom();
  QFontMetrics metrics(from->font());
  QString elidedText = metrics.elidedText(
    fromText, Qt::ElideRight, from->width()
  );
  from->setText(elidedText);

  auto toText = transfer.getTo();
  elidedText = metrics.elidedText(
    toText, Qt::ElideRight, to->width()
  );
  to->setText(elidedText);

  // set only minimal needed size
  this->setMaximumHeight(
    this->minimumSizeHint().height()
  );

  // For Style sheet
  QStyleOption opt;
  opt.initFrom(this);
  QPainter p(this);
  style()->drawPrimitive(
    QStyle::PE_Widget, &opt, &p, this
  );

  QWidget::paintEvent(event);
}
}  // namespace srilakshmikanthanp::pulldog::ui::gui::components
