// copyright (c) 2023
//
// file:     async_logging.h
// brief:    AsyncLogging class.
// author:   CnLzh

#ifndef TURBONET_SRC_LOGGER_ASYNC_LOGGING_H_
#define TURBONET_SRC_LOGGER_ASYNC_LOGGING_H_

#include "public_define.h"
#include "log_stream.h"
#include "thread.h"

#include <memory>
#include <vector>
#include <atomic>
#include <string>
namespace turbo {

class AsyncLogging {
 public:
  AsyncLogging(std::string base_name,
			   const off_t &roll_size,
			   const int &flush_interval = 3);
  ~AsyncLogging();

  void Start();
  void Stop();
  void Append(const tb_s8 *data, const tb_s32 &len);

 private:

  void ThreadFunc();

  using Buffer = FixedBuffer<kLargeBuffer>;
  using BufferVector = std::vector<std::unique_ptr<Buffer>>;
  using BufferPtr = BufferVector::value_type;

  const tb_s32 flush_interval_;
  std::atomic<bool> running_;
  const std::string base_name_;
  const off_t roll_size_;
  Thread thread_;
  CountDownLatch latch_;
  MutexLock mutex_;
  Condition cond_;
  BufferPtr current_buffer_;
  BufferPtr next_buffer_;
  BufferVector buffers_;

  DISALLOW_COPY_AND_ASSIGN(AsyncLogging)
};

} // turbo

#endif //TURBONET_SRC_LOGGER_ASYNC_LOGGING_H_
