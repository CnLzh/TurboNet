#include "timestamp.h"

namespace turbo {

Timestamp::Timestamp()
	: time_since_epoch_(0) {
}

Timestamp::Timestamp(const Duration &time_since_epoch)
	: time_since_epoch_(time_since_epoch) {
}

Timestamp turbo::Timestamp::Now() {
  return Timestamp(SystemLock::now().time_since_epoch());
}

std::string Timestamp::ToString() const {
  tb_s8 buf[32] = {0};
  auto seconds = std::chrono::duration_cast<std::chrono::seconds>(time_since_epoch_).count();
  auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(time_since_epoch_).count();
  snprintf(buf, sizeof(buf) - 1, "%ld.%06ld", seconds, microseconds % kMicroSecondsPerSecond);
  return buf;
}

std::string Timestamp::ToFormatString(bool show_microseconds) const {
  tb_s8 buf[64] = {0};
  auto seconds = std::chrono::duration_cast<std::chrono::seconds>(time_since_epoch_).count();
  auto local_time = std::localtime(&seconds);
  if (show_microseconds) {
	auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(time_since_epoch_).count();
	snprintf(buf, sizeof(buf), "%4d-%02d-%02d %02d:%02d:%02d.%06ld",
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

}

