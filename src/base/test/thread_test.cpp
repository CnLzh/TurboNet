/// Copyright (C) 2023
//
// File:     thread_test.h
// Brief:    thread until test.
// Author:   CnLzh

#include "thread.h"

#include <iostream>

void AddTest(int a, int b) {
  std::cout << "AddTest: " << a + b << std::endl;
}

void MinusTest(int a, int b) {
  std::cout << "MinusTest" << a - b << std::endl;
}

int main() {
  std::function<void()> func_add = [] { return AddTest(1, 2); };
  turbo::Thread t_add(func_add, "ThreadAdd");

  std::function<void()> func_minus = [] { return MinusTest(2, 1); };
  turbo::Thread t_minus(func_minus);

  std::cout << "thread add" << std::endl;
  std::cout << "tid: " << t_add.Tid() << std::endl;
  std::cout << "name: " << t_add.Name() << std::endl;
  std::cout << "started: " << t_add.Started() << std::endl << std::endl;

  std::cout << "thread minus" << std::endl;
  std::cout << "tid: " << t_minus.Tid() << std::endl;
  std::cout << "name: " << t_minus.Name() << std::endl;
  std::cout << "started: " << t_minus.Started() << std::endl << std::endl;

  std::cout << "thread Num: " << turbo::Thread::NumCreated() << std::endl << std::endl;

  t_add.Start();
  t_minus.Start();

  std::cout << "thread add tid: " << t_add.Tid() << std::endl;
  std::cout << "thread minus tid: " << t_minus.Tid() << std::endl;

  t_add.Join();
  t_minus.Join();

  std::cerr << "Test passed." << std::endl;
  return 0;
}