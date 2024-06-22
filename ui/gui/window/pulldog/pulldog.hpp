// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QMainWindow>
#include <QVBoxLayout>
#include <QStackedWidget>

#include "controller/controller.hpp"
#include "models/transfer/transfer.hpp"
#include "ui/gui/components/titlebar/titlebar.hpp"
#include "ui/gui/screens/pullinfo/pullinfo.hpp"
#include "ui/gui/screens/settings/settings.hpp"

namespace srilakshmikanthanp::pulldog::ui::gui::window {
/**
 * @brief PullDog
 */
class PullDog : public QWidget {
 private:

  Q_DISABLE_COPY(PullDog)

 private:

  Q_OBJECT

 private:
  components::TitleBar *titleBar = new components::TitleBar();
  screens::PullInfo *pullInfo = new screens::PullInfo();
  QWidget *container = new QWidget();
  screens::Settings *settings = new screens::Settings();
  QStackedWidget *stackedWidget = new QStackedWidget();
  Controller *controller;

 signals:
  /**
   * @brief On Destination Root Changed
   */
  void destinationPathChanged(QString destRoot);

  /**
   * @brief On Watch List Changed
   */
  void watchListChanged(QStringList watchList);

 public:
  /**
   * @brief Construct a new Pull Dog object
   */
  PullDog(Controller *controller, QWidget *parent = nullptr);

  /**
   * @brief Destroy the Pull Dog object
   */
  ~PullDog() = default;

  /**
   * @brief Add a File Transfer
   */
  void addTransfer(const models::Transfer &transfer);

  /**
   * @brief Remove a File Transfer
   */
  bool removeTransfer(const models::Transfer &transfer);

  /**
   * @brief Set the Watch List
   */
  void setWatchList(const QStringList &watchList);

  /**
   * @brief Get the Watch List
   */
  QStringList getWatchList() const;

  /**
   * @brief Set the Destination Root
   */
  void setDestinationRoot(const QString &destRoot);

  /**
   * @brief Get the Destination Root
   */
  QString getDestinationRoot() const;
};
}  // namespace srilakshmikanthanp::pulldog::ui::gui::window
