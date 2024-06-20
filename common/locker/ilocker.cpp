// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "ilocker.hpp"

namespace srilakshmikanthanp::pulldog::common {
/**
 * @brief Construct a new ILocker object
 */
ILocker::ILocker(QObject *parent) : QObject(parent) {}
}  // namespace srilakshmikanthanp::pulldog::common
