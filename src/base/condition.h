// Copyright (C) 2023
//
// File:     condition.h
// Brief:    Condition class.
// Author:   CnLzh

#ifndef TURBONET_SRC_BASE_CONDITION_H_
#define TURBONET_SRC_BASE_CONDITION_H_

#include "public_define.h"
#include "mutex.h"

#include <condition_variable>

namespace turbo {

class Condition final {
 public:
  explicit Condition(MutexLock& mutex);
  ~Condition();

  void Wait();
  // return true if timeout, false otherwise.
  bool WaitForSeconds(const tb_f64& seconds);

  void Notify();
  void NotifyAll();

 private:
  MutexLock& mutex_;
  std::unique_lock<std::mutex> lock_;
  std::condition_variable cond_;
  DISALLOW_COPY_AND_ASSIGN(Condition)
};

} // turbo
#endif //TURBONET_SRC_BASE_CONDITION_H_
