// copyright (c) 2023
//
// file:     log_file.h
// brief:    logfile class.
// author:   CnLzh

#ifndef TURBONET_SRC_LOGGER_LOG_FILE_H_
#define TURBONET_SRC_LOGGER_LOG_FILE_H_

#include "public_define.h"
#include "file_util.h"
#include "mutex.h"
#include "timestamp.h"
#include <memory>

#include <string>
namespace turbo {

class LogFile {
 public:
  LogFile(std::string base_name,
		  const off_t &roll_size,
		  const bool &thread_safe = true,
		  const tb_s32 &flush_interval = 3,
		  const tb_s32 &check_every_n = 1024) noexcept;

  ~LogFile() = default;

  void Append(const tb_s8 *data, const tb_s32 &len);
  void Flush();

 private:
  void RollFile();
  std::string GetLogFileName(Timestamp &now_time);
  void AppendUnLocked(const tb_s8 *data, const tb_s32 &len);

  std::string base_name_;
  off_t roll_size_;
  const tb_s32 flush_interval_;
  const tb_s32 check_every_n_;

  tb_s32 count_;

  std::unique_ptr<FileUtil> file_;
  std::unique_ptr<MutexLock> mutex_;
  std::chrono::days start_period_;
  std::chrono::seconds last_roll_;
  std::chrono::seconds last_flush_;

  DISALLOW_COPY_AND_ASSIGN(LogFile);
};

} // turbo

#endif //TURBONET_SRC_LOGGER_LOG_FILE_H_
