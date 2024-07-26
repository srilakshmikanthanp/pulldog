#pragma once  // #include only once see https://en.wikipedia.org/wiki/Pragma_once

// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>
#include <QPushButton>

namespace srilakshmikanthanp::pulldog::ui::gui::components {
/**
 * @brief TitleBar
 */
class TitleBar : public QWidget {
 private:

  Q_DISABLE_COPY(TitleBar)

 private:

  Q_OBJECT

 private:

  QLabel *titleLabel = new QLabel();
  QPushButton *settings  = new QPushButton();
  QPushButton *failList  = new QPushButton();

 signals:
  /**
   * @brief signal to emit when the settings button is clicked
   */
  void settingsClicked();

  /**
   * @brief signal to emit when the faillist button is clicked
   */
  void failListClicked();

 public:

  /**
   * @brief Construct a new TitleBar object
   *
   * @param parent
   */
  TitleBar(QWidget *parent = nullptr);

  /**
   * @brief Destroy the TitleBar object
   */
  ~TitleBar() = default;

  /**
   * @brief set up Language
   */
  void setupLanguage();
};
}  // namespace srilakshmikanthanp::pulldog::ui::gui::components
