// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "folderlist.hpp"

namespace srilakshmikanthanp::pulldog::ui::gui::components {
/**
 * @brief Construct a new Watch Tile object
 */
FolderTile::FolderTile(const QString &path, QWidget* parent) : path(path), QWidget(parent) {
  // set up the layout
  auto layout = new QHBoxLayout();

  // set up the language
  this->setupLanguage();

  layout->addWidget(label);
  layout->addStretch();
  layout->addWidget(removeButton);

  // set up the connections
  connect(
    removeButton, &QPushButton::clicked,
    this, &FolderTile::onRemove
  );

  setLayout(layout);
}

/**
 * @brief Set up the language
 */
void FolderTile::setupLanguage() {
  label->setText(path);
  removeButton->setText(tr("-"));
}

/**
 * @brief on remove button clicked
 */
void FolderTile::onRemove() {
  emit removeWatchTile(path);
}

/**
 * @brief Get the path
 */
QString FolderTile::getPath() const {
  return path;
}

/**
 * @brief Construct a new Watch List object
 */
FolderList::FolderList(QWidget* parent) : QWidget(parent) {
  // set up the layout
  auto layout = new QVBoxLayout(this);

  // set up the language
  this->setupLanguage();

  // set up the connections
  connect(
    addButton, &QPushButton::clicked,
    this, &FolderList::onAdd
  );

  auto btnLayout = new QHBoxLayout();
  btnLayout->addWidget(addButton);

  // set button to end
  btnLayout->insertStretch(0);

  // add label layout to the layout
  layout->addLayout(tilesLayout);
  layout->addLayout(btnLayout);
}

/**
 * @brief Destroy the Watch List object
 */
FolderList::~FolderList() {
  for (auto tile : tiles) {
    tile->deleteLater();
  }
}

/**
 * @brief Set up the language
 */
void FolderList::setupLanguage() {
  addButton->setText(tr("Add"));
}

/**
 * @brief on Add
 */
void FolderList::onAdd() {
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
void FolderList::setFolderList(QStringList watchList) {
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
void FolderList::addPath(const QString &path) {
  auto tile = new FolderTile(path, this);
  watchList.append(path);
  tilesLayout->addWidget(tile);
  tiles.append(tile);

  connect(
    tile, &FolderTile::removeWatchTile,
    this, &FolderList::removePath
  );

  emit folderListChanged(watchList);
}

/**
 * @brief Remove a directory from watchList
 */
void FolderList::removePath(const QString &path) {
  watchList.removeOne(path);

  for (auto tile : tiles) {
    if (tile->getPath() == path) {
      tile->deleteLater();
      tiles.removeOne(tile);
      break;
    }
  }

  emit folderListChanged(
    watchList
  );
}

/**
 * @brief Get the watchList
 */
QStringList FolderList::getFolderList() const {
  return watchList;
}
} // namespace srilakshmikanthanp::pulldog::ui::gui::components
