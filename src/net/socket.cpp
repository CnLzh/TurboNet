#include "socket.h"
#include "inet_address.h"
#include "socket_ops.h"
#include "logger.h"

#include <netinet/in.h>
#include <netinet/tcp.h>

namespace turbo {

Socket::Socket(int sockfd)
	: sockfd_(sockfd) {

}

Socket::~Socket() {
  sockets::CloseOrDie(sockfd_);
}

int Socket::Fd() const {
  return sockfd_;
}

bool Socket::GetTcpInfo(struct tcp_info *tcpi) const {
  socklen_t len = sizeof(struct tcp_info);
  MemZero(tcpi, len);
  return ::getsockopt(sockfd_, SOL_TCP, TCP_INFO, tcpi, &len) == 0;
}

bool Socket::GetTcpInfoString(char *buf, int len) const {
  struct tcp_info tcpi{};
  if (!GetTcpInfo(&tcpi))
	return false;
  snprintf(buf, len, "retransmits=%u"
					 "rto=%u ato=%u snd_mss=%u rcv_mss=%u "
					 "lost=%u retrans=%u rtt=%u rttvar=%u "
					 "snd_ssthresh=%u snd_cwnd=%u total_retrans=%u",
		   tcpi.tcpi_retransmits,
		   tcpi.tcpi_rto,
		   tcpi.tcpi_ato,
		   tcpi.tcpi_snd_mss,
		   tcpi.tcpi_rcv_mss,
		   tcpi.tcpi_lost,
		   tcpi.tcpi_retrans,
		   tcpi.tcpi_rtt,
		   tcpi.tcpi_rttvar,
		   tcpi.tcpi_snd_ssthresh,
		   tcpi.tcpi_snd_cwnd,
		   tcpi.tcpi_total_retrans);
  return true;
}

void Socket::BindAddress(const InetAddress &addr) const {
  sockets::BindOrDie(sockfd_, addr.GetSockAddr());
}

void Socket::Listen() const {
  sockets::ListenOrDie(sockfd_);
}

int Socket::Accept(InetAddress *peer_addr) const {
  struct sockaddr_in6 addr{};
  MemZero(&addr, sizeof(addr));
  int conn_fd = sockets::AcceptOrDie(sockfd_, &addr);
  if (conn_fd >= 0)
	peer_addr->SetSockAddrInet6(addr);
  return conn_fd;
}

void Socket::ShutdownWrite() const {
  sockets::ShutdownWriteOrDie(sockfd_);
}

void Socket::SetTcpNoDelay(bool on) const {
  int opt_val = on ? 1 : 0;
  ::setsockopt(sockfd_, IPPROTO_TCP, TCP_NODELAY,
			   &opt_val, static_cast<socklen_t>(sizeof opt_val));
}

void Socket::SetReuseAddr(bool on) const {
  int opt_val = on ? 1 : 0;
  ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR,
			   &opt_val, static_cast<socklen_t>(sizeof opt_val));
}

void Socket::SetReusePort(bool on) const {
#ifdef SO_REUSEPORT
  int opt_val = on ? 1 : 0;
  int ret = ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEPORT,
						 &opt_val, static_cast<socklen_t>(sizeof opt_val));
  if (ret < 0 && on)
	LOG_SYS_ERROR << "SO_REUSEPORT failed.";
#else
  if(on)
	LOG_SYS_ERROR<<"SO_REUSEPORT is not supported,";
#endif
}

void Socket::SetKeepAlive(bool on) const {
  int opt_val = on ? 1 : 0;
  ::setsockopt(sockfd_, SOL_SOCKET, SO_KEEPALIVE,
			   &opt_val, static_cast<socklen_t>(sizeof opt_val));
}

} // turbo