/// Copyright (C) 2023
//
// File:     timestamp_test.h
// Brief:    Timestamp until test.
// Author:   CnLzh

#include "timestamp.h"
#include <iostream>

using namespace std;
using namespace turbo;

int main() {
  Timestamp time_1 = Timestamp::Now();
  cout << "time_1:" << endl;
  cout << time_1.ToString() << endl;
  cout << time_1.ToFormatString(true) << endl << endl;

  Timestamp time_2 = Timestamp::Now();
  cout << "time_2:" << endl;
  cout << time_2.ToString() << endl;
  cout << time_2.ToFormatString(true) << endl << endl;

  Timestamp time_3 = AddTime(time_1, 1.23);
  cout << "time_3:" << endl;
  cout << time_3.ToString() << endl;
  cout << time_3.ToFormatString(true) << endl << endl;

  std::cerr << "Test passed." << std::endl;
  return 0;
}