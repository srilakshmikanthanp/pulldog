// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "titlebar.hpp"

namespace srilakshmikanthanp::pulldog::ui::gui::components {
/**
 * @brief Construct a new TitleBar object
 */
TitleBar::TitleBar(QWidget *parent) : QWidget(parent) {
  // set the title
  titleLabel->setText("PullDog");

  // set the layout
  QHBoxLayout *layout = new QHBoxLayout();
  layout->addWidget(titleLabel);
  layout->addStretch();
  layout->addWidget(settings);

  // set the style
  settings->setText("Settings");

  // connect the signals
  connect(
    settings, &QPushButton::clicked,
    this, &TitleBar::settingsClicked
  );

  // set the layout
  this->setLayout(layout);
}
}
