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
  // add pathPickLabel
  this->layout->addWidget(pathPickLabel);

  // add pathPick
  this->layout->addWidget(pathPick);

  // add watchListLabel
  this->layout->addWidget(watchListLabel);

  // add watchList
  this->layout->addWidget(watchList);

  // connect folderListChanged signal
  connect(
    watchList, &components::FolderList::folderListChanged,
    this, &Settings::watchListChanged
  );

  // connect pathChanged signal
  connect(
    pathPick, &components::PathPick::pathChanged,
    this, &Settings::destinationPathChanged
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
