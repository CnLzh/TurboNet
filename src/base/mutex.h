// File:     mutex.h
// Brief:    Encapsulation mutex.
// Author:   CnLzh
#ifndef TURBONET_SRC_BASE_MUTEX_H_
#define TURBONET_SRC_BASE_MUTEX_H_

#include "public_define.h"

#include <mutex>

namespace turbo {

class MutexLock {
 public:
  MutexLock();
  ~MutexLock();

  [[nodiscard]] bool IsLockedByThisThread() const;
  void AssertLocked() const;

 private:
  void Lock();
  void UnLock();
  std::mutex &GetMutex();

  friend class Condition;
  friend class MutexLockGuard;
  
  class UnAssignGuard {
   public:
	explicit UnAssignGuard(MutexLock &owner);
	~UnAssignGuard();
   private:
	MutexLock &owner_;
	DISALLOW_COPY_AND_ASSIGN(UnAssignGuard)
  };

  void AssignHolder();
  void UnAssignHolder();

  std::mutex mutex_;
  pid_t holder_;
  DISALLOW_COPY_AND_ASSIGN(MutexLock)
};

class MutexLockGuard {
 public:
  explicit MutexLockGuard(MutexLock &mutex);
  ~MutexLockGuard();
 private:
  MutexLock &mutex_;
  DISALLOW_COPY_AND_ASSIGN(MutexLockGuard)
};

} // turbo

#endif //TURBONET_SRC_BASE_MUTEX_H_
