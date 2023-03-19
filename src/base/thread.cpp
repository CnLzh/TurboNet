#include "thread.h"
#include "current_thread.h"

#include <cassert>
#include <sys/prctl.h>
#include <iostream>

namespace turbo {

std::atomic_int32_t Thread::num_created_(0);

Thread::Thread(Thread::ThreadFunc func, std::string name) noexcept
	: started_(false),
	  joined_(false),
	  tid_(0),
	  name_(std::move(name)),
	  latch_(1),
	  func_(std::move(func)) {
  SetDefaultName();
}

Thread::~Thread() noexcept {
  if (started_ && !joined_)
	thread_->detach();
}

void Thread::Start() {
  assert(!started_);
  started_ = true;
  thread_ = std::make_shared<std::thread>(std::thread(
	  [&]() {
		tid_ = CurrentThread::CachedTid();
		CurrentThread::t_thread_name = name_;
		::prctl(PR_SET_NAME, CurrentThread::ThreadName().c_str());
		latch_.CountDown();
		try {
		  func_();
		  CurrentThread::t_thread_name = "finished";
		} catch (const std::exception &ex) {
		  std::cerr << "exception caught in Thread " << CurrentThread::ThreadName()
					<< std::endl << "reason: " << ex.what() << std::endl;
		  CurrentThread::t_thread_name = "crashed";
		  abort();
		}
	  }));
  if (!thread_->joinable()) {
	started_ = false;
	// TODO: output LOG_FATAL
  } else {
	latch_.Wait();
	assert(tid_ > 0);
  }

}

void Thread::Join() {
  assert(started_ && !joined_);
  joined_ = true;
  thread_->join();
}

bool Thread::Started() const {
  return started_;
}

pid_t Thread::Tid() const {
  return tid_;
}

const std::string &Thread::Name() const {
  return name_;
}

tb_s32 Thread::NumCreated() {
  return num_created_;
}

void Thread::SetDefaultName() {
  tb_s32 num = ++num_created_;
  if (name_.empty())
	name_ = "Thread" + std::to_string(num);
}

} // turbo