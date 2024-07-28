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
  // set the style
  settings->setIcon(QIcon(":/images/settings.png"));
  settings->setObjectName("Transparent");
  settings->setCursor(Qt::PointingHandCursor);

  // set the style
  failList->setIcon(QIcon(":/images/failed.png"));
  failList->setObjectName("Transparent");
  failList->setCursor(Qt::PointingHandCursor);

  // set the layout
  QHBoxLayout *layout = new QHBoxLayout();
  layout->addStretch();
  layout->addWidget(failList);
  layout->addWidget(settings);

  // set the layout
  this->setLayout(layout);

  // connect the signals
  connect(
    settings, &QPushButton::clicked,
    this, &TitleBar::settingsClicked
  );

  connect(
    failList, &QPushButton::clicked,
    this, &TitleBar::failListClicked
  );

  // set up language
  this->setupLanguage();
}

/**
 * @brief set up Language
 */
void TitleBar::setupLanguage() {

}
}
