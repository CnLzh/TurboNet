#include "count_down_latch.h"

namespace turbo {

CountDownLatch::CountDownLatch(const tb_s32 &count) noexcept
	: count_(count),
	  mutex_(),
	  cond_(mutex_) {

}

void CountDownLatch::Wait() {
  MutexLockGuard lock(mutex_);
  while (count_ > 0)
	cond_.Wait();
}

void CountDownLatch::CountDown() {
  MutexLockGuard lock(mutex_);
  --count_;
  if (count_ == 0)
	cond_.NotifyAll();
}

tb_s32 CountDownLatch::GetCount() const {
  MutexLockGuard lock(mutex_);
  return count_;
}

} // turbo