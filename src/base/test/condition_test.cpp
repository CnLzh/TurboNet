/// Copyright (C) 2023
//
// File:     condition_test.h
// Brief:    Condition until test.
// Author:   CnLzh

#include "condition.h"
#include <iostream>
#include <thread>
using namespace std;
using namespace turbo;

MutexLock mutex_test;
Condition cond_test(mutex_test);

void WaitThread() {
  MutexLockGuard lock(mutex_test);
  cond_test.Wait();
  cout << "Wait Thread Notify" << endl;
}

void NotifyThread() {
  sleep(1);
  cond_test.Notify();
}

int main() {
  thread th_1(WaitThread);
  thread th_2(NotifyThread);
  th_1.join();
  th_2.join();

  std::cerr << "Test passed." << std::endl;
  return 0;
}