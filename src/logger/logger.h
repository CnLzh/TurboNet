// Copyright (C) 2023
//
// File:     logger.h
// Brief:    The logger class provides interfaces for users to operate on logs.
// Author:   CnLzh

#ifndef TURBONET_SRC_LOGGER_LOGGER_H_
#define TURBONET_SRC_LOGGER_LOGGER_H_

#include "timestamp.h"
#include "log_stream.h"

#include <functional>

namespace turbo {

#define LOG_DEBUG  if(turbo::Logger::GetLogLevel() <= turbo::Logger::DEBUG)  \
    turbo::Logger(__FILE__, __LINE__, __func__).Stream()
#define LOG_INFO  if(turbo::Logger::GetLogLevel() <= turbo::Logger::INFO)  \
    turbo::Logger(__FILE__, __LINE__).Stream()
#define LOG_WARN  turbo::Logger(__FILE__, __LINE__, turbo::Logger::WARN).Stream()
#define LOG_ERROR  turbo::Logger(__FILE__, __LINE__, turbo::Logger::ERROR).Stream()
#define LOG_FATAL  turbo::Logger(__FILE__, __LINE__, turbo::Logger::FATAL).Stream()
#define LOG_SYS_ERROR  turbo::Logger(__FILE__, __LINE__, false).Stream()
#define LOG_SYS_FATAL  turbo::Logger(__FILE__, __LINE__, true).Stream()

class Logger final {
 public:
  enum LogLevel {
	DEBUG,
	INFO,
	WARN,
	ERROR,
	FATAL,
	NUM_LOG_LEVELS,
  };

  class SourceFile final {
   public:
	template<int N>
	explicit SourceFile(const char (&arr)[N]) noexcept
		: data_(arr),
		  size_(N - 1) {
	  const char *slash = strrchr(data_, '/');
	  if (slash) {
		data_ = slash + 1;
		size_ -= static_cast<int>(data_ - arr);
	  }
	}

	explicit SourceFile(const char *file_name) noexcept
		: data_(file_name) {
	  const char *slash = strrchr(data_, '/');
	  if (slash) data_ = slash + 1;
	  size_ = static_cast<int>(strlen(data_));
	}

	const char *data_;
	int size_;
  };

  Logger(const char *file, int line) noexcept;
  Logger(const char *file, int line, const char *func) noexcept;
  Logger(const char *file, int line, LogLevel level) noexcept;
  Logger(const char *file, int line, bool is_abort) noexcept;

  ~Logger();

  LogStream &Stream();

  static LogLevel GetLogLevel();
  static void SetLogLevel(const LogLevel &level);

  using OutputFunc = void (*)(const char *msg, int len);
  using FlushFunc = void (*)();

  static void SetOutput(OutputFunc);
  static void SetFlush(FlushFunc);

  static void SetDefaultSingletonAsyncLogging(const char *base_name = "TurboNet",
											  const off_t &roll_size = 512 * 1024 * 1024);
  static void StartDefaultSingletonAsyncLogging();
  static void StopDefaultSingletonAsyncLogging();

 private:

  class Impl final {
   public:
	using LogLevel = Logger::LogLevel;

	Impl(LogLevel log_level, int errno_info, const char *file, int line) noexcept;
	void FormatTime();
	void Finish();

	Timestamp timestamp_;
	LogLevel log_level_;
	LogStream log_stream_;
	int line_;
	SourceFile source_file_;
  };

  Impl impl_;

  static LogLevel s_log_level_;
  static OutputFunc s_output_;
  static FlushFunc s_flush_;
  DISALLOW_COPY_AND_ASSIGN(Logger)
};

const char *StrErrorR(int errno_info);

} // turbo

#endif //TURBONET_SRC_LOGGER_LOGGER_H_
