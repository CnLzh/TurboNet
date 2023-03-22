#include "logger.h"

namespace turbo {

inline LogStream &operator<<(LogStream &s, const Logger::SourceFile &v) {
  s.Append(v.data_, v.size_);
  return s;
}

void DefaultOutput(const tb_s8 *msg, tb_s32 len) {
  fwrite(msg, sizeof(tb_s8), len, stdout);
}

void DefaultFlush() {
  fflush(stdout);
}

Logger::LogLevel Logger::s_log_level_ = Logger::LogLevel::INFO;
Logger::OutputFunc Logger::s_output_ = DefaultOutput;
Logger::FlushFunc Logger::s_flush_ = DefaultFlush;

Logger::Logger(SourceFile file, tb_s32 line)
	: impl_(INFO, nullptr, file, line) {

}

Logger::Logger(SourceFile file, tb_s32 line, LogLevel level)
	: impl_(level, nullptr, file, line) {

}

Logger::Logger(SourceFile file, tb_s32 line, LogLevel level, const tb_s8 *func)
	: impl_(level, nullptr, file, line) {
  impl_.log_stream_ << func << ' ';
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
				   tb_s32 errno,
				   const Logger::SourceFile &file,
				   tb_s32 line) noexcept
	: timestamp_(Timestamp::Now()),
	  log_stream_(),
	  log_level_(log_level),
	  source_file_(file),
	  line_(line) {

}

void Logger::Impl::FormatTime() {

}

void Logger::Impl::Finish() {

}

} // turbo