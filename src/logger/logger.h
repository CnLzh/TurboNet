// Copyright (C) 2023
//
// File:     logger.h
// Brief:    The logger class provides interfaces for users to operate on logs.
// Author:   CnLzh

#ifndef TURBONET_SRC_LOGGER_LOGGER_H_
#define TURBONET_SRC_LOGGER_LOGGER_H_

#include "public_define.h"

class Logger {
 public:
  enum LogLevel{
	DEBUG = 0,
	INFO,
	WARNING,
	ERROR,
	FATAL
  };

 private:
  DISALLOW_COPY_AND_ASSIGN(Logger)
};

#endif //TURBONET_SRC_LOGGER_LOGGER_H_
