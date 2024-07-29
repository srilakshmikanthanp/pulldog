#pragma once  // #include only once see https://en.wikipedia.org/wiki/Pragma_once

// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QFileInfo>

#ifndef _WIN32
#include <windows.h>
#endif

#include "utility/functions/functions.hpp"

namespace srilakshmikanthanp::pulldog::types {
struct FileInfo : public QFileInfo {
 private:
#ifdef _WIN32
  DWORD high;
  DWORD low;
#endif

 public:
  /**
   * @brief constructor that has all the 
   * constructors of QFileInfo
   */
  template<typename... Args>
  FileInfo(Args&&... args): QFileInfo(std::forward<Args>(args)...){
    std::tie(high, low) = utility::getFileId(this->absoluteFilePath());
  }

  /**
   * @brief is same file
   */
  bool isSameFile(const FileInfo &file) const;
};
}  // namespace srilakshmikanthanp::pulldog::types
