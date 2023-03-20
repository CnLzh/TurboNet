// Copyright (C) 2023
//
// File:     thread_pool_test.h
// Brief:    ThreadPool until test.
// Author:   CnLzh

#include "thread_pool.h"

#include <iostream>

using namespace std;
using namespace turbo;

int Add(int a, int b) {
  return a + b;
}

int Minus(int a, int b) {
  return a - b;
}

int Sum(int min, int max) {
  int sum = 0;
  for (int i = min; i < max; i++) {
	sum += i;
  }
  return sum;
}

int main() {

  {
	ThreadPool thread_pool("Thread_Pool_Test");
	auto result_1 = thread_pool.Commit(Add, 1, 2);
	auto result_2 = thread_pool.Commit(Minus, 3, 4);

	std::cout << "Thread Pool Name = " << thread_pool.Name() << std::endl;
	std::cout << "Thread Pool Idle Num = " << thread_pool.IdleThreadCount() << std::endl;
	std::cout << "Thread Pool Num = " << thread_pool.ThreadCount() << std::endl;

	std::cout << "Add result = " << result_1.get() << std::endl;
	std::cout << "Minus result = " << result_2.get() << std::endl;

	for (int i = 0; i < 5000; i++)
	  thread_pool.Commit(Sum, 1, 1000000);

	thread_pool.ShutDown();
  }

  std::cerr << "Test passed." << std::endl;
  return 0;
}