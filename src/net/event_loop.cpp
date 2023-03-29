#include "event_loop.h"
#include "logger.h"
#include "current_thread.h"

#include <cassert>
namespace turbo {

thread_local EventLoop *t_loop_in_this_thread = nullptr;

EventLoop::EventLoop()
	: looping_(false),
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
  // TODO: do something
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