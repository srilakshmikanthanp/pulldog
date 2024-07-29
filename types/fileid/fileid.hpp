#pragma once  // #include only once see https://en.wikipedia.org/wiki/Pragma_once

// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#ifdef _WIN32
#include <windows.h>
#endif

#include "utility/functions/functions.hpp"

namespace srilakshmikanthanp::pulldog::types {
struct FileId {
 private:

#ifdef _WIN32
  DWORD high, low;
#endif

 public:

  /**
   * @brief constructor to get the file id
   */
  FileId(QString file);

  /**
   * @brief Default constructor
   */
  FileId();

  /**
   * @brief set file
   */
  void setFile(QString file);

  /**
   * @brief is same file
   */
  bool isSameFile(const FileId &file) const;

  /**
   * @brief Equality operator
   */
  bool operator==(const FileId &other) const;
};
}  // namespace srilakshmikanthanp::pulldog::types
