// copyright (c) 2023
//
// file:     inet_address.h
// brief:    InetAddress class.
// author:   CnLzh

#ifndef TURBONET_SRC_NET_INET_ADDRESS_H_
#define TURBONET_SRC_NET_INET_ADDRESS_H_

#include <netinet/in.h>

#include "public_define.h"

namespace turbo {

class InetAddress {
 public:
 private:
  union {
   struct sockaddr_in addr_;
   struct sockaddr_in6 addr6_;
 };
};

} // turbo

#endif //TURBONET_SRC_NET_INET_ADDRESS_H_
