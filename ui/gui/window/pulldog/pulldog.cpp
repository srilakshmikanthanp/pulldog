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

  // add the failed info widget to the stacked widget
  stackedWidget->addWidget(failedInfo);

  // create the final layout
  auto layout = new QVBoxLayout();

  // add the stacked widget to the layout
  layout->addWidget(stackedWidget);

  // set the layout
  this->setLayout(layout);

  // connect removed
  connect(
    settings, &screens::Settings::onFolderRemoveRequested,
    this, &PullDog::onFolderRemoveRequested
  );

  // on settings clicked
  connect(
    titleBar, &components::TitleBar::settingsClicked,
    [this] { stackedWidget->setCurrentIndex(1); }
  );

  // on failed list clicked
  connect(
    titleBar, &components::TitleBar::failListClicked,
    [this] { stackedWidget->setCurrentIndex(2); }
  );

  // on close
  connect(
    settings, &screens::Settings::onCloseClicked,
    [this] { stackedWidget->setCurrentIndex(0); }
  );

  // on close
  connect(
    failedInfo, &screens::FailedInfo::onCloseClicked,
    [this] { stackedWidget->setCurrentIndex(0); }
  );

  // connect added
  connect(
    settings, &screens::Settings::onFolderAddRequested,
    this, &PullDog::onFolderAddRequested
  );

  // connect destination path changed
  connect(
    settings, &screens::Settings::destinationPathChanged,
    this, &PullDog::destinationPathChanged
  );
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
 * @brief set history hint
 */
void PullDog::setHistoryHint(qsizetype hint) {
  this->historyHint = hint;
}

/**
 * @brief get history hint
 */
qsizetype PullDog::getHistoryHint() const {
  return this->historyHint;
}

/**
 * @brief Add a File Transfer
 */
void PullDog::addTransfer(const models::Transfer &transfer) {
  auto progress = new components::Progress(transfer, this);
  this->removeTransfer(transfer);
  pullInfo->addProgress(progress);
  this->transfers[transfer] = progress;
}

/**
 * @brief On Progress Changed
 */
void PullDog::onProgressChanged(models::Transfer transfer, double progress) {
  if (this->transfers.contains(transfer)) {
    this->transfers[transfer]->setProgress(progress);
  }
}

/**
 * @brief Remove a File Transfer
 */
bool PullDog::removeTransfer(const models::Transfer &transfer) {
  if (!this->transfers.contains(transfer)) {
    return false;
  }

  auto progress = this->transfers[transfer];
  this->transfers.remove(transfer);
  pullInfo->removeProgress(progress);
  progress->deleteLater();

  return true;
}

/**
 * @brief Add a File Transfer
 */
void PullDog::addFailed(const models::Transfer &transfer) {
  using FailedTile = components::FailedTile;
  auto tile = new FailedTile(transfer, this);
  connect(tile, &FailedTile::onRetryRequested, this, &PullDog::onRetryRequested);
  failedInfo->addFailedTile(tile);
  this->failures[transfer] = tile;
}

/**
 * @brief Remove a File Transfer
 */
bool PullDog::removeFailed(const models::Transfer &transfer) {
  if (!this->failures.contains(transfer)) {
    return false;
  }

  auto tile = this->failures[transfer];
  this->failures.remove(transfer);
  failedInfo->removeFailedTile(tile);
  tile->deleteLater();

  return true;
}

/**
 * @brief Handle End
 */
void PullDog::onCopyEnd(const models::Transfer &transfer) {
  if (!this->transfers.contains(transfer)) {
    return;
  }

  auto progress = this->transfers[transfer];
  progress->setProgress(100);
  if (transfers.size() >= historyHint) {
    this->removeTransfer(transfer);
  }
}
}  // namespace srilakshmikanthanp::pulldog::ui::gui::window
