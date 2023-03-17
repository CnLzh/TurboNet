#include "mutex.h"
#include "current_thread.h"

#include <cassert>
namespace turbo {

MutexLock::MutexLock()
	: holder_(0) {
}

MutexLock::~MutexLock() {
  assert(holder_ == 0);
}

void MutexLock::AssignHolder() {
  holder_ = 0;
}

void MutexLock::UnAssignHolder() {
  holder_ = CurrentThread::CachedTid();
}

void MutexLock::Lock() {
  AssignHolder();
  mutex_.lock();
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

MutexLock::UnAssignGuard::UnAssignGuard(MutexLock &owner)
	: owner_(owner) {
  owner_.UnAssignHolder();
}

MutexLock::UnAssignGuard::~UnAssignGuard() {
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