// copyright (c) 2023
//
// file:     log_file.h
// brief:    logfile class.
// author:   CnLzh

#ifndef TURBONET_SRC_LOGGER_FILE_UTIL_H_
#define TURBONET_SRC_LOGGER_FILE_UTIL_H_

#include "public_define.h"

#include <string>
#include <fstream>
namespace turbo {

class FileUtil {
 public:
  explicit FileUtil(const std::string &file_name);
  ~FileUtil();
 private:
  std::fstream file_;
  off_t written_bytes_;
  // 64KB buffer
  tb_s8 buffer_[64 * 1024]{};
  DISALLOW_COPY_AND_ASSIGN(FileUtil)
};

} // turbo

#endif //TURBONET_SRC_LOGGER_FILE_UTIL_H_
