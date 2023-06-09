// copyright (c) 2023
//
// file:     endian.h
// brief:    Conversion between byte orders.
// author:   CnLzh

#ifndef TURBONET_SRC_NET_INET_ENDIAN_H_
#define TURBONET_SRC_NET_INET_ENDIAN_H_

#include <cstdint>
#include <endian.h>

namespace turbo::sockets {

inline uint64_t HostToNetwork64(uint64_t host64) {
  return htobe64(host64);
}

inline uint32_t HostToNetwork32(uint32_t host32) {
  return htobe32(host32);
}

inline uint16_t HostToNetwork16(uint16_t host16) {
  return htobe16(host16);
}

inline uint64_t NetworkToHost64(uint64_t net64) {
  return be64toh(net64);
}

inline uint32_t NetworkToHost32(uint32_t net32) {
  return be32toh(net32);
}

inline uint16_t NetworkToHost16(uint16_t net16) {
  return be16toh(net16);
}

} // turbo::sockets
#endif //TURBONET_SRC_NET_INET_ENDIAN_H_
