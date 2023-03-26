// Copyright (C) 2023
//
// File:     count_down_latch.h
// Brief:    CountDownLatch to provide inter-thread synchronization.
// Author:   CnLzh

#ifndef TURBONET_SRC_BASE_COUNT_DOWN_LATCH_H_
#define TURBONET_SRC_BASE_COUNT_DOWN_LATCH_H_

#include "public_define.h"
#include "condition.h"

namespace turbo {

class CountDownLatch final {
 public:
  explicit CountDownLatch(int count) noexcept;
  void Wait();
  void CountDown();
  int GetCount() const;

 private:
  mutable MutexLock mutex_;
  Condition cond_;
  int count_;
  DISALLOW_COPY_AND_ASSIGN(CountDownLatch)

};

} // turbo

#endif //TURBONET_SRC_BASE_COUNT_DOWN_LATCH_H_
