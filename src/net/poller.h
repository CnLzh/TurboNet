// copyright (c) 2023
//
// file:     poller.h
// brief:    Poller class.
// author:   CnLzh

#ifndef TURBONET_SRC_NET_POLLER_H_
#define TURBONET_SRC_NET_POLLER_H_

#include "public_define.h"

#include <vector>
#include <map>
namespace turbo {

class EventLoop;
class Channel;

class Poller {
 public:
  using ChannelList = std::vector<Channel *>;

  explicit Poller(EventLoop *loop);
  ~Poller() = default;

 protected:
  using ChannelMap = std::map<int, Channel *>;
  ChannelMap channel_map_;

 private:
  EventLoop *loop_;
  DISALLOW_COPY_AND_ASSIGN(Poller)
};

} // turbo

#endif //TURBONET_SRC_NET_POLLER_H_
