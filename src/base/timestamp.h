// Copyright (C) 2023
//
// File:     timestamp.h
// Brief:    Timestamp class.
// Author:   CnLzh

#ifndef TURBONET_SRC_BASE_TIMESTAMP_H_
#define TURBONET_SRC_BASE_TIMESTAMP_H_

#include "public_define.h"

#include <string>

namespace turbo {

class Timestamp
	: public EqualityComparable<Timestamp>,
	  public LessThanComparable<Timestamp> {
 public:
  Timestamp();
  explicit Timestamp(const tb_s64_t &microseconds_since_epoch);

  static Timestamp Now();

  [[nodiscard]] std::string ToString() const;
  [[nodiscard]] std::string ToFormatString(bool show_microseconds = false) const;

  [[nodiscard]] tb_s64_t MicrosecondsSinceEpoch() const;

  static const int kMicroSecondsPerSecond = 1000 * 1000;

 private:
  tb_s64_t microseconds_since_epoch_;

};

inline bool operator<(const Timestamp &lhs, const Timestamp &rhs) {
  return lhs.MicrosecondsSinceEpoch() < rhs.MicrosecondsSinceEpoch();
}

inline bool operator==(const Timestamp &lhs, const Timestamp &rhs) {
  return lhs.MicrosecondsSinceEpoch() == rhs.MicrosecondsSinceEpoch();
}

inline Timestamp AddTime(const Timestamp &timestamp, const tb_f64 &seconds) {
  auto delta = static_cast<tb_s64_t>(seconds * Timestamp::kMicroSecondsPerSecond);
  return Timestamp(timestamp.MicrosecondsSinceEpoch() + delta);
}

} // turbo
#endif //TURBONET_SRC_BASE_TIMESTAMP_H_
