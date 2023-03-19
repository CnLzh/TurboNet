#include "condition.h"

namespace turbo {

Condition::Condition(MutexLock &mutex) noexcept
	: mutex_(mutex),
	  lock_(mutex_.GetMutex(), std::adopt_lock) {

}

Condition::~Condition() noexcept {
  lock_.release();
}

void Condition::Wait() {
  mutex_.AssertLocked();
  MutexLock::UnAssignGuard ug(mutex_);
  cond_.wait(lock_);
}

void Condition::Notify() {
  cond_.notify_one();
}

void Condition::NotifyAll() {
  cond_.notify_all();
}

bool Condition::WaitForSeconds(const tb_f64 &seconds) {
  mutex_.AssertLocked();
  auto nano_seconds = static_cast<tb_s64>(seconds * kNanoSecondsPerSecond);
  MutexLock::UnAssignGuard ug(mutex_);
  return cond_.wait_for(lock_, std::chrono::nanoseconds(nano_seconds)) == std::cv_status::timeout;
}

} // turbo
