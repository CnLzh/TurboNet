// Copyright (C) 2023
//
// File:     mutex_test.h
// Brief:    Mutex until test.
// Author:   CnLzh

#include "current_thread.h"
#include <iostream>
#include <thread>

using namespace std;
using namespace turbo;

void CurrentThreadTest() {
  CurrentThread::CachedTid();
  cout << CurrentThread::TidString() << endl;
  cout << CurrentThread::ThreadName() << endl;
  CurrentThread::t_thread_name = "Current Test";
  cout << CurrentThread::ThreadName() << endl;
}

int main() {
  thread th_1(CurrentThreadTest);
  thread th_2(CurrentThreadTest);
  th_1.join();
  th_2.join();

  std::cerr << "Test passed." << std::endl;
  return 0;
}