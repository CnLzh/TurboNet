// copyright (c) 2023
//
// file:     socket_ops.h
// brief:    Encapsulated socket interface.
// author:   CnLzh

#ifndef TURBONET_SRC_NET_SOCKET_OPS_H_
#define TURBONET_SRC_NET_SOCKET_OPS_H_

#include <arpa/inet.h>

namespace turbo::sockets {

const struct sockaddr *SockaddrCast(const struct sockaddr_in *addr);
struct sockaddr *SockaddrCast(struct sockaddr_in *addr);
const struct sockaddr *SockaddrCast(const struct sockaddr_in6 *addr);
struct sockaddr *SockaddrCast(struct sockaddr_in6 *addr);

const struct sockaddr_in *SockaddrInCast(const struct sockaddr *addr);
const struct sockaddr_in6 *SockaddrIn6Cast(const struct sockaddr *addr);

int CreateNoBlockingOrDie(sa_family_t family);
int Connect(int sockfd, const struct sockaddr *addr);
void BindOrDie(int sockfd, const struct sockaddr *addr);
void ListenOrDie(int sockfd);
int AcceptOrDie(int sockfd, struct sockaddr_in6 *addr);
ssize_t Read(int sockfd, void *buf, size_t cnt);
ssize_t ReadV(int sockfd, const struct iovec *iov, int iovcnt);
size_t Write(int sockfd, const void *buf, size_t cnt);
void CloseOrDie(int sockfd);
void ShutdownWriteOrDie(int sockfd);

void IpToString(char *buf, size_t size, const struct sockaddr *addr);
void IpPortToString(char *buf, size_t size, const struct sockaddr *addr);
void IpFromString(const char *ip, struct sockaddr_in *addr);
void IpFromString(const char *ip, struct sockaddr_in6 *addr);
void IpPortFromString(const char *ip, uint16_t port, struct sockaddr_in *addr);
void IpPortFromString(const char *ip, uint16_t port, struct sockaddr_in6 *addr);

int GetSocketError(int sockfd);

struct sockaddr_in6 GetLocalAddr(int sockfd);
struct sockaddr_in6 GetPeerAddr(int sockfd);
bool IsSelfConnect(int sockfd);

} // turbo::sockets

#endif //TURBONET_SRC_NET_SOCKET_OPS_H_
