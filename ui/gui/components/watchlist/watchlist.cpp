// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "watchlist.hpp"

namespace srilakshmikanthanp::pulldog::ui::gui::components {
/**
 * @brief Construct a new Watch Tile object
 */
WatchTile::WatchTile(const QString &path, QWidget* parent) : path(path), QWidget(parent) {
  // set up the layout
  auto layout = new QHBoxLayout(this);

  // set up the language
  this->setupLanguage();

  layout->addWidget(label);
  layout->addWidget(removeButton);

  // set up the connections
  connect(removeButton, &QPushButton::clicked, this, &WatchTile::onRemove);
}

/**
 * @brief Set up the language
 */
void WatchTile::setupLanguage() {
  label->setText(path);
  removeButton->setText(tr("Remove"));
}

/**
 * @brief on remove button clicked
 */
void WatchTile::onRemove() {
  emit removeWatchTile(path);
}

/**
 * @brief Get the path
 */
QString WatchTile::getPath() const {
  return path;
}

/**
 * @brief Construct a new Watch List object
 */
WatchList::WatchList(QStringList watchList, QWidget* parent) : QWidget(parent) {
  // set up the layout
  auto layout = new QVBoxLayout(this);

  // set up the language
  this->setupLanguage();

  // set up the connections
  connect(addButton, &QPushButton::clicked, this, &WatchList::onAdd);

  // label layout
  auto labelLayout = new QHBoxLayout();
  auto btnLayout = new QHBoxLayout();

  // add label to the layout
  labelLayout->addWidget(label);
  btnLayout->addWidget(addButton);

  // add label layout to the layout
  layout->addLayout(labelLayout);
  layout->addLayout(tilesLayout);
  layout->addLayout(btnLayout);

  // set up the watchList
  this->setWatchList(watchList);
}

/**
 * @brief Destroy the Watch List object
 */
WatchList::~WatchList() {
  for (auto tile : tiles) {
    tile->deleteLater();
  }
}

/**
 * @brief Set up the language
 */
void WatchList::setupLanguage() {
  label->setText(tr("WatchList"));
  addButton->setText(tr("Add"));
}

/**
 * @brief on Add
 */
void WatchList::onAdd() {
  auto path = QFileDialog::getExistingDirectory(
    this, tr("Select a directory"), QDir::homePath()
  );

  if (path.isEmpty()) {
    return;
  }

  this->addPath(path);
}

/**
 * @brief set the watchList
 */
void WatchList::setWatchList(QStringList watchList) {
  // clear the tiles
  for (auto tile : tiles) {
    tile->deleteLater();
  }

  tiles.clear();

  // add the tiles
  for (auto path : watchList) {
    this->addPath(path);
  }
}

/**
 * @brief Add a directory to watchList
 */
void WatchList::addPath(const QString &path) {
  auto tile = new WatchTile(path, this);
  watchList.append(path);
  tilesLayout->addWidget(tile);
  tiles.append(tile);

  connect(
    tile, &WatchTile::removeWatchTile,
    this, &WatchList::removePath
  );

  emit watchListChanged(watchList);
}

/**
 * @brief Remove a directory from watchList
 */
void WatchList::removePath(const QString &path) {
  watchList.removeOne(path);

  for (auto tile : tiles) {
    if (tile->getPath() == path) {
      tile->deleteLater();
      tiles.removeOne(tile);
      break;
    }
  }

  emit watchListChanged(
    watchList
  );
}

/**
 * @brief Get the watchList
 */
QStringList WatchList::getWatchList() const {
  return watchList;
}
} // namespace srilakshmikanthanp::pulldog::ui::gui::components
