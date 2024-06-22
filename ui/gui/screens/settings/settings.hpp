#pragma once // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "ui/gui/components/folderlist/folderlist.hpp"
#include "ui/gui/components/pathpick/pathpick.hpp"

namespace srilakshmikanthanp::pulldog::ui::gui::screens {
/**
 * @brief Settings Screen
 */
class Settings : public QWidget {
 private:

  Q_DISABLE_COPY(Settings)

 private:

  Q_OBJECT

 private:

  components::FolderList *watchList = new components::FolderList();
  components::PathPick *pathPick = new components::PathPick();
  QVBoxLayout *layout = new QVBoxLayout();
  QLabel *pathPickLabel = new QLabel(tr("Destination Path"), this);
  QLabel *watchListLabel = new QLabel(tr("Watch List"), this);

 signals:
  /**
   * @brief signal to emit when the watchList is changed
   */
  void watchListChanged(QStringList watchList);

 signals:
  /**
   * @brief signal to emit when the progress is changed
   */
  void destinationPathChanged(QString path);

 public:

  /**
   * @brief Construct a new Settings object
   */
  Settings(QWidget *parent = nullptr);

  /**
   * @brief Destroy the Settings object
   */
  ~Settings() = default;

  /**
   * @brief set Watch List
   */
  void setWatchList(const QStringList &watchList);

  /**
   * @brief Get Watch List
   */
  QStringList getWatchList() const;

  /**
   * @brief Add Watch Path
   */
  void addWatchPath(const QString &path);

  /**
   * @brief Remove path
   */
  void removeWatchPath(const QString &path);

  /**
   * @brief Get Path
   */
  QString getDestinationRoot() const;

  /**
   * @brief set Path
   */
  void setDestinationRoot(const QString &path);
};
}  // namespace srilakshmikanthanp::pulldog::ui::gui::screens
