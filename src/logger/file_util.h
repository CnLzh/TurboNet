// copyright (c) 2023
//
// file:     log_file.h
// brief:    logfile class.
// author:   CnLzh

#ifndef TURBONET_SRC_LOGGER_FILE_UTIL_H_
#define TURBONET_SRC_LOGGER_FILE_UTIL_H_

#include "public_define.h"

#include <string>
namespace turbo {

class FileUtil {
 public:
  explicit FileUtil(std::string file_name);
 private:
  DISALLOW_COPY_AND_ASSIGN(FileUtil)
};

} // turbo

#endif //TURBONET_SRC_LOGGER_FILE_UTIL_H_
