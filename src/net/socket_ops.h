// copyright (c) 2023
//
// file:     socket_ops.h
// brief:    Encapsulated socket interface.
// author:   CnLzh

#ifndef TURBONET_SRC_NET_SOCKET_OPS_H_
#define TURBONET_SRC_NET_SOCKET_OPS_H_

#include <arpa/inet.h>

namespace turbo{

int CreateNoBlockingOrDie(sa_family_t family);

}

#endif //TURBONET_SRC_NET_SOCKET_OPS_H_
