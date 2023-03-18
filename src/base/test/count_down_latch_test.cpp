// Copyright (C) 2023
//
// File:     count_down_latch_test.h
// Brief:    CountDownLatch until test.
// Author:   CnLzh

#include <count_down_latch.h>
#include <iostream>

using namespace std;
using namespace turbo;

CountDownLatch cdl(5);

void WaitOne() {
  cdl.Wait();
  cout << "Wait One notify: " << cdl.GetCount() << endl;
}

void WaitTwo() {
  cdl.Wait();
  cout << "Wait two Notify:" << cdl.GetCount() << endl;
}

int main() {
  thread th_1(WaitOne);
  thread th_2(WaitTwo);

  for (int i = 0; i < 5; i++) {
	cdl.CountDown();
	cout << "CountDownLatch count = " << cdl.GetCount() << endl;
  }

  th_1.join();
  th_2.join();

  std::cerr << "Test passed." << std::endl;
  return 0;
}