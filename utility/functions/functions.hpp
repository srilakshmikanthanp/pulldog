#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QStyleHints>
#include <QGuiApplication>
#include <QStyleHints>
#include <QWidget>
#include <QWidget>
#include <QFile>
#include <QRegularExpression>

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#include <dwmapi.h>
#undef NOMINMAX
#endif

#include <QString>
#include <stdexcept>

namespace srilakshmikanthanp::pulldog::utility {
/**
 * @brief compare file by partial file hash
 */
bool isUptoDateByPartialHash(const QString &src, const QString &dest, const int chunks = 50, const int hashSize = 1024);

/**
 * @brief compare file by file metadata
 */
bool isUptoDateByMetaData(const QString &src, const QString &dest);

/**
 * @brief compare file with all the methods may not be accurate
 * but it is fast and efficient
 */
bool isUptoDate(const QString &src, const QString &dest);

/**
 * @brief get the file name from the handle
 */
#ifdef _WIN32
QString getFileNameFromHandle(HANDLE hFile);
#endif

/**
 * @brief Function used to set the Os level attributes for the widget
 */
void setPlatformAttributes(QWidget *widget);
}  // namespace srilakshmikanthanp::utility
