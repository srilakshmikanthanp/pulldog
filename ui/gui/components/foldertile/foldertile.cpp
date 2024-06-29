// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "foldertile.hpp"

namespace srilakshmikanthanp::pulldog::ui::gui::components {
/**
 * @brief Construct a new Watch Tile object
 */
FolderTile::FolderTile(const QString &path, QWidget* parent) : path(path), QWidget(parent) {
  // set up the layout
  auto layout = new QHBoxLayout();

  // set icon
  removeButton->setIcon(QIcon(":/images/remove.png"));
  removeButton->setObjectName("Transparent");
  removeButton->setCursor(Qt::PointingHandCursor);

  // set up the connections
  connect(
    removeButton, &QPushButton::clicked,
    this, &FolderTile::onRemove
  );

  // set up the language
  this->setupLanguage();

  layout->addWidget(label);
  layout->addWidget(removeButton);

  // align the items
  layout->setAlignment(label, Qt::AlignLeft);
  layout->setAlignment(removeButton, Qt::AlignRight);

  // set the layout
  layout->setSpacing(0);
  layout->setContentsMargins(0, 0, 0, 0);

  // set the layout
  this->setLayout(layout);
}

/**
 * @brief Set up the language
 */
void FolderTile::setupLanguage() {
  label->setText(path);
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
} // namespace srilakshmikanthanp::pulldog::ui::gui::components
