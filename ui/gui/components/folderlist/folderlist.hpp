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

namespace srilakshmikanthanp::pulldog::ui::gui::components {
/**
 * @brief Watch Tile component is a tile to watch a directory
 */
class FolderTile : public QWidget {
 private:

  Q_DISABLE_COPY(FolderTile)

 private:

  Q_OBJECT

 private:

  QLabel *label             = new QLabel(this);
  QPushButton *removeButton = new QPushButton(this);

 private:

  QString path;

 private:

  /**
   * @brief Set up the language
   */
  void setupLanguage();

  /**
   * @brief on remove button clicked
   */
  void onRemove();

 signals:
  /**
   * @brief signal to emit when the watch tile is removed
   */
  void removeWatchTile(QString path);

 public:

  /**
   * @brief Construct a new Watch Tile object
   *
   * @param path
   * @param parent
   */
  FolderTile(const QString &path, QWidget *parent = nullptr);

  /**
   * @brief Destroy the Watch Tile object
   */
  ~FolderTile() = default;

  /**
   * @brief get the path
   *
   * @return QString
   */
  QString getPath() const;
};

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

  QPushButton *addButton   = new QPushButton(this);
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
   * @brief signal to emit when the watchlist is changed
   */
  void folderListChanged(QStringList watchlist);

 public:

  /**
   * @brief Construct a new FolderList object
   *
   * @param watchlist
   * @param parent
   */
  FolderList(QWidget *parent = nullptr);

  /**
   * @brief Destroy the FolderList object
   */
  ~FolderList();

  /**
   * @brief set the watchlist
   *
   * @param watchlist
   */
  void setFolderList(QStringList watchlist);

  /**
   * @brief Add a directory to watchlist
   */
  void addPath(const QString &path);

  /**
   * @brief Remove a directory from watchlist
   */
  void removePath(const QString &path);

  /**
   * @brief get the watchlist
   *
   * @return QStringList
   */
  QStringList getFolderList() const;
};
}  // namespace srilakshmikanthanp::pulldog::ui::gui::components
