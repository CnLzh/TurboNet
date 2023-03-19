#include "mutex.h"
#include "current_thread.h"

#include <cassert>
namespace turbo {

MutexLock::MutexLock() noexcept
	: holder_(0) {
}

MutexLock::~MutexLock() noexcept {
  assert(holder_ == 0);
}

void MutexLock::AssignHolder() {
  holder_ = CurrentThread::CachedTid();
}

void MutexLock::UnAssignHolder() {
  holder_ = 0;
}

void MutexLock::Lock() {
  mutex_.lock();
  AssignHolder();
}

void MutexLock::UnLock() {
  UnAssignHolder();
  mutex_.unlock();
}

bool MutexLock::IsLockedByThisThread() const {
  return holder_ == CurrentThread::CachedTid();
}

void MutexLock::AssertLocked() const {
  assert(IsLockedByThisThread());
}

std::mutex &MutexLock::GetMutex() {
  return mutex_;
}

MutexLock::UnAssignGuard::UnAssignGuard(MutexLock &owner) noexcept
	: owner_(owner) {
  owner_.UnAssignHolder();
}

MutexLock::UnAssignGuard::~UnAssignGuard() noexcept{
  owner_.AssignHolder();
}

MutexLockGuard::MutexLockGuard(MutexLock &mutex)
	: mutex_(mutex) {
  mutex_.Lock();
}

MutexLockGuard::~MutexLockGuard() {
  mutex_.UnLock();
}

}