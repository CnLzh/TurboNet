/// Copyright (C) 2023
//
// File:     event_loop_test.h
// Brief:    EventLoop until test.
// Author:   CnLzh

#include "event_loop.h"
#include "logger.h"

#include <iostream>

using namespace std;
using namespace turbo;
int main() {
  Logger::SetLogLevel(Logger::LogLevel::DEBUG);

  EventLoop loop;
  loop.Loop();
  std::cerr << "Test passed." << std::endl;
  return 0;
}