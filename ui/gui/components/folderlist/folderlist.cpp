// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "folderlist.hpp"

namespace srilakshmikanthanp::pulldog::ui::gui::components {
/**
 * @brief Construct a new Watch List object
 */
FolderList::FolderList(QWidget* parent) : QWidget(parent) {
  // set up the layout
  auto layout = new QVBoxLayout();

  // set up the language
  this->setupLanguage();

  // set up the connections
  connect(
    addButton, &QPushButton::clicked,
    this, &FolderList::onAdd
  );

  // set icon
  addButton->setIcon(QIcon(":/images/plus.png"));
  addButton->setObjectName("Transparent");
  addButton->setCursor(Qt::PointingHandCursor);
  addButton->setIconSize(QSize(24, 24));

  // add label layout to the layout
  layout->addLayout(tilesLayout);
  layout->addWidget(addButton);

  // center align button
  layout->setAlignment(addButton, Qt::AlignCenter);

  // set the layout
  this->setLayout(layout);
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
  addButton->setToolTip(tr("Add a directory to watch"));
}

/**
 * @brief on Add
 */
void FolderList::onAdd() {
  auto path = QFileDialog::getExistingDirectory(
    this, tr("Select a directory"), QDir::homePath()
  );

  path = QDir::cleanPath(path);

  if (path.isEmpty()) {
    return;
  }

  // emit the signal
  emit onFolderAddRequested(path);
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

  QLayoutItem *item = nullptr;

  while (item = tilesLayout->takeAt(0)) {
    delete item->widget();
  }

  // add the tiles
  for (auto path : watchList) {
    this->addPath(path);
  }
}

/**
 * @brief Add a directory to watchList
 */
void FolderList::addPath(const QString &path) {
  auto tile = new FolderTile(path);
  watchList.append(path);
  tilesLayout->addWidget(tile);
  tiles.append(tile);

  connect(
    tile, &FolderTile::removeWatchTile,
    this, &FolderList::onFolderRemoveRequested
  );
}

/**
 * @brief Remove a directory from watchList
 */
void FolderList::removePath(const QString &path) {
  watchList.removeOne(path);

  for (auto tile : tiles) {
    if (tile->getPath() != path) {
      continue;
    }

    disconnect(
      tile, &FolderTile::removeWatchTile,
      this, &FolderList::onFolderRemoveRequested
    );

    disconnect(
      tile, &FolderTile::removeWatchTile,
      this, &FolderList::removePath
    );

    tilesLayout->removeWidget(tile);
    tile->deleteLater();
    tiles.removeOne(tile);

    break;
  }
}

/**
 * @brief Get the watchList
 */
QStringList FolderList::getFolderList() const {
  return watchList;
}
} // namespace srilakshmikanthanp::pulldog::ui::gui::components
