/// Copyright (C) 2023
//
// File:     inet_address_test.h
// Brief:    InetAddress until test.
// Author:   CnLzh

#include "inet_address.h"
#include "socket_ops.h"

#include <iostream>

using namespace std;
using namespace turbo;

int main() {
  std::string ip;
  if (InetAddress::Resolve("www.example.com", ip)) {
	std::cout << ip << std::endl;
  }
  std::cerr << "Test passed." << std::endl;
  return 0;
}