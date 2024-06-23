// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "pathpick.hpp"

namespace srilakshmikanthanp::pulldog::ui::gui::components {
/**
 * @brief Construct a new Progress object
 *
 * @param parent
 */
PathPick::PathPick(QWidget *parent) : QWidget(parent) {
  // create layout
  auto layout = new QHBoxLayout();

  // set up language
  this->setupLanguage();

  // add widgets to layout
  layout->addWidget(label);
  layout->addStretch();
  layout->addWidget(button);

  // create label
  label->setText(path);

  // connect button
  connect(
    button, &QPushButton::clicked,
    this, &PathPick::onBrowse
  );

  // set layout
  setLayout(layout);
}

/**
 * @brief Set up the language
 */
void PathPick::setupLanguage() {
  this->label->setText(tr("Choose Path"));
  this->button->setText(tr("Browse"));
}

/**
 * @brief on browse button clicked
 */
void PathPick::onBrowse() {
  // open file dialog
  auto path = QFileDialog::getExistingDirectory(this, tr("Choose Path to save Files"), this->path);

  // check path
  if (path.isEmpty()) {
    return;
  }

  // set path
  label->setText(path);

  // emit signal
  emit pathChanged(path);
}

/**
 * @brief Get the path
 */
QString PathPick::getPath() const {
  return path;
}

/**
 * @brief set the path
 */
void PathPick::setPath(QString path) {
  label->setText(this->path = path);
}
}  // namespace srilakshmikanthanp::pulldog::ui::gui::components
