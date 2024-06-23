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
  // style
  titleLabel->setObjectName("Title");

  // set the layout
  QHBoxLayout *layout = new QHBoxLayout();
  layout->addWidget(titleLabel);
  layout->addStretch();
  layout->addWidget(settings);

  // set the style
  settings->setIcon(QIcon(":/images/settings.png"));
  settings->setObjectName("Transparent");
  settings->setCursor(Qt::PointingHandCursor);

  // connect the signals
  connect(
    settings, &QPushButton::clicked,
    this, &TitleBar::settingsClicked
  );

  // set up language
  this->setupLanguage();

  // set the layout
  this->setLayout(layout);
}

/**
 * @brief set up Language
 */
void TitleBar::setupLanguage() {
  titleLabel->setText(tr("Pulldog"));
}
}
