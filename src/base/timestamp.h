// Copyright (C) 2023
//
// File:     timestamp.h
// Brief:    Timestamp class.
// Author:   CnLzh

#ifndef TURBONET_SRC_BASE_TIMESTAMP_H_
#define TURBONET_SRC_BASE_TIMESTAMP_H_

#include "public_define.h"

#include <chrono>
#include <string>

namespace turbo {

class Timestamp final
	: public EqualityComparable<Timestamp>,
	  public LessThanComparable<Timestamp> {

  using SystemLock = std::chrono::system_clock;
  using Duration = SystemLock::duration;

 public:
  Timestamp() noexcept;
  explicit Timestamp(const Duration &time_since_epoch) noexcept;

  static Timestamp Now();

  [[nodiscard]] std::string ToString() const;
  [[nodiscard]] std::string ToFormatString(bool show_microseconds = false) const;

  [[nodiscard]] Duration TimeSinceEpoch() const;

  [[nodiscard]] std::chrono::days TimeSinceEpochDays() const;

  [[nodiscard]] std::chrono::seconds TimeSinceEpochSeconds() const;

  [[nodiscard]] std::chrono::microseconds TimeSinceEpochMicroSeconds() const;

 private:
  Duration time_since_epoch_;
};

inline bool operator<(const Timestamp &lhs, const Timestamp &rhs) {
  return lhs.TimeSinceEpoch() < rhs.TimeSinceEpoch();
}

inline bool operator==(const Timestamp &lhs, const Timestamp &rhs) {
  return lhs.TimeSinceEpoch() == rhs.TimeSinceEpoch();
}

inline Timestamp AddTime(const Timestamp &timestamp, double seconds) {
  auto delta = static_cast<long long>(seconds * kMicroSecondsPerSecond);
  return Timestamp(timestamp.TimeSinceEpoch() + std::chrono::microseconds(delta));
}

} // turbo
#endif //TURBONET_SRC_BASE_TIMESTAMP_H_
