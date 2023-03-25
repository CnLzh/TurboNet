#include "async_logging.h"
#include "log_file.h"

#include <utility>
#include <cassert>

namespace turbo {

AsyncLogging::AsyncLogging(std::string base_name,
						   const off_t &roll_size,
						   const int &flush_interval) noexcept
	: flush_interval_(flush_interval),
	  running_(false),
	  base_name_(std::move(base_name)),
	  roll_size_(roll_size),
	  thread_([this] { ThreadFunc(); }, "Logging"),
	  latch_(1),
	  mutex_(),
	  cond_(mutex_),
	  current_buffer_(std::make_unique<Buffer>()),
	  next_buffer_(std::make_unique<Buffer>()),
	  buffers_() {
  current_buffer_->BZero();
  next_buffer_->BZero();
}

AsyncLogging::~AsyncLogging() {
  if (running_)
	Stop();
}

void AsyncLogging::Start() {
  running_ = true;
  thread_.Start();
  latch_.Wait();
}

void AsyncLogging::Stop() {
  running_ = false;
  cond_.Notify();
  thread_.Join();
}

void AsyncLogging::Append(const tb_s8 *data, const tb_s32 &len) {
  MutexLockGuard lock(mutex_);
  if (current_buffer_->Avail() > len) {
	current_buffer_->Append(data, len);
  } else {
	buffers_.push_back(std::move(current_buffer_));
	if (next_buffer_) {
	  current_buffer_ = std::move(next_buffer_);
	} else {
	  current_buffer_ = std::make_unique<Buffer>();
	}
	current_buffer_->Append(data, len);
	cond_.Notify();
  }
}

void AsyncLogging::ThreadFunc() {
  assert(running_ == true);
  latch_.CountDown();
  LogFile output(base_name_, roll_size_, false);
  BufferPtr new_buffer_1 = std::make_unique<Buffer>();
  BufferPtr new_buffer_2 = std::make_unique<Buffer>();
  new_buffer_1->BZero();
  new_buffer_2->BZero();
  BufferVector buffers_to_write;
  while (running_) {
	// NOLINTNEXTLINE(bugprone-use-after-move)
	assert(new_buffer_1 && new_buffer_1->Length() == 0);
	// NOLINTNEXTLINE(bugprone-use-after-move)
	assert(new_buffer_2 && new_buffer_2->Length() == 0);
	assert(buffers_to_write.empty());

	{
	  MutexLockGuard lock(mutex_);
	  if (buffers_.empty())
		cond_.WaitForSeconds(flush_interval_);
	  buffers_.push_back(std::move(current_buffer_));
	  current_buffer_ = std::move(new_buffer_1);
	  buffers_to_write.swap(buffers_);
	  if (!next_buffer_)
		next_buffer_ = std::move(new_buffer_2);
	}

	assert(!buffers_to_write.empty());

	if (buffers_to_write.size() > 25) {
	  char buf[256];
	  snprintf(buf, sizeof(buf),
			   "Drop log messages at %s, %zd larger buffers\n",
			   Timestamp::Now().ToFormatString().c_str(),
			   buffers_to_write.size() - 2);
	  fputs(buf, stderr);
	  output.Append(buf, static_cast<tb_s32>(strlen(buf)));
	  buffers_to_write.erase(buffers_to_write.begin() + 2, buffers_to_write.end());
	}

	for (const auto &buffer : buffers_to_write) {
	  output.Append(buffer->Data(), buffer->Length());
	}

	if (!new_buffer_1) {
	  assert(!buffers_to_write.empty());
	  new_buffer_1 = std::move(buffers_to_write.back());
	  buffers_to_write.pop_back();
	  new_buffer_1->Reset();
	}

	if (!new_buffer_2) {
	  assert(!buffers_to_write.empty());
	  new_buffer_2 = std::move(buffers_to_write.back());
	  buffers_to_write.pop_back();
	  new_buffer_2->Reset();
	}

	buffers_to_write.clear();
	output.Flush();
  }
  output.Flush();
}

} // turbo