#include "thread_pool.h"
#include "current_thread.h"

#include <cassert>

namespace turbo {

ThreadPool::ThreadPool(
	std::string thread_pool_name,
	const tb_u32 &max_pool_size,
	const tb_u32 &core_pool_size,
	const tb_u32 &max_task_size,
	const tb_f64 &idle_timeout_seconds
) noexcept
	: thread_pool_name_(std::move(thread_pool_name)),
	  kMaxPoolSize_(max_pool_size),
	  kCorePoolSize_(core_pool_size),
	  kMaxTaskSize_(max_task_size),
	  kIdleTimeoutSeconds_(idle_timeout_seconds),
	  thread_pool_mutex_(),
	  tasks_mutex_(),
	  task_cv_(tasks_mutex_),
	  monitor_mutex_(),
	  monitor_cv_(monitor_mutex_),
	  thread_pool_status_(TPS_RUNNING),
	  idle_thread_num_(0),
	  now_thread_num_(0) {
  InitThreadMonitor();
}

void ThreadPool::AddThread(tb_u32 size) {
  assert(thread_pool_status_ == TPS_RUNNING);
  MutexLockGuard thread_pool_lock(thread_pool_mutex_);
  for (; now_thread_num_ < kMaxPoolSize_ && size > 0; --size) {
	std::unique_ptr<Thread> thread = std::make_unique<Thread>(
		[this] {
		  while (true) {
			Task task;
			{
			  MutexLockGuard task_lock(tasks_mutex_);
			  while (thread_pool_status_ != TPS_TERMINATED && tasks_.empty()) {
				if (task_cv_.WaitForSeconds(kIdleTimeoutSeconds_)) {
				  if (now_thread_num_ > kCorePoolSize_) {
					--idle_thread_num_;
					--now_thread_num_;
					MutexLockGuard monitor_lock(monitor_mutex_);
					monitor_thread_free_.emplace_back(CurrentThread::CachedTid());
					monitor_cv_.Notify();
					return;
				  }
				}
			  }
			  if (thread_pool_status_ == TPS_TERMINATED && tasks_.empty()) {
				--idle_thread_num_;
				--now_thread_num_;
				return;
			  }
			  task = std::move(tasks_.front());
			  tasks_.pop_front();
			  if (thread_pool_status_ == TPS_SHUTDOWN && tasks_.empty())
				thread_pool_status_ = TPS_TERMINATED;
			}
			--idle_thread_num_;
			task();
			++idle_thread_num_;
		  }
		},
		std::string("ThreadPool Work") + std::to_string(thread_pool_.size())
	);
	thread_pool_.emplace_back(std::move(thread));
	++idle_thread_num_;
	++now_thread_num_;
	thread_pool_.back()->Start();
  }
}

void ThreadPool::InitThreadMonitor() {
  Thread monitor(
	  [this] {
		while (true) {
		  MutexLockGuard monitor_lock(monitor_mutex_);
		  while (monitor_thread_free_.empty()) {
			if (monitor_cv_.WaitForSeconds(kIdleTimeoutSeconds_) && thread_pool_status_ == TPS_TERMINATED)
			  return;
		  }
		  auto it = monitor_thread_free_.begin();
		  while (it != monitor_thread_free_.end()) {
			{
			  MutexLockGuard thread_pool_lock(thread_pool_mutex_);
			  auto ret = std::find_if(
				  thread_pool_.begin(),
				  thread_pool_.end(),
				  [=](std::unique_ptr<Thread> &t) {
					return t->Tid() == *it;
				  }
			  );
			  if (ret != thread_pool_.end()) {
				(*ret)->Join();
				thread_pool_.erase(ret);
			  }
			}
			it = monitor_thread_free_.erase(it);
		  }
		}
	  },
	  "ThreadPool Monitor"
  );
  monitor.Start();
}

} // turbo