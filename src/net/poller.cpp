#include "poller.h"
#include "event_loop.h"
#include "channel.h"

namespace turbo {

Poller::Poller(EventLoop *loop)
	: loop_(loop) {

}

void Poller::AssertInLoopThread() const {
  loop_->AssertInLoopThread();
}

bool Poller::HaveChannel(Channel *channel) const {
  AssertInLoopThread();
  auto it = channel_map_.find(channel->GetFd());
  return it != channel_map_.end() && it->second == channel;
}

} // turbo