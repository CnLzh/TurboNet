#include "logger.h"

#include <utility>
#include "current_thread.h"
#include "singleton.h"
#include "async_logging.h"

namespace turbo {

// know string length at compile time
class T {
 public:
  template<size_t N>
  constexpr explicit T(const char(&str)[N]) noexcept
	  : str_(str),
		len_(N - 1) {}

  constexpr explicit T(const char *str, const size_t &len) noexcept
	  : str_(str),
		len_(len) {}

  const char *str_;
  const size_t len_;
};

inline LogStream &operator<<(LogStream &s, T v) {
  s.Append(v.str_, v.len_);
  return s;
}

thread_local long long t_last_seconds = 0;
thread_local std::string t_time_str;
thread_local char t_erno_buf[512];

static AsyncLogging *g_async_logging = nullptr;
static std::atomic<bool> g_is_default_async_started = false;

inline LogStream &operator<<(LogStream &s, const Logger::SourceFile &v) {
  s.Append(v.data_, v.size_);
  return s;
}

void DefaultOutput(const char *msg, size_t len) {
  ::fwrite(msg, 1, len, stdout);
}

void DefaultFlush() {
  ::fflush(stdout);
}

void DefaultAsyncLoggingOutput(const char *msg, size_t len) {
  g_async_logging->Append(msg, len);
}

const char *StrErrorR(int errno_info) {
  strerror_r(errno_info, t_erno_buf, sizeof(t_erno_buf));
  return t_erno_buf;
}

Logger::LogLevel Logger::s_log_level_ = Logger::LogLevel::INFO;
Logger::OutputFunc Logger::s_output_ = DefaultOutput;
Logger::FlushFunc Logger::s_flush_ = DefaultFlush;

const char *kLogLevelName[Logger::LogLevel::NUM_LOG_LEVELS] = {
	"DEBUG ",
	"INFO  ",
	"WARN  ",
	"ERROR ",
	"FATAL ",
};

Logger::Logger(const char *file, int line, const char *func) noexcept
	: impl_(DEBUG, 0, file, line) {
  impl_.log_stream_ << "(" << func << "): ";
}

Logger::Logger(const char *file, int line) noexcept
	: impl_(INFO, 0, file, line) {
}

Logger::Logger(const char *file, int line, LogLevel level) noexcept
	: impl_(level, 0, file, line) {
}

Logger::Logger(const char *file, int line, bool is_abort) noexcept
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

void Logger::SetDefaultSingletonAsyncLogging(const char *base_name, off_t roll_size) {
  g_async_logging = &Singleton<AsyncLogging>::Instance(base_name, roll_size);
  SetOutput(DefaultAsyncLoggingOutput);
}

void Logger::StartDefaultSingletonAsyncLogging() {
  assert(g_async_logging);
  if (!g_is_default_async_started) {
	g_async_logging->Start();
	g_is_default_async_started = true;
  }
}

void Logger::StopDefaultSingletonAsyncLogging() {
  assert(g_async_logging);
  if (g_is_default_async_started) {
	g_async_logging->Stop();
	g_is_default_async_started = false;
  }
}

Logger::Impl::Impl(LogLevel log_level,
				   int errno_info,
				   const char *file,
				   int line) noexcept
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
  char buf[8];
  snprintf(buf, sizeof(buf), ".%06lld", microseconds % kMicroSecondsPerSecond);

  log_stream_ << T(t_time_str.c_str(), 19) << T(buf);
}

void Logger::Impl::Finish() {
  log_stream_ << '\n';
}

} // turbo

