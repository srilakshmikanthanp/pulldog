// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "icopier.hpp"

namespace srilakshmikanthanp::pulldog::common {
/**
 * @brief Construct a new ICopier object
 *
 * @param src
 * @param dest
 * @param parent
 */
ICopier::ICopier(QObject *parent): QObject(parent) {}
}  // namespace srilakshmikanthanp::pulldog::common
