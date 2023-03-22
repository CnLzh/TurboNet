#include "logger.h"

namespace turbo {

Logger::Logger(SourceFile file, tb_s32 line) {

}

Logger::Logger(SourceFile file, LogLevel level) {

}

Logger::Logger(SourceFile file, LogLevel level, const tb_s8 *func) {

}

#if 0
LogStream &Logger::Stream() {
  return impl_.log_stream_;
}
#endif

Logger::LogLevel Logger::GetLogLevel() {
  return Logger::LogLevel::INFO;
#if 0
  return Logger::log_level_;
#endif
}

void Logger::SetLogLevel(const LogLevel& level) {
#if 0
  Logger::log_level_ = level;
#endif
}

void Logger::SetOutput(Logger::OutputFunc) {

}

void Logger::SetFlush(Logger::FlushFunc) {

}

Logger::Impl::Impl(LogLevel log_level,
				   tb_s32 errno,
				   const Logger::SourceFile &file,
				   tb_s32 line) noexcept
	: timestamp_(Timestamp::Now()),
	  log_stream_(),
	  log_level_(log_level),
	  line_(line),
	  source_file_(file) {

}

void Logger::Impl::FormatTime() {

}

} // turbo