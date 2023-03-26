// copyright (c) 2023
//
// file:     file_util.h
// brief:    FileUtil class.
// author:   CnLzh

#ifndef TURBONET_SRC_LOGGER_FILE_UTIL_H_
#define TURBONET_SRC_LOGGER_FILE_UTIL_H_

#include "public_define.h"

#include <string>
namespace turbo {

class FileUtil {
 public:
  explicit FileUtil(const std::string &file_name) noexcept;
  ~FileUtil() noexcept;

  void Append(const char *data, size_t len);
  void Flush();
  [[nodiscard]] off_t WrittenBytes() const;

 private:
  size_t Write(const char *data, size_t len);
  FILE *fp_;
  off_t written_bytes_;
  // 64KB buffer
  char buffer_[64 * 1024]{};
  DISALLOW_COPY_AND_ASSIGN(FileUtil)
};

} // turbo

#endif //TURBONET_SRC_LOGGER_FILE_UTIL_H_
