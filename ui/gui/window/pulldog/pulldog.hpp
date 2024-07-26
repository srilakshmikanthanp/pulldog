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
#include "ui/gui/screens/failedinfo/failedinfo.hpp"
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
  screens::FailedInfo *failedInfo = new screens::FailedInfo();
  screens::Settings *settings = new screens::Settings();
  QStackedWidget *stackedWidget = new QStackedWidget();
  Controller *controller;

 private:
  int maxProgressHistory = 10;

 signals:
  /**
   * @brief On Destination Root Changed
   */
  void destinationPathChanged(QString destRoot);

 signals:
  /**
   * @brief signal to emit folder removed
   */
  void onFolderRemoveRequested(const QString &path);

 signals:
  /**
   * @brief Retry Requested
   */
  void onRetryRequested(models::Transfer transfer);

 private:
  /**
   * @brief Function helps to maintain history of progress
   * but not more than maxProgressHistory
   */
  void maintainProgressHistory();

 signals:
  /**
   * @brief signal to emit folder added
   */
  void onFolderAddRequested(const QString &path);

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
   * @brief Add a File Transfer
   */
  void addFailed(const models::Transfer &transfer);

  /**
   * @brief Remove a File Transfer
   */
  bool removeFailed(const models::Transfer &transfer);

  /**
   * @brief Set the Watch List
   */
  void addWatchPath(const QString &path);

  /**
   * @brief Remove watch List
   */
  void removeWatchPath(const QString &path);

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

  /**
   * @brief Set max Progress History
   */
  void setMaxProgressHistory(int maxProgressHistory);

  /**
   * @brief Get max Progress History
   */
  int getMaxProgressHistory() const;

  /**
   * @brief Handle End
   */
  void onCopyEnd(const models::Transfer &transfer);

  /**
   * @brief On Progress Changed
   */
  void onProgressChanged(models::Transfer transfer, double progress);
};
}  // namespace srilakshmikanthanp::pulldog::ui::gui::window
