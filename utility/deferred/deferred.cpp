// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "deferred.hpp"

namespace srilakshmikanthanp::pulldog::utility {
/**
 * @brief Constructor to create deferred object
 */
Deferred::Deferred(const std::function<void()> &func) : func(func) {}

/**
 * @brief Destructor to execute the function
 */
Deferred::~Deferred() {
  func();
}
}
