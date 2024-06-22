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

  // set the current index to 0
  stackedWidget->setCurrentIndex(0);

  // create the final layout
  auto layout = new QVBoxLayout();

  // add the stacked widget to the layout
  layout->addWidget(stackedWidget);

  // set the layout
  setLayout(containerLayout);
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
void PullDog::setWatchList(const QStringList &watchList) {
  settings->setWatchList(watchList);
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
