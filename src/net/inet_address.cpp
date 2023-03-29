#include "public_define.h"
#include "inet_address.h"
#include "inet_endian.h"
#include "socket_ops.h"
#include "logger.h"

#include <cassert>
#include <cstdint>
#include <netdb.h>
namespace turbo {

#define CHECK_IPV4(type) (type == ADDRESS_TYPE_IPV4)

InetAddress::InetAddress(uint16_t port, bool loop_back_only, bool ipv6) {
  if (ipv6) {
	MemZero(&addr6_, sizeof(addr6_));
	addr6_.sin6_family = AF_INET6;
	addr6_.sin6_port = sockets::HostToNetwork16(port);
	addr6_.sin6_addr = loop_back_only ? in6addr_loopback : in6addr_any;
  } else {
	MemZero(&addr_, sizeof(addr_));
	addr_.sin_family = AF_INET;
	addr_.sin_port = sockets::HostToNetwork16(port);
	addr_.sin_addr.s_addr = sockets::HostToNetwork32(loop_back_only ? INADDR_LOOPBACK : INADDR_ANY);
  }
}

InetAddress::InetAddress(const std::string &ip, uint16_t port, bool ipv6) {
  if (ipv6 || ip.find_first_of(':') != std::string::npos) {
	MemZero(&addr6_, sizeof(addr6_));
	sockets::IpPortFromString(ip.c_str(), port, &addr6_);
  } else {
	MemZero(&addr_, sizeof(addr_));
	sockets::IpPortFromString(ip.c_str(), port, &addr_);
  }
}
InetAddress::InetAddress(const sockaddr_in &addr)
	: addr_(addr) {

}

InetAddress::InetAddress(const sockaddr_in6 &addr6)
	: addr6_(addr6) {

}

sa_family_t InetAddress::Family() const {
  return addr_.sin_family;
}

const struct sockaddr *InetAddress::GetSockAddr() const {
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
  return addr_.sin_port;
}

uint16_t InetAddress::PortHost() const {
  return sockets::NetworkToHost16(PortNet());
}

uint32_t InetAddress::Ipv4Net() const {
  assert(addr_.sin_family == AF_INET);
  return addr_.sin_addr.s_addr;
}
uint32_t InetAddress::Ipv4Host() const {
  assert(addr_.sin_family == AF_INET);
  return sockets::NetworkToHost32(addr_.sin_addr.s_addr);
}

const uint8_t *InetAddress::Ipv6() const {
  assert(addr6_.sin6_family == AF_INET6);
  return addr6_.sin6_addr.__in6_u.__u6_addr8;
}

bool InetAddress::Resolve(const std::string &host_name, std::string &ip) {
  auto *req = (struct gaicb *)malloc(sizeof(struct gaicb));
  MemZero(req, sizeof(struct gaicb));
  req->ar_name = host_name.c_str();

  int ret = getaddrinfo_a(GAI_WAIT, &req, 1, nullptr);
  if (ret != 0) {
	LOG_SYS_ERROR << "getaddrinfo_a, error = " << gai_strerror(ret);
	return false;
  }

  struct addrinfo *res = req->ar_result;
  char host[NI_MAXHOST];
  ret = getnameinfo(res->ai_addr, res->ai_addrlen, host, sizeof(host), nullptr, 0, NI_NUMERICHOST);
  if (ret != 0) {
	LOG_SYS_ERROR << "getnameinfo, error = " << gai_strerror(ret);
	return false;
  }
  ip = host;
  return true;
}

} // turbo