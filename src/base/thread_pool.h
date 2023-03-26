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
#include <future>
#include <cassert>

namespace turbo {

class ThreadPool final {

  enum ThreadPoolStatus {
	TPS_RUNNING,
	TPS_SHUTDOWN,
	TPS_TERMINATED,
  };

  using Task = std::function<void()>;

  std::string thread_pool_name_;

  const unsigned int kMaxPoolSize_;
  const unsigned int kCorePoolSize_;
  const unsigned int kMaxTaskSize_;
  const double kIdleTimeoutSeconds_;

  std::list<std::unique_ptr<Thread>> thread_pool_;
  mutable MutexLock thread_pool_mutex_;

  std::deque<Task> tasks_;
  mutable MutexLock tasks_mutex_;
  Condition task_cv_;

  std::list<pid_t> monitor_thread_free_;
  mutable MutexLock monitor_mutex_;
  Condition monitor_cv_;

  std::atomic<ThreadPoolStatus> thread_pool_status_;
  std::atomic<unsigned int> idle_thread_num_;
  std::atomic<unsigned int> now_thread_num_;

  std::unique_ptr<Thread> thread_monitor_;

 public:
  explicit ThreadPool(
	  std::string thread_pool_name = "ThreadPool",
	  const unsigned int &max_pool_size = std::thread::hardware_concurrency(),
	  const unsigned int &core_pool_size = std::thread::hardware_concurrency() / 2,
	  const unsigned int &max_task_size = 1000,
	  const double &idle_timeout_seconds = 120
  ) noexcept;

  ~ThreadPool() noexcept;

  template<typename F, typename ... Args>
  auto Commit(F &&f, Args &&... args) -> std::future<decltype(f(args...))>;

  std::string Name() const;

  unsigned int IdleThreadCount() const;

  size_t ThreadCount() const;

  void ShutDown();

 private:
  void AddThread(unsigned int size = 1);
  void InitThreadMonitor();
  DISALLOW_COPY_AND_ASSIGN(ThreadPool)
};

template<typename F, typename ... Args>
auto ThreadPool::Commit(F &&f, Args &&...args) -> std::future<decltype(f(args...))> {
  assert(thread_pool_status_ == TPS_RUNNING);

  using RetType = decltype(f(args...));

  auto task = std::make_shared<std::packaged_task<RetType()>>(
	  std::bind(std::forward<F>(f), std::forward<Args>(args)...)
  );

  std::future<RetType> result = task->get_future();

  {
	MutexLockGuard task_lock(tasks_mutex_);
	if (kMaxTaskSize_ > tasks_.size()) {
	  tasks_.emplace_back(
		  [task]() {
			(*task)();
		  });
	} else {
	  AddThread();
	  tasks_.emplace_back(
		  [task]() {
			(*task)();
		  });
	}
  }
  task_cv_.Notify();

  return result;
}

} // turbo

#endif //TURBONET_SRC_BASE_THREAD_POOL_H_
