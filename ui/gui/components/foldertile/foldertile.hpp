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

  QLabel *label             = new QLabel();
  QPushButton *removeButton = new QPushButton();

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
} // namespace srilakshmikanthanp::pulldog::ui::gui::components
