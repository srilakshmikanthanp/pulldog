// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "pulldog.hpp"

namespace srilakshmikanthanp::pulldog::ui::gui::window {
/**
 * @brief Construct a new Pull Dog object
 */
PullDog::PullDog(Controller *controller, QWidget *parent): QWidget(parent), controller(controller) {
  // create the layout for the default widget
  auto containerLayout = new QVBoxLayout();

  // add the title bar
  containerLayout->addWidget(titleBar);

  // add Pull Info
  containerLayout->addWidget(pullInfo);

  // set the layout
  container->setLayout(containerLayout);

  // add the default widget to the stacked widget
  stackedWidget->addWidget(container);

  // add the settings widget to the stacked widget
  stackedWidget->addWidget(settings);

  // create the final layout
  auto layout = new QVBoxLayout();

  // add the stacked widget to the layout
  layout->addWidget(stackedWidget);

  // set the layout
  setLayout(layout);

  // on settings clicked
  connect(
    titleBar, &components::TitleBar::settingsClicked,
    [this] { stackedWidget->setCurrentIndex(1); }
  );

  // on close
  connect(
    settings, &screens::Settings::onCloseClicked,
    [this] { stackedWidget->setCurrentIndex(0); }
  );

  // connect added
  connect(
    settings, &screens::Settings::onFolderAddRequested,
    this, &PullDog::onFolderAddRequested
  );

  // connect removed
  connect(
    settings, &screens::Settings::onFolderRemoveRequested,
    this, &PullDog::onFolderRemoveRequested
  );

  // connect destination path changed
  connect(
    settings, &screens::Settings::destinationPathChanged,
    this, &PullDog::destinationPathChanged
  );
}

/**
 * @brief Function helps to maintain history of progress
 * but not more than maxProgressHistory
 */
void PullDog::maintainProgressHistory() {
  // if progress list is less than maxProgressHistory
  if (pullInfo->getProgressList().size() < maxProgressHistory) {
    return;
  }

  // get the progress list
  auto list = pullInfo->getProgressList();

  // number of extra progress
  auto extraProgress = list.size() - maxProgressHistory;

  // if any finished progress found remove it reversed
  for (auto progress = list.rbegin(); progress != list.rend(); progress++) {
    if ((*progress)->isFinished() && extraProgress > 0) {
      pullInfo->removeProgress(*progress);
      extraProgress--;
    }

    if (extraProgress == 0) {
      break;
    }
  }
}

/**
 * @brief On Progress Changed
 */
void PullDog::onProgressChanged(models::Transfer transfer, double progress) {
  auto progressList = pullInfo->getProgressList();

  for (auto p : progressList) {
    if (p->getTransfer() == transfer) {
      p->setProgress(progress);
      return;
    }
  }
}

/**
 * @brief Add a File Transfer
 */
void PullDog::addTransfer(const models::Transfer &transfer) {
  auto progress = new components::Progress(transfer, this);
  removeTransfer(transfer);
  pullInfo->addProgress(progress);
}

/**
 * @brief Remove a File Transfer
 */
bool PullDog::removeTransfer(const models::Transfer &transfer) {
  bool found = false;

  for (auto progress : pullInfo->getProgressList()) {
    if (progress->getTransfer() == transfer) {
      pullInfo->removeProgress(progress);
      found = true;
    }
  }

  // if not found
  return found;
}

/**
 * @brief Set the Watch List
 */
void PullDog::addWatchPath(const QString &path) {
  settings->addWatchPath(path);
}

/**
 * @brief Remove watch List
 */
void PullDog::removeWatchPath(const QString &path) {
  settings->removeWatchPath(path);
}

/**
 * @brief Get the Watch List
 */
QStringList PullDog::getWatchList() const {
  return settings->getWatchList();
}

/**
 * @brief Set the Destination Root
 */
void PullDog::setDestinationRoot(const QString &destRoot) {
  settings->setDestinationRoot(destRoot);
}

/**
 * @brief Get the Destination Root
 */
QString PullDog::getDestinationRoot() const {
  return settings->getDestinationRoot();
}

/**
 * @brief Set max Progress History
 */
void PullDog::setMaxProgressHistory(int maxProgressHistory) {
  this->maxProgressHistory = maxProgressHistory;
}

/**
 * @brief Get max Progress History
 */
int PullDog::getMaxProgressHistory() const {
  return maxProgressHistory;
}

/**
 * @brief Handle End
 */
void PullDog::onCopyEnd(const models::Transfer &transfer) {
  for (auto progress : pullInfo->getProgressList()) {
    if (progress->getTransfer() == transfer) {
      progress->setProgress(100);
      maintainProgressHistory();
      return;
    }
  }
}
}  // namespace srilakshmikanthanp::pulldog::ui::gui::window
