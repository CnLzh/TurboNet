#include "socket_ops.h"
#include "logger.h"
#include "inet_endian.h"

#include <cassert>
#include <sys/uio.h>
namespace turbo::sockets {

const struct sockaddr *SockaddrCast(const struct sockaddr_in *addr) {
  return static_cast<const struct sockaddr *>(implicit_cast<const void *>(addr));
}

struct sockaddr *SockaddrCast(struct sockaddr_in *addr) {
  return static_cast<struct sockaddr *>(implicit_cast<void *>(addr));
}

const struct sockaddr *SockaddrCast(const struct sockaddr_in6 *addr) {
  return static_cast<const struct sockaddr *>(implicit_cast<const void *>(addr));
}

struct sockaddr *SockaddrCast(struct sockaddr_in6 *addr) {
  return static_cast<struct sockaddr *>(implicit_cast<void *>(addr));
}

const struct sockaddr_in *SockaddrInCast(const struct sockaddr *addr) {
  return static_cast<const struct sockaddr_in *>(implicit_cast<const void *>(addr));
}

const struct sockaddr_in6 *SockaddrIn6Cast(const struct sockaddr *addr) {
  return static_cast<const struct sockaddr_in6 *>(implicit_cast<const void *>(addr));
}

int CreateNoBlockingSocketOrDie(sa_family_t family) {
  int sockfd = ::socket(family, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
  if (sockfd < 0)
	LOG_SYS_FATAL << "CreateNoBlockOrDie, error = " << sockfd;
  return sockfd;
}

int Connect(int sockfd, const struct sockaddr *addr) {
  return ::connect(sockfd, addr, static_cast<socklen_t>(sizeof(struct sockaddr_in6)));
}

void BindOrDie(int sockfd, const struct sockaddr *addr) {
  int ret = ::bind(sockfd, addr, static_cast<socklen_t>(sizeof(struct sockaddr_in6)));
  if (ret < 0)
	LOG_SYS_FATAL << "BindOrDie, error = " << ret;
}

void ListenOrDie(int sockfd) {
  int ret = ::listen(sockfd, SOMAXCONN);
  if (ret < 0)
	LOG_SYS_FATAL << "ListenOrDie, error = " << ret;
}

int AcceptOrDie(int sockfd, struct sockaddr_in6 *addr) {
  auto addr_len = static_cast<socklen_t>(sizeof(*addr));
  int connect_fd = ::accept4(sockfd, SockaddrCast(addr), &addr_len, SOCK_NONBLOCK | SOCK_CLOEXEC);
  if (connect_fd < 0)
	LOG_SYS_FATAL << "AcceptOrDie, error = " << connect_fd;
  return connect_fd;
}

ssize_t Read(int sockfd, void *buf, size_t cnt) {
  return ::read(sockfd, buf, cnt);
}

ssize_t ReadV(int sockfd, const struct iovec *iov, int iovcnt) {
  return ::readv(sockfd, iov, iovcnt);
}

size_t Write(int sockfd, const void *buf, size_t cnt) {
  return ::write(sockfd, buf, cnt);
}

void CloseOrDie(int sockfd) {
  int ret = ::close(sockfd);
  if (ret < 0)
	LOG_SYS_ERROR << "CloseOrDie, error = " << ret;
}

void ShutdownWriteOrDie(int sockfd) {
  int ret = ::shutdown(sockfd, SHUT_WR);
  if (ret < 0)
	LOG_SYS_ERROR << "ShutdownWriteOrDie, error = " << ret;
}

void IpToString(char *buf, size_t size, const struct sockaddr *addr) {
  if (addr->sa_family == AF_INET) {
	assert(size >= INET_ADDRSTRLEN);
	const struct sockaddr_in *addr4 = SockaddrInCast(addr);
	::inet_ntop(AF_INET, &addr4->sin_addr, buf, static_cast<socklen_t>(size));
  } else if (addr->sa_family == AF_INET6) {
	assert(size >= INET6_ADDRSTRLEN);
	const struct sockaddr_in6 *addr6 = SockaddrIn6Cast(addr);
	::inet_ntop(AF_INET6, &addr6->sin6_addr, buf, static_cast<socklen_t>(size));
  }
}

void IpPortToString(char *buf, size_t size, const struct sockaddr *addr) {
  IpToString(buf, size, addr);
  size_t end = ::strlen(buf);
  const struct sockaddr_in *addr4 = SockaddrInCast(addr);
  uint16_t port = NetworkToHost16(addr4->sin_port);
  assert(size > end);
  snprintf(buf + end, size - end, ":%u", port);
}

void IpFromString(const char *ip, struct sockaddr_in *addr) {
  addr->sin_family = AF_INET;
  int ret = ::inet_pton(AF_INET, ip, &addr->sin_addr);
  if (ret <= 0)
	LOG_SYS_ERROR << "IpFromString, error = " << ret;
}

void IpFromString(const char *ip, struct sockaddr_in6 *addr) {
  addr->sin6_family = AF_INET6;
  int ret = inet_pton(AF_INET6, ip, &addr->sin6_addr);
  if (ret <= 0)
	LOG_SYS_ERROR << "IpFromString, error = " << ret;
}
void IpPortFromString(const char *ip, uint16_t port, struct sockaddr_in *addr) {
  IpFromString(ip, addr);
  addr->sin_port = HostToNetwork16(port);
}

void IpPortFromString(const char *ip, uint16_t port, struct sockaddr_in6 *addr) {
  IpFromString(ip, addr);
  addr->sin6_port = HostToNetwork16(port);
}

int GetSocketError(int sockfd) {
  int opt_val = 0;
  auto opt_len = static_cast<socklen_t>(opt_val);
  if (::getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &opt_val, &opt_len) < 0)
	return errno;
  else
	return opt_val;
}

struct sockaddr_in6 GetLocalAddr(int sockfd) {
  struct sockaddr_in6 local_addr{};
  MemZero(&local_addr, sizeof(local_addr));
  auto addr_len = static_cast<socklen_t>(sizeof(local_addr));
  int ret = ::getsockname(sockfd, SockaddrCast(&local_addr), &addr_len);
  if (ret < 0)
	LOG_SYS_ERROR << "GetLocalAddr, error = " << ret;
  return local_addr;
}

struct sockaddr_in6 GetPeerAddr(int sockfd) {
  struct sockaddr_in6 peer_addr{};
  MemZero(&peer_addr, sizeof(peer_addr));
  auto addr_len = static_cast<socklen_t>(sizeof(peer_addr));
  int ret = ::getpeername(sockfd, SockaddrCast(&peer_addr), &addr_len);
  if (ret < 0)
	LOG_SYS_ERROR << "GetPeerAddr, error = " << ret;
  return peer_addr;
}

bool IsSelfConnect(int sockfd) {
  struct sockaddr_in6 local_addr = GetLocalAddr(sockfd);
  struct sockaddr_in6 peer_addr = GetPeerAddr(sockfd);
  if (local_addr.sin6_family == AF_INET) {
	const struct sockaddr_in *local_addr4 = reinterpret_cast<struct sockaddr_in *>(&local_addr);
	const struct sockaddr_in *peer_addr4 = reinterpret_cast<struct sockaddr_in *>(&peer_addr);
	return local_addr4->sin_port == peer_addr4->sin_port &&
		local_addr4->sin_addr.s_addr == peer_addr4->sin_addr.s_addr;
  } else if (local_addr.sin6_family == AF_INET6) {
	return local_addr.sin6_port == peer_addr.sin6_port &&
		memcmp(&local_addr.sin6_addr, &peer_addr.sin6_addr, sizeof(local_addr.sin6_addr)) == 0;
  } else {
	return false;
  }
}

} // turbo::sockets