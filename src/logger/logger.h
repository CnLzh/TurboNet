// Copyright (C) 2023
//
// File:     logger.h
// Brief:    The logger class provides interfaces for users to operate on logs.
// Author:   CnLzh

#ifndef TURBONET_SRC_LOGGER_LOGGER_H_
#define TURBONET_SRC_LOGGER_LOGGER_H_

#include "timestamp.h"
#include "log_stream.h"

namespace turbo {

class Logger final {
 public:
  enum LogLevel {
	DEBUG,
	INFO,
	WARNING,
	ERROR,
	FATAL,
  };

  class SourceFile final {
   public:
	template<tb_s32 N>
	explicit SourceFile(const tb_s8 (&arr)[N]) noexcept
		: data_(arr),
		  size_(N - 1) {
	  const tb_s8 *slash = strrchr(data_, '/');
	  if (slash) {
		data_ = slash + 1;
		size_ -= static_cast<tb_s32>(data_ - arr);
	  }
	}

	explicit SourceFile(const tb_s8 *file_name) noexcept
		: data_(file_name) {
	  const tb_s8 *slash = strrchr(data_, '/');
	  if (slash) data_ = slash + 1;
	  size_ = static_cast<tb_s32>(strlen(data_));
	}

	const char *data_;
	tb_s32 size_;
  };

  Logger(SourceFile file, tb_s32 line);
  Logger(SourceFile file, tb_s32 line, LogLevel level);
  Logger(SourceFile file, tb_s32 line, LogLevel level, const tb_s8 *func);

  LogStream &Stream();

  static LogLevel GetLogLevel();
  static void SetLogLevel(const LogLevel &level);

  using OutputFunc = void (*)(const tb_s8 *msg, tb_s32 len);
  using FlushFunc = void (*)();

  static void SetOutput(OutputFunc);
  static void SetFlush(FlushFunc);

 private:

  class Impl final {
   public:
	using LogLevel = Logger::LogLevel;

	Impl(LogLevel log_level, tb_s32 errno, const SourceFile &file, tb_s32 line) noexcept;
	void FormatTime();
	void Finish();

	Timestamp timestamp_;
	LogLevel log_level_;
	LogStream log_stream_;
	tb_s32 line_;
	SourceFile source_file_;
  };

  Impl impl_;

  static LogLevel s_log_level_;
  static OutputFunc s_output_;
  static FlushFunc s_flush_;
  DISALLOW_COPY_AND_ASSIGN(Logger)
};

} // turbo

#endif //TURBONET_SRC_LOGGER_LOGGER_H_
