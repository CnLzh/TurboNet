/// Copyright (C) 2023
//
// File:     async_logging_test.h
// Brief:    AsyncLogging until test.
// Author:   CnLzh

#include "async_logging.h"
#include "logger.h"

#include <iostream>

using namespace std;
using namespace turbo;

int main() {

  Logger::SetDefaultSingletonAsyncLogging();
  Logger::StartDefaultSingletonAsyncLogging();
  std::string str(1000, 'x');
  for (int i = 0; i < 10; i++)
	LOG_INFO << str;

  sleep(5);

  std::cerr << "Test passed." << std::endl;
  return 0;
}