// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "fileinfo.hpp"

namespace srilakshmikanthanp::pulldog::types {
/** 
 * @brief is same file
  */
bool FileInfo::isSameFile(const FileInfo &file) const {
#ifdef _WIN32
  return high == file.high && low == file.low;
#endif
}
}
