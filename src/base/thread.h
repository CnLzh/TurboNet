// Copyright (C) 2023
//
// File:     thread.h
// Brief:    Encapsulation Thread class.
// Author:   CnLzh

#ifndef TURBONET_SRC_BASE_THREAD_H_
#define TURBONET_SRC_BASE_THREAD_H_

#include "public_define.h"
#include "count_down_latch.h"

#include <thread>
#include <memory>
#include <functional>
namespace turbo {

class Thread final {
 public:
  using ThreadFunc = std::function<void()>;
  explicit Thread(ThreadFunc func, std::string name = std::string()) noexcept;

  ~Thread() noexcept;

  void Start();
  void Join();

  bool Started() const;
  pid_t Tid() const;
  const std::string &Name() const;

  static tb_s32 NumCreated();

 private:
  void SetDefaultName();
  bool started_;
  bool joined_;
  std::shared_ptr<std::thread> thread_;
  pid_t tid_;
  std::string name_;
  ThreadFunc func_;
  CountDownLatch latch_;
  static std::atomic_int32_t num_created_;
  DISALLOW_COPY_AND_ASSIGN(Thread)
};

} // turbo

#endif //TURBONET_SRC_BASE_THREAD_H_
