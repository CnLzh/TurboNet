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

class Logger {
 public:
  enum LogLevel {
	DEBUG,
	INFO,
	WARNING,
	ERROR,
	FATAL,
  };

  class SourceFile {
   public:
	template<tb_s32 N>
	explicit SourceFile(const tb_s8 (&arr)[N])
		: data_(arr),
		  size_(N - 1) {
	  const tb_s8 *slash = strrchr(data_, '/');
	  if (slash) {
		data_ = slash + 1;
		size_ -= static_cast<tb_s32>(data_ - arr);
	  }
	}

	explicit SourceFile(const tb_s8 *file_name)
		: data_(file_name) {
	  const tb_s8 *slash = strrchr(data_, '/');
	  if (slash) data_ = slash + 1;
	  size_ = static_cast<tb_s32>(strlen(data_));
	}

   private:
	const char *data_;
	tb_s32 size_;

	DISALLOW_COPY_AND_ASSIGN(SourceFile)
  };

 private:
  DISALLOW_COPY_AND_ASSIGN(Logger)
};

} // turbo

#endif //TURBONET_SRC_LOGGER_LOGGER_H_
