#include "public_define.h"
#include "inet_address.h"
#include "endian.h"
#include "socket_ops.h"

#include <cassert>
#include <cstdint>
#include <netdb.h>
namespace turbo {

#define CHECK_IPV4(type) (type == ADDRESS_TYPE_IPV4)

InetAddress::InetAddress(uint16_t port, bool loop_back_only, bool ipv6) {
  if (ipv6) {
	MemZero(&addr6_, sizeof(addr6_));
	sockets::PortFromString(port, &addr6_, loop_back_only);
	using_type_ = ADDRESS_TYPE_IPV6;
  } else {
	MemZero(&addr_, sizeof(addr_));
	sockets::PortFromString(port, &addr_, loop_back_only);
	using_type_ = ADDRESS_TYPE_IPV4;
  }
}

InetAddress::InetAddress(const std::string &ip, uint16_t port, bool ipv6) {
  if (ipv6 || ip.find_first_of(':') != std::string::npos) {
	MemZero(&addr6_, sizeof(addr6_));
	sockets::IpPortFromString(ip.c_str(), port, &addr6_);
	using_type_ = ADDRESS_TYPE_IPV6;
  } else {
	MemZero(&addr_, sizeof(addr_));
	sockets::IpPortFromString(ip.c_str(), port, &addr_);
	using_type_ = ADDRESS_TYPE_IPV4;
  }
}
InetAddress::InetAddress(const sockaddr_in &addr)
	: addr_(addr),
	  using_type_(ADDRESS_TYPE_IPV4) {

}

InetAddress::InetAddress(const sockaddr_in6 &addr6)
	: addr6_(addr6),
	  using_type_(ADDRESS_TYPE_IPV6) {

}

sa_family_t InetAddress::Family() const {
  if (CHECK_IPV4(using_type_))
	return addr_.sin_family;
  return addr6_.sin6_family;
}

const struct sockaddr *InetAddress::GetSockAddr() const {
  if (CHECK_IPV4(using_type_))
	return sockets::SockaddrCast(&addr_);
  return sockets::SockaddrCast(&addr6_);
}

void InetAddress::SetSockAddrInet4(const sockaddr_in &addr) {
  addr_ = addr;
}

void InetAddress::SetSockAddrInet6(const sockaddr_in6 &addr6) {
  addr6_ = addr6;
}

std::string InetAddress::IpToString() const {
  char buf[64]{};
  sockets::IpToString(buf, sizeof(buf), GetSockAddr());
  return buf;
}

std::string InetAddress::IpPortToString() const {
  char buf[64]{};
  sockets::IpPortToString(buf, sizeof(buf), GetSockAddr());
  return buf;
}

uint16_t InetAddress::PortNet() const {
  if (CHECK_IPV4(using_type_))
	return addr_.sin_port;
  return addr6_.sin6_port;
}

uint16_t InetAddress::PortHost() const {
  return sockets::NetworkToHost16(PortNet());
}

uint32_t InetAddress::Ip4Net() const {
  assert(CHECK_IPV4(using_type_));
  return addr_.sin_addr.s_addr;
}
uint32_t InetAddress::Ip4Host() const {
  assert(CHECK_IPV4(using_type_));
  return sockets::NetworkToHost32(addr_.sin_addr.s_addr);
}

const uint8_t *InetAddress::Ip6() const {
  return addr6_.sin6_addr.__in6_u.__u6_addr8;
}

static thread_local char t_resolve_buffer[64 * 1024]{};
bool InetAddress::Resolve(const std::string &host_bane, InetAddress *result) {
  assert(result);
  // TODO: getaddrinfo_a
  return false;
}

} // turbo