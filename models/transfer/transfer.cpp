// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "transfer.hpp"
#include <iostream>

namespace srilakshmikanthanp::pulldog::models {
/**
 * @brief Construct a new Transfer object
 */
Transfer::Transfer(QString from, QString to)
    : from(from), to(to) {}

/**
 * @brief Get the from
 */
const QString& Transfer::getFrom() const {
  return from;
}

/**
 * @brief get the to
 */
const QString& Transfer::getTo() const {
  return to;
}

/**
 * @brief Operator < for Transfer
 */
bool Transfer::operator<(const Transfer &transfer) const {
  return from < transfer.from || (from == transfer.from && to < transfer.to);
}

/**
 * @brief Compare two transfer objects
 */
bool Transfer::operator==(const Transfer &transfer) const {
  return from == transfer.from && to == transfer.to;
}
}  // namespace srilakshmikanthanp::pulldog::models
