#include "logger.h"
#include "current_thread.h"

namespace turbo {

// know string length at compile time
class T {
 public:
  template<size_t N>
  constexpr explicit T(const tb_s8(&str)[N]) noexcept
	  : str_(str),
		len_(N - 1) {}

  constexpr explicit T(const tb_s8 *str, const size_t &len) noexcept
	  : str_(str),
		len_(len) {}

  const char *str_;
  const size_t len_;
};

inline LogStream &operator<<(LogStream &s, T v) {
  s.Append(v.str_, v.len_);
  return s;
}

thread_local tb_s64 t_last_seconds = 0;
thread_local std::string t_time_str;
thread_local tb_s8 t_erno_buf[512];

inline LogStream &operator<<(LogStream &s, const Logger::SourceFile &v) {
  s.Append(v.data_, v.size_);
  return s;
}

void DefaultOutput(const tb_s8 *msg, tb_s32 len) {
  ::fwrite(msg, 1, len, stdout);
}

void DefaultFlush() {
  ::fflush(stdout);
}

const tb_s8 *StrErrorR(tb_s32 errno_info) {
  strerror_r(errno_info, t_erno_buf, sizeof(t_erno_buf));
  return t_erno_buf;
}

Logger::LogLevel Logger::s_log_level_ = Logger::LogLevel::INFO;
Logger::OutputFunc Logger::s_output_ = DefaultOutput;
Logger::FlushFunc Logger::s_flush_ = DefaultFlush;

const tb_s8 *kLogLevelName[Logger::LogLevel::NUM_LOG_LEVELS] = {
	"DEBUG ",
	"INFO  ",
	"WARN  ",
	"ERROR ",
	"FATAL ",
};

Logger::Logger(const tb_s8 *file, tb_s32 line, const tb_s8 *func) noexcept
	: impl_(DEBUG, 0, file, line) {
  impl_.log_stream_ << "(" << func << "): ";
}

Logger::Logger(const tb_s8 *file, tb_s32 line) noexcept
	: impl_(INFO, 0, file, line) {
}

Logger::Logger(const tb_s8 *file, tb_s32 line, LogLevel level) noexcept
	: impl_(level, 0, file, line) {
}

Logger::Logger(const tb_s8 *file, tb_s32 line, bool is_abort) noexcept
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
	  log_level_(log_level),
	  log_stream_(),
	  line_(line),
	  source_file_(file) {
  // stream << timestamp << thread info << LogLevel << SourceFile << Line
  FormatTime();
  CurrentThread::CachedTid();
  log_stream_ << T(" - thread:") << T(CurrentThread::TidString().c_str(), CurrentThread::TidStringLength()) << T(" - ")
			  << T(kLogLevelName[log_level], 6)
			  << source_file_ << ':' << line_ << T(": ");
  // if(errno_info) stream << info
  if (errno_info != 0) {
	log_stream_ << StrErrorR(errno_info) << T(" (errno=") << errno_info << T(") ");
  }
}

void Logger::Impl::FormatTime() {
  auto seconds = timestamp_.TimeSinceEpochSeconds().count();
  if (t_last_seconds != seconds) {
	t_last_seconds = seconds;
	t_time_str = timestamp_.ToFormatString(false);
  }

  auto microseconds = timestamp_.TimeSinceEpochMicroSeconds().count();
  tb_s8 buf[8];
  snprintf(buf, sizeof(buf), ".%06lld", microseconds % kMicroSecondsPerSecond);

  log_stream_ << T(t_time_str.c_str(), 19) << T(buf);
}

void Logger::Impl::Finish() {
  log_stream_ << '\n';
}

} // turbo

