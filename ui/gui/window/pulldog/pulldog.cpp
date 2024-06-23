// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "pulldog.hpp"

namespace srilakshmikanthanp::pulldog::ui::gui::window {
/**
 * @brief Construct a new Pull Dog object
 */
PullDog::PullDog(Controller *controller, QWidget *parent)
    : QWidget(parent), controller(controller) {
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
    settings, &screens::Settings::onFolderAdded,
    this, &PullDog::onFolderAdded
  );

  // connect removed
  connect(
    settings, &screens::Settings::onFolderRemoved,
    this, &PullDog::onFolderRemoved
  );

  // connect destination path changed
  connect(
    settings, &screens::Settings::destinationPathChanged,
    this, &PullDog::destinationPathChanged
  );
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
  pullInfo->addProgress(new components::Progress(transfer, this));
}

/**
 * @brief Remove a File Transfer
 */
bool PullDog::removeTransfer(const models::Transfer &transfer) {
  for (auto progress : pullInfo->getProgressList()) {
    if (progress->getTransfer() == transfer) {
      pullInfo->removeProgress(progress);
      return true;
    }
  }

  // if not found
  return false;
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
}  // namespace srilakshmikanthanp::pulldog::ui::gui::window
