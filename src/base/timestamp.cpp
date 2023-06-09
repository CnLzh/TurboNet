#include "timestamp.h"

namespace turbo {

Timestamp::Timestamp() noexcept
	: time_since_epoch_(0) {
}

Timestamp::Timestamp(const Duration &time_since_epoch) noexcept
	: time_since_epoch_(time_since_epoch) {
}

Timestamp turbo::Timestamp::Now() {
  return Timestamp(SystemLock::now().time_since_epoch());
}

std::string Timestamp::ToString() const {
  char buf[32] = {0};
  auto seconds = std::chrono::duration_cast<std::chrono::seconds>(time_since_epoch_).count();
  auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(time_since_epoch_).count();
  snprintf(buf, sizeof(buf) - 1, "%ld.%06lld", seconds, microseconds % kMicroSecondsPerSecond);
  return buf;
}

std::string Timestamp::ToFormatString(bool show_microseconds) const {
  char buf[64] = {0};
  auto seconds = std::chrono::duration_cast<std::chrono::seconds>(time_since_epoch_).count();
  auto local_time = std::localtime(&seconds);
  if (show_microseconds) {
	auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(time_since_epoch_).count();
	snprintf(buf, sizeof(buf), "%4d-%02d-%02d %02d:%02d:%02d.%06lld",
			 local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday,
			 local_time->tm_hour, local_time->tm_min, local_time->tm_sec, microseconds % kMicroSecondsPerSecond);
  } else {
	snprintf(buf, sizeof(buf), "%4d-%02d-%02d %02d:%02d:%02d",
			 local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday,
			 local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
  }
  return buf;
}

std::chrono::system_clock::duration Timestamp::TimeSinceEpoch() const {
  return time_since_epoch_;
}

std::chrono::days Timestamp::TimeSinceEpochDays() const {
  return std::chrono::duration_cast<std::chrono::days>(time_since_epoch_);
}

std::chrono::seconds Timestamp::TimeSinceEpochSeconds() const {
  return std::chrono::duration_cast<std::chrono::seconds>(time_since_epoch_);
}

std::chrono::microseconds Timestamp::TimeSinceEpochMicroSeconds() const {
  return std::chrono::duration_cast<std::chrono::microseconds>(time_since_epoch_);
}


}

