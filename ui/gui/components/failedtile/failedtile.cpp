// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "failedtile.hpp"

namespace srilakshmikanthanp::pulldog::ui::gui::components {
/**
 * @brief Failed Tile component is a tile to show failed transfers
 */
FailedTile::FailedTile(models::Transfer transfer, QWidget *parent): QWidget(parent), transfer(transfer) {
  // set the layout
  auto layout = new QHBoxLayout();

  // set the label
  label->setText(transfer.getFrom());

  // set the style
  retry->setIcon(QIcon(":/images/refresh.png"));
  retry->setObjectName("Transparent");
  retry->setCursor(Qt::PointingHandCursor);

  // add the label to the layout
  layout->addWidget(label);

  // stretch the layout
  layout->addStretch();

  // add the retry button to the layout
  layout->addWidget(retry);
  layout->addWidget(ignore);

  // set the layout
  setLayout(layout);

  // connect the ignore button
  connect(
    ignore, &QPushButton::clicked,
    [=] { emit onIgnoreRequested(transfer); }
  );

  // connect the retry button
  connect(
    retry, &QPushButton::clicked,
    [=] { emit onRetryRequested(transfer); }
  );

  // set the language
  this->setupLanguage();
}

/**
 * @brief Set up the language
 */
void FailedTile::setupLanguage() {
  ignore->setText(tr("Ignore"));
}


/**
 * @brief get the transfer
 */
models::Transfer FailedTile::getTransfer() {
  return this->transfer;
}
}  // namespace srilakshmikanthanp::pulldog::ui::gui::components
