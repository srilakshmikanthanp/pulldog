#pragma once // #include only once see https://en.wikipedia.org/wiki/Pragma_once

// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <functional>
#include <utility>
#include <memory>

namespace srilakshmikanthanp::pulldog::utility {
/**
 * @brief Deferred class to execute a function at the end of the scope
 */
struct Deferred {
  // Constructor to create deferred object
  Deferred(const std::function<void()> &func);

  // Destructor to execute the function
  ~Deferred();

 private:
  std::function<void()> func;
};
}  // namespace srilakshmikanthanp::pulldog::utility

/**
 * @brief Utility macro to join two tokens
 */
#define DEFER_ACTUALLY_JOIN(x, y) x##y

#define DEFER_JOIN(x, y) DEFER_ACTUALLY_JOIN(x, y)

#ifdef __COUNTER__
  #define DEFER_UNIQUE_VARNAME(x) DEFER_JOIN(x, __COUNTER__)
#else
  #define DEFER_UNIQUE_VARNAME(x) DEFER_JOIN(x, __LINE__)
#endif

/**
 * @brief Macro will help to create a deferred object without name
 */
#define DEFER(...) \
  srilakshmikanthanp::pulldog::utility::Deferred \
  DEFER_UNIQUE_VARNAME(deferred_) \
  (__VA_ARGS__)
