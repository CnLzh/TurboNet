#include "event_loop.h"
#include "logger.h"
#include "current_thread.h"
#include "channel.h"
#include "poller.h"

#include <cassert>

#include <sys/poll.h>
namespace turbo {

thread_local EventLoop *t_loop_in_this_thread = nullptr;

EventLoop::EventLoop()
	: looping_(false),
	  quit_(false),
	  thread_id_(CurrentThread::CachedTid()) {
  LOG_DEBUG << "EventLoop created " << this << " in thread " << thread_id_;
  if (t_loop_in_this_thread) {
	LOG_FATAL << "EventLoop was create in " << t_loop_in_this_thread
			  << " exists in thread " << thread_id_;
  } else {
	t_loop_in_this_thread = this;
  }
}

EventLoop::~EventLoop() {
  assert(!looping_);
  t_loop_in_this_thread = nullptr;
}

bool EventLoop::IsInLoopThread() const {
  return thread_id_ == CurrentThread::CachedTid();
}

void EventLoop::AssertInLoopThread() const {
  if (!IsInLoopThread())
	AbortNotInLoopThread();
}

void EventLoop::Loop() {
  assert(!looping_);
  AssertInLoopThread();
  looping_ = true;
  quit_ = false;
  LOG_DEBUG << "EventLoop " << this << " start looping";
  // TODO: do something
  ::poll(nullptr, 0, 5 * 1000);
  LOG_DEBUG << "EventLoop " << this << " stop looping";
  looping_ = false;
}

void EventLoop::UpdateChannel(Channel *channel) {
  assert(channel->OwnerLoop() == this);
  AssertInLoopThread();
  poller_->UpdateChannel(channel);
}

void EventLoop::RemoveChannel(Channel *channel) {
  assert(channel->OwnerLoop() == this);
  AssertInLoopThread();
  //TODO: event_handing
  poller_->RemoveChannel(channel);
}

bool EventLoop::HaveChannel(Channel *channel) {
  assert(channel->OwnerLoop() == this);
  AssertInLoopThread();
  return poller_->HaveChannel(channel);
}

// private
void EventLoop::AbortNotInLoopThread() const {
  LOG_FATAL << "EventLoop::AbortNotInLoopThread, it was created in thread = "
			<< thread_id_ << ", current thread id = " << CurrentThread::CachedTid();
}

// static
EventLoop *EventLoop::GetEventLoopCurrentThread() {
  return t_loop_in_this_thread;
}

} // turbo