#pragma once  // header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QObject>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QGraphicsDropShadowEffect>

#include "ui/gui/components/foldertile/foldertile.hpp"

namespace srilakshmikanthanp::pulldog::ui::gui::components {
/**
 * @brief FolderList component is a list of directories to watch
 * for changes
 */
class FolderList : public QWidget {
 private:

  Q_DISABLE_COPY(FolderList)

 private:

  Q_OBJECT

 private:

  QPushButton *addButton   = new QPushButton();
  QVBoxLayout *tilesLayout = new QVBoxLayout();
  QList<FolderTile *> tiles;

 private:

  QStringList watchList;

 private:

  /**
   * @brief Set up the language
   */
  void setupLanguage();

  /**
   * @brief on add button clicked
   */
  void onAdd();

 signals:
  /**
   * @brief signal to emit folder removed
   */
  void onFolderRemoveRequested(const QString &path);

 signals:
  /**
   * @brief signal to emit folder added
   */
  void onFolderAddRequested(const QString &path);

 public:

  /**
   * @brief Construct a new FolderList object
   *
   * @param watchList
   * @param parent
   */
  FolderList(QWidget *parent = nullptr);

  /**
   * @brief Destroy the FolderList object
   */
  ~FolderList();

  /**
   * @brief set the watchList
   *
   * @param watchList
   */
  void setFolderList(QStringList watchList);

  /**
   * @brief Add a directory to watchList
   */
  void addPath(const QString &path);

  /**
   * @brief Remove a directory from watchList
   */
  void removePath(const QString &path);

  /**
   * @brief get the watchList
   *
   * @return QStringList
   */
  QStringList getFolderList() const;
};
}  // namespace srilakshmikanthanp::pulldog::ui::gui::components
