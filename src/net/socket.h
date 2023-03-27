// copyright (c) 2023
//
// file:     socket.h
// brief:    Socket class.
// author:   CnLzh

#ifndef TURBONET_SRC_NET_SOCKET_H_
#define TURBONET_SRC_NET_SOCKET_H_

#include "public_define.h"

struct tcp_info;

namespace turbo {

class InetAddress;

class Socket {
 public:
  explicit Socket(int sockfd);
  ~Socket();

  [[nodiscard]] int Fd() const;
  bool GetTcpInfo(struct tcp_info *) const;
  bool GetTcpInfoString(char *buf, int len) const;

  void BindAddress(const InetAddress &addr) const;
  void Listen() const;
  int Accept(InetAddress *peer_addr) const;
  void ShutdownWrite() const;

  void SetTcpNoDelay(bool on) const;
  void SetReuseAddr(bool on) const;
  void SetReusePort(bool on) const;
  void SetKeepAlive(bool on) const;

 private:
  const int sockfd_;
  DISALLOW_COPY_AND_ASSIGN(Socket)
};

} // turbo

#endif //TURBONET_SRC_NET_SOCKET_H_
