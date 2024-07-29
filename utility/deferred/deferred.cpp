// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "deferred.hpp"

namespace srilakshmikanthanp::pulldog::utility {
Deferred(Deleter deleter) : std::shared_ptr<void>(nullptr, deleter) {}
}
