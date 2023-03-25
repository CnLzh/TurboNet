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

AsyncLogging *g_sync_log = nullptr;

void AsyncOutput(const tb_s8 *msg, tb_s32 len) {
  g_sync_log->Append(msg, len);
}

int main(int argc, char *argv[]) {
  off_t roll_size = 1024 * 1024 * 512;
  std::string s = basename(argv[0]);
  g_sync_log = new AsyncLogging(basename(argv[0]), roll_size);
  g_sync_log->Start();
  Logger::SetOutput(AsyncOutput);

  std::string str(1000, 'x');
  for (int i = 0; i < 10; i++)
	LOG_INFO << str;

  delete g_sync_log;
  std::cerr << "Test passed." << std::endl;
  return 0;
}