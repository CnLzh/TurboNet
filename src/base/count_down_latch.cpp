#include "count_down_latch.h"

namespace turbo {

CountDownLatch::CountDownLatch(int count) noexcept
	: mutex_(),
	  cond_(mutex_),
	  count_(count) {

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

int CountDownLatch::GetCount() const {
  MutexLockGuard lock(mutex_);
  return count_;
}

} // turbo