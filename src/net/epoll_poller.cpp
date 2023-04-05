#include "epoll_poller.h"
#include "logger.h"
#include "channel.h"

#include <cassert>
#include <cerrno>
#include <sys/epoll.h>

namespace {
const int kNew = -1;
const int kAdd = 1;
const int kDel = 2;
}

namespace turbo {

const int EpollPoller::kInitEventListSize = 16;

EpollPoller::EpollPoller(EventLoop *loop)
	: Poller(loop),
	  epoll_fd_(::epoll_create(EPOLL_CLOEXEC)),
	  event_list_(kInitEventListSize) {
  if (epoll_fd_ < 0)
	LOG_SYS_FATAL << "EpollPoller::EpollPoller";
}

EpollPoller::~EpollPoller() {
  ::close(epoll_fd_);
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
  Poller::AssertInLoopThread();
  const int index = channel->GetIndex();
  LOG_DEBUG << "fd = " << channel->GetFd()
			<< " events = " << channel->GetEvents()
			<< " index = " << index;
  if (index == kNew || index == kDel) {
	int fd = channel->GetFd();
	if (index == kNew) {
	  assert(channel_map_.find(fd) == channel_map_.end());
	  channel_map_[fd] = channel;
	} else {
	  assert(channel_map_.find(fd) != channel_map_.end());
	  assert(channel_map_[fd] == channel);
	}
	channel->SetIndex(kAdd);
	Update(EPOLL_CTL_ADD, channel);
  } else {
	int fd = channel->GetFd();
	assert(channel_map_.find(fd) != channel_map_.end());
	assert(channel_map_[fd] == channel);
	if (channel->IsNoneEvents()) {
	  Update(EPOLL_CTL_DEL, channel);
	  channel->SetIndex(kDel);
	} else {
	  Update(EPOLL_CTL_MOD, channel);
	}
  }
}

void EpollPoller::RemoveChannel(Channel *channel) {
  Poller::AssertInLoopThread();
  int fd = channel->GetFd();
  assert(channel_map_.find(fd) != channel_map_.end());
  assert(channel_map_[fd] == channel);
  assert(channel->IsNoneEvents());
  int index = channel->GetIndex();
  assert(index == kAdd || index == kDel);
  assert(channel_map_.erase(fd) == 1);
  if (index == kAdd) {
	Update(EPOLL_CTL_DEL, channel);
  }
  channel->SetIndex(kNew);
}

const char *EpollPoller::OperationToString(int op) {
  switch (op) {
	case EPOLL_CTL_ADD: return "ADD";
	case EPOLL_CTL_DEL: return "DEL";
	case EPOLL_CTL_MOD: return "MOD";
	default: return "Unknown Operation";
  }
}

void EpollPoller::FillActiveChannels(int num_events, Poller::ChannelList *active_channels) const {
  assert(static_cast<size_t>(num_events) < event_list_.size());
  for (int i = 0; i < num_events; i++) {
	auto *channel = static_cast<Channel *>(event_list_[i].data.ptr);
	channel->SetRevents(static_cast<int>(event_list_[i].events));
	active_channels->push_back(channel);
  }
}

void EpollPoller::Update(int operation, Channel *channel) const {
  struct epoll_event event{};
  MemZero(&event, sizeof(epoll_event));
  event.events = channel->GetEvents();
  event.data.ptr = channel;
  int fd = channel->GetFd();
  if (::epoll_ctl(epoll_fd_, operation, fd, &event) < 0) {
	if (operation == EPOLL_CTL_DEL) {
	  LOG_SYS_ERROR << "epoll_ctl op = " << OperationToString(operation) << " fd = " << fd;
	} else {
	  LOG_SYS_FATAL << "epoll_ctl op = " << OperationToString(operation) << " fd = " << fd;
	}
  }
}

} // turbo