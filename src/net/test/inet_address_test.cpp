/// Copyright (C) 2023
//
// File:     inet_address_test.h
// Brief:    InetAddress until test.
// Author:   CnLzh

#include "inet_address.h"
#include "socket.h"
#include "socket_ops.h"

#include <netinet/in.h>
#include <iostream>

using namespace std;
using namespace turbo;

int main() {
  std::string ip;
  if (InetAddress::Resolve("www.example.com", ip)) {
	std::cout << ip << std::endl;
  }

#if 0 // accept test
  Socket server_fd(socket(AF_INET, SOCK_STREAM, 0));
  InetAddress server_inet(9808);
  server_fd.BindAddress(server_inet);
  server_fd.Listen();
  InetAddress client_inet;
  server_fd.Accept(&client_inet);

  cout << client_inet.Family() << endl;
  cout << client_inet.IpPortToString() << endl;
#endif

  std::cerr << "Test passed." << std::endl;
  return 0;
}