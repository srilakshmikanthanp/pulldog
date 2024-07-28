// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "pullinfo.hpp"

namespace srilakshmikanthanp::pulldog::ui::gui::screens {
/**
 * @brief Constructor for PullInfo
 */
PullInfo::PullInfo(QWidget *parent): QWidget(parent) {
  this->setLayout(this->stackLayout);

  // disable horizontal scrolling
  scrollArea->horizontalScrollBar()->setEnabled(false);

  // set the scroll area
  scrollArea->setWidgetResizable(true);
  scrollArea->setWidget(this->scrollAreaWidget);

  // set the layout for the scroll area
  scrollAreaWidget->setLayout(this->scrollAreaLayout);

  // set center alignment
  noProgress->setAlignment(Qt::AlignCenter);

  // set the no progress label
  stackLayout->addWidget(this->noProgress);
  stackLayout->addWidget(this->scrollArea);

  // align top
  scrollAreaLayout->setAlignment(Qt::AlignTop);

  // set the language
  this->setUpLanguage();
}

void PullInfo::setUpLanguage() {
  this->noProgress->setText(tr("All Transfers are Completed"));
}

/**
 * @brief Remove Progress
 *
 * @param progress
 */
void PullInfo::removeProgress(components::Progress *progress) {
  // remove the progress from the layout
  this->scrollAreaLayout->removeWidget(progress);

  // up date the scroll area
  this->update();
}

/**
 * @brief Add Progress
 *
 * @param progress
 */
void PullInfo::addProgress(components::Progress *progress) {
  // add the progress to the layout
  this->scrollAreaLayout->insertWidget(0, progress);

  // up date the scroll area
  this->update();
}

/**
 * @brief Get Progress List
 *
 * @return QList<components::Progress *>
 */
QList<components::Progress *> PullInfo::getProgressList() const {
  QList<components::Progress *> progressList;

  for (int i = 0; i < this->scrollAreaLayout->count(); i++) {
    auto widget = this->scrollAreaLayout->itemAt(i)->widget();
    auto item = static_cast<components::Progress *>(widget);
    progressList.append(item);
  }

  return progressList;
}

/**
 * @brief Override the showEvent
 */
void PullInfo::paintEvent(QPaintEvent *event) {
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
