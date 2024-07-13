// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "settings.hpp"

namespace srilakshmikanthanp::pulldog::ui::gui::screens {
/**
 * @brief Construct a new Settings object
 *
 * @param parent
 */
Settings::Settings(QWidget *parent) : QWidget(parent) {
  // add Close Button
  this->layout->addWidget(close);

  // close icon
  close->setIcon(QIcon(":/images/back.png"));
  close->setObjectName("Transparent");
  close->setCursor(Qt::PointingHandCursor);

  // move close button to the right
  this->layout->setAlignment(close, Qt::AlignLeft);

  // set spacing
  this->layout->addSpacing(20);

  // add pathPick
  this->layout->addWidget(pathPickLabel);
  this->layout->addWidget(pathPick);

  // add watchList
  this->layout->addWidget(watchListLabel);
  this->layout->addWidget(watchList);

  // alignment
  this->layout->setAlignment(pathPickLabel, Qt::AlignTop);

  // alignment
  this->layout->insertStretch( -1, 1 );

  // style
  pathPickLabel->setObjectName("Heading");
  watchListLabel->setObjectName("Heading");

  // connect folderListChanged signal
  connect(
    watchList, &components::FolderList::onFolderAddRequested,
    this, &Settings::onFolderAddRequested
  );

  // connect folderListChanged signal
  connect(
    watchList, &components::FolderList::onFolderRemoveRequested,
    this, &Settings::onFolderRemoveRequested
  );

  // connect pathChanged signal
  connect(
    pathPick, &components::PathPick::pathChanged,
    this, &Settings::destinationPathChanged
  );

  // connect close button
  connect(
    close, &QPushButton::clicked,
    this, &Settings::onCloseClicked
  );

  // set layout
  this->setLayout(layout);
}

/**
 * @brief set Watch List
 *
 * @param watchList
 */
void Settings::setWatchList(const QStringList &watchList) {
  this->watchList->setFolderList(watchList);
}

/**
 * @brief Get Watch List
 *
 * @return QStringList
 */
QStringList Settings::getWatchList() const {
  return this->watchList->getFolderList();
}

/**
 * @brief Add Watch Path
 *
 * @param path
 */
void Settings::addWatchPath(const QString &path) {
  this->watchList->addPath(path);
}

/**
 * @brief Remove path
 *
 * @param path
 */
void Settings::removeWatchPath(const QString &path) {
  this->watchList->removePath(path);
}

/**
 * @brief Get Path
 *
 * @return QString
 */
QString Settings::getDestinationRoot() const {
  return this->pathPick->getPath();
}

/**
 * @brief set Path
 *
 * @param path
 */
void Settings::setDestinationRoot(const QString &path) {
  this->pathPick->setPath(path);
}
} // namespace srilakshmikanthanp::pulldog::ui::gui::screens
