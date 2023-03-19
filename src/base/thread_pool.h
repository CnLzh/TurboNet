// Copyright (C) 2023
//
// File:     thread_pool.h
// Brief:    ThreadPool class.
// Author:   CnLzh

#ifndef TURBONET_SRC_BASE_THREAD_POOL_H_
#define TURBONET_SRC_BASE_THREAD_POOL_H_

#include "public_define.h"
#include "thread.h"
#include "mutex.h"
#include "condition.h"

#include <functional>
#include <list>
#include <memory>
#include <deque>

namespace turbo {

class ThreadPool final {

  enum ThreadPoolStatus {
	TPS_RUNNING,
	TPS_SHUTDOWN,
	TPS_TERMINATED,
  };

  using Task = std::function<void()>;

  std::string thread_pool_name_;

  const tb_u32 kMaxPoolSize_;
  const tb_u32 kCorePoolSize_;
  const tb_u32 kMaxTaskSize_;
  const tb_f64 kIdleTimeoutSeconds_;

  std::list<std::unique_ptr<Thread>> thread_pool_;
  mutable MutexLock thread_pool_mutex_;

  std::deque<Task> tasks_;
  mutable MutexLock tasks_mutex_;
  Condition task_cv_;

  std::list<pid_t> monitor_thread_free_;
  mutable MutexLock monitor_mutex_;
  Condition monitor_cv_;

  std::atomic<ThreadPoolStatus> thread_pool_status_;
  std::atomic<tb_u32> idle_thread_num_;
  std::atomic<tb_u32> now_thread_num_;

 public:
  explicit ThreadPool(
	  std::string thread_pool_name = "ThreadPool",
	  const tb_u32 &max_pool_size = std::thread::hardware_concurrency(),
	  const tb_u32 &core_pool_size = std::thread::hardware_concurrency() / 2,
	  const tb_u32 &max_task_size = 1000,
	  const tb_f64 &idle_timeout_seconds = 120
  ) noexcept;
 private:
  void AddThread(tb_u32 size = 1);
  void InitThreadMonitor();
  DISALLOW_COPY_AND_ASSIGN(ThreadPool)
};

} // turbo

#endif //TURBONET_SRC_BASE_THREAD_POOL_H_
