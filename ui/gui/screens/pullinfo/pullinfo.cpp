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
  // set the scroll area
  scrollArea->setWidgetResizable(true);
  scrollArea->setWidget(this->scrollAreaWidget);

  // disable horizontal scrolling
  scrollArea->horizontalScrollBar()->setEnabled(false);

  // set the layout for the scroll area
  this->scrollAreaWidget->setLayout(this->scrollAreaLayout);

  // set center alignment
  this->noProgress->setAlignment(Qt::AlignCenter);

  // set the no progress label
  this->stackLayout->addWidget(this->noProgress);
  this->stackLayout->addWidget(this->scrollArea);

  // align top
  scrollAreaLayout->setAlignment(Qt::AlignTop);

  // set the language
  this->setUpLanguage();
}

/**
 * @brief Destructor for PullInfo
 */
PullInfo::~PullInfo() {
  for (auto progress: this->progressList) {
    delete progress;
  }
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

  // remove the progress from the list
  this->progressList.removeOne(progress);

  // delete the progress
  progress->deleteLater();

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
  this->scrollAreaLayout->addWidget(progress);

  // add the progress to the list
  this->progressList.append(progress);

  // up date the scroll area
  this->update();
}

/**
 * @brief Get Progress List
 *
 * @return QList<components::Progress *>
 */
QList<components::Progress *> PullInfo::getProgressList() const {
  return this->progressList;
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
