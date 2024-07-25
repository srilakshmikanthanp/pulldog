// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "failedinfo.hpp"

namespace srilakshmikanthanp::pulldog::ui::gui::screens {
/**
 * @brief Constructor for FailedInfo
 */
FailedInfo::FailedInfo(QWidget *parent): QWidget(parent) {
  // set the scroll area
  scrollArea->setWidgetResizable(true);
  scrollArea->setWidget(this->scrollAreaWidget);

  // disable horizontal scrolling
  scrollArea->horizontalScrollBar()->setEnabled(false);

  // set the layout for the scroll area
  this->scrollAreaWidget->setLayout(this->scrollAreaLayout);

  // set center alignment
  this->noFailures->setAlignment(Qt::AlignCenter);

  // set the no progress label
  this->stackLayout->addWidget(this->noFailures);
  this->stackLayout->addWidget(this->scrollArea);

  // align top
  scrollAreaLayout->setAlignment(Qt::AlignTop);

  // set the language
  this->setUpLanguage();
}

/**
 * @brief Destructor for FailedInfo
 */
FailedInfo::~FailedInfo() {
  for (auto progress: this->failedList) {
    delete progress;
  }
}

void FailedInfo::setUpLanguage() {
  this->noFailures->setText(tr("No Failures"));
}

/**
 * @brief Remove Tile
 *
 * @param progress
 */
void FailedInfo::removeFailedTile(components::FailedTile *tile) {
  // remove the tile from the layout
  this->scrollAreaLayout->removeWidget(tile);

  // remove the tile from the list
  this->failedList.removeOne(tile);

  // disconnect the signal
  disconnect(
    tile, &components::FailedTile::onRetryRequested,
    this, &FailedInfo::onRetryRequested
  );

  // up date the scroll area
  this->update();
}

/**
 * @brief Add Tile
 *
 * @param progress
 */
void FailedInfo::addFailedTile(components::FailedTile *tile) {
  // add the progress to the layout
  this->scrollAreaLayout->insertWidget(0, tile);

  // add the progress to the list
  this->failedList.append(tile);

  // connect the signal
  connect(
    tile, &components::FailedTile::onRetryRequested,
    [=](models::Transfer transfer) {
      emit this->onRetryRequested(transfer);
    }
  );

  // up date the scroll area
  this->update();
}

/**
 * @brief Get tile List
 *
 * @return QList<components::FailedTile *>
 */
QList<components::FailedTile *> FailedInfo::getFailedTiles() const {
  return this->failedList;
}

/**
 * @brief Override the showEvent
 */
void FailedInfo::paintEvent(QPaintEvent *event) {
  // if the vertical layout is empty then add a label
  if (scrollAreaLayout->count() == 0) {
    this->stackLayout->setCurrentIndex(0);
  } else {
    this->stackLayout->setCurrentIndex(1);
  }

  // call the base class
  QWidget::paintEvent(event);
}
}  // namespace srilakshmikanthanp::pulldog::ui::gui::screens
