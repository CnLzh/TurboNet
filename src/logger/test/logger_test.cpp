// Copyright (C) 2023
//
// File:     logger_test.h
// Brief:    Logger until test.
// Author:   CnLzh

#include "logger.h"

#include <iostream>

using namespace std;
using namespace turbo;

int main() {
  Logger::SetLogLevel(Logger::DEBUG);
  LOG_DEBUG << "debug";
  LOG_INFO << "info";
  LOG_WARN << "warning";
  LOG_ERROR << "error";

  std::cerr << "Test passed." << std::endl;
  return 0;
}
