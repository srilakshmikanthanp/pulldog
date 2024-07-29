// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "fileid.hpp"

namespace srilakshmikanthanp::pulldog::types {
/**
 * @brief Default constructor
 */
FileId::FileId(): high(0), low(0) {}

/**
 * @brief constructor
 */
FileId::FileId(QString file) {
  this->setFile(file);
}

/**
 * @brief set file
 */
void FileId::setFile(QString file) {
  if (!file.isEmpty()) {
    std::tie(high, low) = utility::getFileId(file);
  }
}

/**
 * @brief is same file
 */
bool FileId::isSameFile(const FileId &file) const {
#ifdef _WIN32
  return high == file.high && low == file.low;
#endif
}

/**
 * @brief Equality operator
 */
bool FileId::operator==(const FileId &other) const {
  return this->isSameFile(other);
}
}  // namespace srilakshmikanthanp::pulldog::types
