#pragma once  // #include only once see https://en.wikipedia.org/wiki/Pragma_once

// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

namespace srilakshmikanthanp::pulldog::types {
/**
 * @brief Lock mode
 */
enum class LockMode { SHARE, NOSHARE };

/**
 * @brief Lock type
 */
enum class LockType { READ, WRITE };
}  // namespace srilakshmikanthanp::pulldog::types
