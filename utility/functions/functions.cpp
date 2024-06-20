// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "functions.hpp"

namespace srilakshmikanthanp::utility {
#ifdef _WIN32
QString getFileNameFromHandle(HANDLE hFile) {
  auto buffer = std::make_unique<WCHAR[]>(MAX_PATH);

  auto size = GetFinalPathNameByHandleW(
    hFile, buffer.get(), MAX_PATH, VOLUME_NAME_DOS
  );

  if(size == 0) {
    throw std::runtime_error(QString("Error: %1").arg(GetLastError()).toStdString());
  }

  return QString::fromWCharArray(buffer.get(), size);;
}
#endif
}  // namespace srilakshmikanthanp::utility
