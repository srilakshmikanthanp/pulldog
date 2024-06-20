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
class WatchTile : public QWidget {
 private:

  Q_DISABLE_COPY(WatchTile)

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
  WatchTile(const QString &path, QWidget *parent = nullptr);

  /**
   * @brief Destroy the Watch Tile object
   */
  ~WatchTile() = default;

  /**
   * @brief get the path
   *
   * @return QString
   */
  QString getPath() const;
};

/**
 * @brief WatchList component is a list of directories to watch
 * for changes
 */
class WatchList : public QWidget {
 private:

  Q_DISABLE_COPY(WatchList)

 private:

  Q_OBJECT

 private:

  QPushButton *addButton   = new QPushButton(this);
  QLabel *label            = new QLabel(this);
  QVBoxLayout *tilesLayout = new QVBoxLayout();
  QList<WatchTile *> tiles;

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
  void watchListChanged(QStringList watchlist);

 public:

  /**
   * @brief Construct a new WatchList object
   *
   * @param watchlist
   * @param parent
   */
  WatchList(QStringList watchlist, QWidget *parent = nullptr);

  /**
   * @brief Destroy the WatchList object
   */
  ~WatchList();

  /**
   * @brief set the watchlist
   *
   * @param watchlist
   */
  void setWatchList(QStringList watchlist);

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
  QStringList getWatchList() const;
};
}  // namespace srilakshmikanthanp::pulldog::ui::gui::components
