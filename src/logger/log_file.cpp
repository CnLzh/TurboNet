#include "log_file.h"

#include <utility>
#include <cassert>
namespace turbo {

LogFile::LogFile(std::string base_name,
				 off_t roll_size,
				 bool thread_safe,
				 int flush_interval,
				 int check_every_n) noexcept
	: base_name_(std::move(base_name)),
	  roll_size_(roll_size),
	  flush_interval_(flush_interval),
	  check_every_n_(check_every_n),
	  count_(0),
	  file_(nullptr),
	  mutex_(thread_safe ? std::make_unique<MutexLock>() : nullptr),
	  start_period_(0),
	  last_roll_(0),
	  last_flush_(0) {
  assert(base_name_.find('/') == std::string::npos);
  RollFile();
}

void LogFile::Append(const char *data, size_t len) {
  if (mutex_) {
	MutexLockGuard lock(*mutex_);
	AppendUnLocked(data, len);
  } else {
	AppendUnLocked(data, len);
  }
}

void LogFile::RollFile() {
  Timestamp now_time;
  const std::string file_name = GetLogFileName(now_time);
  std::chrono::seconds now_time_seconds = now_time.TimeSinceEpochSeconds();
  if (now_time_seconds > last_roll_) {
	start_period_ = now_time.TimeSinceEpochDays();
	last_roll_ = now_time_seconds;
	last_flush_ = now_time_seconds;
	file_ = std::make_unique<FileUtil>(file_name);
  }
}

std::string LogFile::GetLogFileName(Timestamp &now_time) {
  now_time = Timestamp::Now();
  std::string file_name;
  file_name += base_name_;
  file_name += now_time.ToFormatString(false);
  file_name += GetHostName();
  file_name += GetPid();
  file_name += ".log";
  return file_name;
}

void LogFile::AppendUnLocked(const char *data, size_t len) {
  file_->Append(data, len);
  if (file_->WrittenBytes() > roll_size_) {
	RollFile();
  } else {
	++count_;
	if (count_ >= check_every_n_) {
	  Timestamp now_time = Timestamp::Now();
	  std::chrono::seconds now_seconds = now_time.TimeSinceEpochSeconds();
	  if (now_time.TimeSinceEpochDays() > start_period_) {
		RollFile();
	  } else if ((now_seconds.count() - last_flush_.count()) > flush_interval_) {
		last_flush_ = now_seconds;
		file_->Flush();
	  }
	}
  }
}

void LogFile::Flush() {
  if (mutex_) {
	MutexLockGuard lock(*mutex_);
	file_->Flush();
  } else {
	file_->Flush();
  }
}

} // turbo