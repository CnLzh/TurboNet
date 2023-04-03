#include "epoll_poller.h"
#include "logger.h"
#include "channel.h"

#include <cassert>
#include <cerrno>
#include <sys/epoll.h>

namespace turbo {

const int EpollPoller::kInitEventListSize = 16;

EpollPoller::EpollPoller(EventLoop *loop)
	: Poller(loop),
	  epoll_fd_(::epoll_create(EPOLL_CLOEXEC)),
	  event_list_(kInitEventListSize) {
}

EpollPoller::~EpollPoller() {
  ::close(epoll_fd_);
}

void EpollPoller::RemoveChannel(Channel *channel) {

}

Timestamp EpollPoller::Poll(int timeout, Poller::ChannelList *active_channels) {
  LOG_DEBUG << "epoll fd total count " << channel_map_.size();
  int num_events = ::epoll_wait(epoll_fd_,
								event_list_.data(),
								static_cast<int>(event_list_.size()),
								timeout);
  int errno_info = errno;
  Timestamp now(Timestamp::Now());
  if (num_events > 0) {
	LOG_DEBUG << num_events << "events happened";
	FillActiveChannels(num_events, active_channels);
	if (static_cast<size_t>(num_events) == event_list_.size()) {
	  event_list_.resize(event_list_.size() * 2);
	}
  } else if (num_events == 0) {
	LOG_DEBUG << "Nothing happened";
  } else {
	if (errno_info != EINTR) {
	  errno = errno_info;
	  LOG_SYS_ERROR << "EpollPoller::Poll()";
	}
  }
  return now;
}

void EpollPoller::UpdateChannel(Channel *channel) {

}

void EpollPoller::FillActiveChannels(int num_events, Poller::ChannelList *active_channels) const {

}

} // turbo