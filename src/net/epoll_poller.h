// copyright (c) 2023
//
// file:     epoll_poller.h
// brief:    EpollPoller class inherits from Poller and encapsulates epoll.
// author:   CnLzh

#ifndef TURBONET_SRC_NET_EPOLL_POLLER_H_
#define TURBONET_SRC_NET_EPOLL_POLLER_H_

#include "poller.h"

#include <vector>

struct epoll_event;

namespace turbo {

class EpollPoller : public Poller {
 public:
  explicit EpollPoller(EventLoop *loop);
  ~EpollPoller() override;

  Timestamp Poll(int timeout, ChannelList *active_channels) override;

  void UpdateChannel(Channel *channel) override;
  void RemoveChannel(Channel *channel) override;

 private:
  void FillActiveChannels(int num_events, ChannelList* active_channels) const;

  static const int kInitEventListSize;
  using EventList = std::vector<struct epoll_event>;

  int epoll_fd_;
  EventList event_list_;
};

} // turbo

#endif //TURBONET_SRC_NET_EPOLL_POLLER_H_
