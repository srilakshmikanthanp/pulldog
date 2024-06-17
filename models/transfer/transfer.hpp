#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QString>
#include <QHash>

namespace srilakshmikanthanp::pulldog::models {
/**
 * @brief Transfer class to store the transfer details of files
 */
class Transfer {
 private:
  QString from, to;

 public:

  /**
   * @brief Construct a new Transfer object
   */
  Transfer(QString from, QString to);

  /**
   * @brief Destroy the Transfer object
   */
  ~Transfer() = default;

  /**
   * @brief Get the from
   *
   * @return QString
   */
  const QString& getFrom() const;

  /**
   * @brief get the to
   */
  const QString& getTo() const;

  /**
   * @brief Operator < for Transfer
   */
  bool operator<(const Transfer &transfer) const;

  /**
   * @brief Compare two transfer objects
   */
  bool operator==(const Transfer &transfer) const;
};
}  // namespace srilakshmikanthanp::pulldog::models

/**
 * @brief Hash function for Transfer Qt qHash
 */
inline size_t qHash(const srilakshmikanthanp::pulldog::models::Transfer &transfer) {
  return qHash(transfer.getFrom()) ^ qHash(transfer.getTo());
}
