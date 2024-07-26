#pragma once  // #include only once see https://en.wikipedia.org/wiki/Pragma_once

// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QObject>
#include <QWidget>
#include <QScrollArea>
#include <QScrollBar>
#include <QStackedLayout>

#include "ui/gui/components/progress/progress.hpp"

namespace srilakshmikanthanp::pulldog::ui::gui::screens {
/**
 * @brief Class shows Progress in Scroll Area
 */
class PullInfo : public QWidget {
 private:

  Q_DISABLE_COPY(PullInfo)

 private:

  Q_OBJECT

 private:

  QStackedLayout *stackLayout = new QStackedLayout();
  QScrollArea *scrollArea = new QScrollArea();
  QWidget *scrollAreaWidget = new QWidget();
  QLabel *noProgress = new QLabel();
  QVBoxLayout *scrollAreaLayout = new QVBoxLayout();
  QList<components::Progress *> progressList;

 private:
  void setUpLanguage();

 public:

  /**
   * @brief Construct a new Pull Info object
   *
   * @param parent
   */
  PullInfo(QWidget *parent = nullptr);

  /**
   * @brief Destroy the Pull Info object
   */
  ~PullInfo();

  /**
   * @brief Remove Progress
   *
   * @param progress
   */
  void removeProgress(components::Progress *progress);

  /**
   * @brief Add Progress
   *
   * @param progress
   */
  void addProgress(components::Progress *progress);

  /**
   * @brief Get Progress List
   *
   * @return QList<components::Progress *>
   */
  QList<components::Progress *> getProgressList() const;

  /**
   * @brief override paint event
   */
  void paintEvent(QPaintEvent *event) override;
};
}  // namespace srilakshmikanthanp::pulldog::gui::screens
