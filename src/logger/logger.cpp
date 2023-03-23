#include "logger.h"
#include "current_thread.h"

#include <iostream>
namespace turbo {

thread_local tb_s64 t_last_seconds = 0;
thread_local std::string t_time_str;

inline LogStream &operator<<(LogStream &s, const Logger::SourceFile &v) {
  s.Append(v.data_, v.size_);
  return s;
}

void DefaultOutput(const tb_s8 *msg, tb_s32 len) {
  std::fwrite(msg, sizeof(tb_s8), len, stdout);
}

void DefaultFlush() {
  std::cout.flush();
}

const tb_s8 *StrErrorR(tb_s32 errno_info) {
  return std::strerror(errno_info);
}

Logger::LogLevel Logger::s_log_level_ = Logger::LogLevel::INFO;
Logger::OutputFunc Logger::s_output_ = DefaultOutput;
Logger::FlushFunc Logger::s_flush_ = DefaultFlush;

const std::string kLogLevelName[Logger::LogLevel::NUM_LOG_LEVELS] = {
	"DEBUG ",
	"INFO  ",
	"WARN  ",
	"ERROR ",
	"FATAL ",
};

Logger::Logger(const tb_s8 *file, tb_s32 line, const tb_s8 *func)
	: impl_(DEBUG, 0, file, line) {
  impl_.log_stream_ << "(" << func << "): ";
}

Logger::Logger(const tb_s8 *file, tb_s32 line)
	: impl_(INFO, 0, file, line) {
}

Logger::Logger(const tb_s8 *file, tb_s32 line, LogLevel level)
	: impl_(level, 0, file, line) {
}

Logger::Logger(const tb_s8 *file, tb_s32 line, bool is_abort)
	: impl_(is_abort ? ERROR : FATAL, errno, file, line) {
}

Logger::~Logger() {
  impl_.Finish();
  const LogStream::Buffer &buf(Stream().GetBuffer());
  s_output_(buf.Data(), buf.Length());
  if (impl_.log_level_ == FATAL) {
	s_flush_();
	abort();
  }
}

LogStream &Logger::Stream() {
  return impl_.log_stream_;
}

Logger::LogLevel Logger::GetLogLevel() {
  return Logger::s_log_level_;
}

void Logger::SetLogLevel(const LogLevel &level) {
  Logger::s_log_level_ = level;
}

void Logger::SetOutput(Logger::OutputFunc func) {
  s_output_ = func;
}

void Logger::SetFlush(Logger::FlushFunc func) {
  s_flush_ = func;
}

Logger::Impl::Impl(LogLevel log_level,
				   tb_s32 errno_info,
				   const tb_s8 *file,
				   tb_s32 line) noexcept
	: timestamp_(Timestamp::Now()),
	  log_stream_(),
	  log_level_(log_level),
	  source_file_(file),
	  line_(line) {
  // stream << timestamp << thread info << LogLevel << SourceFile << Line
  FormatTime();
  CurrentThread::CachedTid();
  log_stream_ << " - thread:" << CurrentThread::TidString() << " - " << kLogLevelName[log_level]
			  << source_file_ << ':' << line_ << ": ";
  // if(errno_info) stream << info
  if (errno_info != 0) {
	log_stream_ << StrErrorR(errno_info) << " (errno=" << errno_info << ") ";
  }
}

void Logger::Impl::FormatTime() {
  auto seconds = std::chrono::duration_cast<std::chrono::seconds>(timestamp_.TimeSinceEpoch()).count();
  if (t_last_seconds != seconds) {
	t_last_seconds = seconds;
	t_time_str = timestamp_.ToFormatString(false);
  }

  auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(timestamp_.TimeSinceEpoch()).count();
  tb_s8 buf[8];
  snprintf(buf, sizeof(buf), ".%06lld", microseconds % kMicroSecondsPerSecond);

  log_stream_ << t_time_str << buf;
}

void Logger::Impl::Finish() {
  log_stream_ << '\n';
}

} // turbo

