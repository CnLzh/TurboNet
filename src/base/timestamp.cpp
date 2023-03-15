#include "timestamp.h"

#include <sys/time.h>
#include <cinttypes>

namespace turbo {

static_assert(sizeof(Timestamp) == sizeof(tb_s64_t),
			  "Timestamp should be same size as int64_t");

Timestamp::Timestamp()
	: microseconds_since_epoch_(0) {

}

Timestamp::Timestamp(const tb_s64_t &microseconds_since_epoch)
	: microseconds_since_epoch_(microseconds_since_epoch) {

}

Timestamp turbo::Timestamp::Now() {
  struct timeval tv{};
  gettimeofday(&tv, nullptr);
  tb_s64_t seconds = tv.tv_sec;
  return Timestamp(seconds * kMicroSecondsPerSecond + tv.tv_usec);
}

std::string Timestamp::ToString() const {
  tb_s8 buf[32] = {0};
  tb_s64_t seconds = microseconds_since_epoch_ / kMicroSecondsPerSecond;
  tb_s64_t microseconds = microseconds_since_epoch_ % kMicroSecondsPerSecond;
  snprintf(buf, sizeof(buf), "%" PRId64 ".%06" PRId64 "", seconds, microseconds);
  return buf;
}

std::string Timestamp::ToFormatString(bool show_microseconds) const {
  tb_s8 buf[64] = {0};
  auto seconds = static_cast<time_t>(microseconds_since_epoch_ / kMicroSecondsPerSecond);
  struct tm tm_time{};
  gmtime_r(&seconds, &tm_time);

  if (show_microseconds) {
	auto microseconds = static_cast<tb_s32>(microseconds_since_epoch_ % kMicroSecondsPerSecond);
	snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d.%06d",
			 tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
			 tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec, microseconds);
  } else {
	snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d",
			 tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
			 tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
  }
  return buf;
}

tb_s64_t Timestamp::MicrosecondsSinceEpoch() const {
  return microseconds_since_epoch_;
}

}