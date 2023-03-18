// Copyright (C) 2023
//
// File:     mutex_test.h
// Brief:    Mutex until test.
// Author:   CnLzh
#include "mutex.h"
#include <iostream>

using namespace std;
using namespace turbo;

int main() {
  MutexLock mutex_test;
  {
	MutexLockGuard lock(mutex_test);
	mutex_test.AssertLocked();
	if (!mutex_test.IsLockedByThisThread()) {
	  std::cerr << "Test failed: " << __LINE__ << endl;
	}
  }
  if (mutex_test.IsLockedByThisThread()) {
	std::cerr << "Test failed: " << __LINE__ << endl;
  }

  std::cerr << "Test passed." << std::endl;
  return 0;
}