// copyright (c) 2023
//
// file:     inet_address.h
// brief:    InetAddress class.
// author:   CnLzh

#ifndef TURBONET_SRC_NET_INET_ADDRESS_H_
#define TURBONET_SRC_NET_INET_ADDRESS_H_

#include <netinet/in.h>
#include <string>
#include <array>

namespace turbo {

class InetAddress {
 public:
  explicit InetAddress(uint16_t port, bool loop_back_only = false, bool ipv6 = false);
  InetAddress(const std::string &ip, uint16_t port, bool ipv6 = false);
  explicit InetAddress(const struct sockaddr_in &addr);
  explicit InetAddress(const struct sockaddr_in6 &addr6);

  [[nodiscard]] const struct sockaddr *GetSockAddr() const;
  void SetSockAddrInet4(const struct sockaddr_in &addr);
  void SetSockAddrInet6(const struct sockaddr_in6 &addr6);

  [[nodiscard]] sa_family_t Family() const;

  [[nodiscard]] uint16_t PortNet() const;
  [[nodiscard]] uint16_t PortHost() const;

  [[nodiscard]] uint32_t Ipv4Net() const;
  [[nodiscard]] uint32_t Ipv4Host() const;
  [[nodiscard]] const uint8_t *Ipv6() const;

  [[nodiscard]] std::string IpToString() const;
  [[nodiscard]] std::string IpPortToString() const;

  static bool Resolve(const std::string &host_name, std::string &ip);

 private:
  // TODO: delete AddressType
  enum AddressType {
	ADDRESS_TYPE_IPV4,
	ADDRESS_TYPE_IPV6,
  };

  union {
	struct sockaddr_in addr_{};
	struct sockaddr_in6 addr6_;
  };

  AddressType using_type_;
};

} // turbo

#endif //TURBONET_SRC_NET_INET_ADDRESS_H_
