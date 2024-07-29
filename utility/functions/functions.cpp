// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "functions.hpp"

namespace srilakshmikanthanp::pulldog::utility {
/**
 * @brief compare file by partial file hash
 */
bool isUptoDateByPartialHash(const QString &src, const QString &dest, const int chunks, const int hashSize) {
  // open the file
  QFile srcFile(src), destFile(dest);

  // check if the file is open
  if (!srcFile.open(QIODevice::ReadOnly) || !destFile.open(QIODevice::ReadOnly)) {
    return false;
  }

  // get the file size
  auto destSize = destFile.size();
  auto srcSize = srcFile.size();

  // check if the file size is same
  if (srcSize != destSize) {
    return false;
  }

  // if file size less than hash size compare the file
  if (srcSize < hashSize) {
    return srcFile.readAll() == destFile.readAll();
  }

  // get first chunk of the file
  auto destData = destFile.read(hashSize);
  auto srcData = srcFile.read(hashSize);

  // check if the first chunk is same
  if (srcData != destData) {
    return false;
  }

  // seek the last chunk of the file
  if(
    !destFile.seek(destSize - hashSize) ||
    !srcFile.seek(srcSize - hashSize)
  ) {
    return false;
  }

  // get the last chunk of the file
  destData = destFile.read(hashSize);
  srcData = srcFile.read(hashSize);

  // check if the last chunk is same
  if (srcData != destData) {
    return false;
  }

  // random device to get random number
  std::uniform_int_distribution<qint64> dist(0, srcSize - hashSize);
  std::random_device rd;

  // loop through the chunks
  for (int i = 0; i < chunks; i++) {
    // seek the random chunk of the file
    auto offset = dist(rd);

    if(!destFile.seek(offset) || !srcFile.seek(offset)) {
      return false;
    }

    // get the random chunk of the file
    destData = destFile.read(hashSize);
    srcData = srcFile.read(hashSize);

    // check if the random chunk is same
    if (srcData != destData) {
      return false;
    }
  }

  // return true if all the above conditions are false
  return true;
}

/**
 * @brief compare file by file metadata
 */
bool isUptoDateByMetaData(const QString &src, const QString &dest) {
  // get the file info
  QFileInfo srcInfo(src), destInfo(dest);

  // check if the file exists
  if (!srcInfo.exists() || !destInfo.exists()) {
    return false;
  }

  // check if the file is same
  if (srcInfo.size() != destInfo.size()) {
    return false;
  }

  // check if the file is same
  if (destInfo.created() < srcInfo.lastModified()) {
    return false;
  }

  // return true if all the above conditions are false
  return true;
}

/**
 * @brief compare file with all the methods may not be accurate
 * but it is fast and efficient
 */
bool isUptoDate(const QString &src, const QString &dest) {
  // check if the file is same by partial hash
  if (!isUptoDateByPartialHash(src, dest)) {
    return false;
  }

  // check if the file is same by metadata
  if (!isUptoDateByMetaData(src, dest)) {
    return false;
  }

  // if all the above conditions are false
  return true;
}

/**
 * @brief get the file name from the handle
 */
#ifdef _WIN32
QString getFileNameFromHandle(HANDLE hFile) {
  auto buffer = std::make_unique<WCHAR[]>(MAX_PATH);
  auto size = GetFinalPathNameByHandleW(
    hFile, buffer.get(), MAX_PATH, VOLUME_NAME_DOS | FILE_NAME_OPENED
  );

  if(size == 0) {
    auto msg = QString("Error: %1").arg(GetLastError())
    throw std::runtime_error(msg.toStdString());
  }

  // We need to consistant path name
  auto file = QString::fromWCharArray(buffer.get(), size).toStdString();
  auto path = std::filesystem::canonical(file).string();

  // return filename
  return QString::fromStdString(path);
}
#endif

/**
 * @brief Function used to set the Os level attributes for the widget
 */
void setPlatformAttributes(QWidget *widget) {
#ifdef _WIN32 // if it it windows platform set title bar to preferred theme
  // cast the winId to HWND
  BOOL isDark = QGuiApplication::styleHints()->colorScheme() == Qt::ColorScheme::Dark;
  HWND hwnd = reinterpret_cast<HWND>(widget->winId());

  // set the title bar to preferred theme
  auto result = DwmSetWindowAttribute(
    hwnd,
    DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE,
    &isDark,
    sizeof(isDark)
  );
#endif
}

/**
 * @brief Function used to chech the two file are same or not
 * using file id on windows
 */
bool isSameFile(const QString &left, const QString &right) {
#ifdef _WIN32
  // get the file info
  QFileInfo leftInfo(left), rightInfo(right);

  // check if the file exists
  if (!leftInfo.exists() || !rightInfo.exists()) {
    return false;
  }

  // get the file handle
  auto leftHandle = CreateFileW(
    left.toStdWString().c_str(),
    GENERIC_READ,
    FILE_SHARE_READ,
    nullptr,
    OPEN_EXISTING,
    FILE_ATTRIBUTE_NORMAL,
    nullptr
  );

  auto rightHandle = CreateFileW(
    right.toStdWString().c_str(),
    GENERIC_READ,
    FILE_SHARE_READ,
    nullptr,
    OPEN_EXISTING,
    FILE_ATTRIBUTE_NORMAL,
    nullptr
  );

  // check if the file handle is valid
  if (
    rightHandle == INVALID_HANDLE_VALUE ||
    leftHandle == INVALID_HANDLE_VALUE
  ) {
    return false;
  }

  // get the file id
  BY_HANDLE_FILE_INFORMATION leftInfo, rightInfo;
  
  if(
    !GetFileInformationByHandle(rightHandle, &rightInfo) || 
    !GetFileInformationByHandle(leftHandle, &leftInfo)
  ) {
    return false;
  }

  // close the file handle
  CloseHandle(rightHandle);
  CloseHandle(leftHandle);

  // check if the file id is same
  return leftInfo.nFileIndexHigh == rightInfo.nFileIndexHigh &&
         leftInfo.nFileIndexLow == rightInfo.nFileIndexLow;
#endif
}
}  // namespace srilakshmikanthanp::utility
