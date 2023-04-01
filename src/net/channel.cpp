#include "channel.h"
#include "event_loop.h"

#include <cassert>
#include <sys/epoll.h>
#include <sys/poll.h>
namespace turbo {

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = EPOLLIN | EPOLLPRI;
const int Channel::kWriteEvent = EPOLLOUT;

Channel::Channel(EventLoop *loop, int fd)
	: loop_(loop),
	  fd_(fd),
	  events_(0),
	  revents_(0),
	  index_(-1),
	  add_to_loop_(false) {
}

Channel::~Channel() {
  assert(!add_to_loop_);
  if (loop_->IsInLoopThread())
	assert(!loop_->HaveChannel(this));
}

void Channel::HandleEvent() {
  //TODO: timestamp
  if (revents_ & (EPOLLIN | EPOLLPRI | EPOLLRDHUP)) {
	if (read_call_back_)
	  read_call_back_();
  }
  if (revents_ & EPOLLERR) {
	if (error_call_back_)
	  error_call_back_();
  }
  if (revents_ & EPOLLOUT) {
	if (write_call_back_)
	  write_call_back_();
  }
}
void Channel::SetReadCallBack(Channel::EventCallBack cb) {
  read_call_back_ = std::move(cb);
}

void Channel::SetWriteCallBack(Channel::EventCallBack cb) {
  write_call_back_ = std::move(cb);
}

void Channel::SetCloseCallBack(Channel::EventCallBack cb) {
  close_call_back_ = std::move(cb);
}

void Channel::SetErrorCallBack(Channel::EventCallBack cb) {
  error_call_back_ = std::move(cb);
}
int Channel::GetFd() const {
  return fd_;
}

int Channel::GetEvents() const {
  return events_;
}

void Channel::SetRevents(int event) {
  revents_ = event;
}

bool Channel::IsNoneEvents() const {
  return events_ == kNoneEvent;
}

void Channel::EnableReading() {
  events_ |= kReadEvent;
  Update();
}

void Channel::DisableReading() {
  events_ &= ~kReadEvent;
  Update();
}

void Channel::EnableWriting() {
  events_ |= kWriteEvent;
  Update();
}

void Channel::DisableWriting() {
  events_ &= ~kWriteEvent;
  Update();
}

void Channel::DisableAll() {
  events_ = kNoneEvent;
  Update();
}

bool Channel::IsWriting() const {
  return events_ & kReadEvent;
}

bool Channel::IsReading() const {
  return events_ & kWriteEvent;
}

int Channel::GetIndex() const {
  return index_;
}
void Channel::SetIndex(int index) {
  index_ = index;
}

EventLoop *Channel::OwnerLoop() const {
  return loop_;
}

void Channel::Update() {
  add_to_loop_ = true;
  loop_->UpdateChannel(this);
}

void Channel::Remove() {
  assert(!IsNoneEvents());
  add_to_loop_ = false;
  loop_->RemoveChannel(this);
}

} // turbo