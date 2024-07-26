#pragma once  // #include only once see https://en.wikipedia.org/wiki/Pragma_once

// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QScrollArea>
#include <QScrollBar>
#include <QStackedLayout>

#include "ui/gui/components/failedtile/failedtile.hpp"
#include "models/transfer/transfer.hpp"

namespace srilakshmikanthanp::pulldog::ui::gui::screens {
/**
 * @brief Class shows Progress in Scroll Area
 */
class FailedInfo : public QWidget {
 private:

  Q_DISABLE_COPY(FailedInfo)

 private:

  Q_OBJECT

 private:

  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  QStackedLayout *stackLayout = new QStackedLayout();
  QScrollArea *scrollArea = new QScrollArea();
  QWidget *scrollAreaWidget = new QWidget();
  QLabel *noFailures = new QLabel();
  QVBoxLayout *scrollAreaLayout = new QVBoxLayout();
  QList<components::FailedTile*> failedList;
  QPushButton *close = new QPushButton();

 signals:
  void onRetryRequested(models::Transfer transfer);

 signals:
  void onCloseClicked();

 private:
  void setUpLanguage();

 public:

  /**
   * @brief Construct a new Pull Info object
   *
   * @param parent
   */
  FailedInfo(QWidget *parent = nullptr);

  /**
   * @brief Destroy the Pull Info object
   */
  ~FailedInfo();

  /**
   * @brief Remove Progress
   *
   * @param progress
   */
  void removeFailedTile(components::FailedTile *transfer);

  /**
   * @brief Add Progress
   *
   * @param progress
   */
  void addFailedTile(components::FailedTile* transfer);

  /**
   * @brief Get Progress List
   *
   * @return QList<components::Progress *>
   */
  QList<components::FailedTile *> getFailedTiles() const;

  /**
   * @brief override paint event
   */
  void paintEvent(QPaintEvent *event) override;
};
}  // namespace srilakshmikanthanp::pulldog::gui::screens
