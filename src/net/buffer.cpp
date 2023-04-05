#include "buffer.h"
#include "socket_ops.h"

#include <cassert>

namespace turbo {

const char Buffer::kCRLF[] = "\r\n";

Buffer::Buffer(size_t initial_size)
	: buffer_(kCheapPrepend + initial_size),
	  reader_index_(kCheapPrepend),
	  write_index(kCheapPrepend) {
  assert(ReadableBytes() == 0);
  assert(WriteableBytes() == initial_size);
  assert(PrependableBytes() == kCheapPrepend);
}

size_t Buffer::WriteableBytes() const {
  return buffer_.size() - write_index;
}

size_t Buffer::ReadableBytes() const {
  return write_index - reader_index_;
}

size_t Buffer::PrependableBytes() const {
  return reader_index_;
}

void Buffer::Swap(Buffer &rhs) {
  buffer_.swap(rhs.buffer_);
  std::swap(reader_index_, rhs.reader_index_);
  std::swap(write_index, write_index);
}

const char *Buffer::ReadableAddress() const {
  return Begin() + reader_index_;
}

const char *Buffer::WriteableAddress() const {
  return Begin() + write_index;
}

char *Buffer::WriteableAddress() {
  return Begin() + write_index;
}

char *Buffer::Begin() {
  return buffer_.data();
}

const char *Buffer::Begin() const {
  return buffer_.data();
}

void Buffer::MakeSpace(size_t len) {
  if (WriteableBytes() + PrependableBytes() < kCheapPrepend + len) {
	buffer_.resize(write_index + len);
  } else {
	assert(kCheapPrepend < reader_index_);
	size_t read_able = ReadableBytes();
	std::copy(Begin() + reader_index_,
			  Begin() + write_index,
			  Begin() + kCheapPrepend);
	reader_index_ = kCheapPrepend;
	write_index = reader_index_ + read_able;
	assert(read_able == ReadableBytes());
  }
}

void Buffer::MoveWriteBackward(size_t len) {
  assert(len <= WriteableBytes());
  write_index += len;
}

void Buffer::EnsureWriteableBytes(size_t len) {
  if (WriteableBytes() < len)
	MakeSpace(len);
  assert(WriteableBytes() >= len);
}

void Buffer::MoveReadBackward(size_t len) {
  assert(len <= ReadableBytes());
  if (len < ReadableBytes()) {
	reader_index_ += len;
  } else {
	MoveReadAll();
  }
}

void Buffer::MoveReadAll() {
  reader_index_ = kCheapPrepend;
  write_index = kCheapPrepend;
}

std::string Buffer::ReadToString(size_t len) {
  assert(len <= ReadableBytes());
  std::string result(ReadableAddress(), len);
  MoveReadBackward(len);
  return result;
}

std::string Buffer::ReadAllToString() {
  return ReadToString(ReadableBytes());
}

void Buffer::PushBack(const char *data, size_t len) {
  EnsureWriteableBytes(len);
  std::copy(data, data + len, WriteableAddress());
  MoveWriteBackward(len);
}

void Buffer::PushBack(const std::string &data) {
  PushBack(data.c_str(), data.size());
}

void Buffer::PushBack(const void *data, size_t len) {
  PushBack(static_cast<const char *>(data), len);
}

void Buffer::PushFront(const void *data, size_t len) {
  assert(len <= PrependableBytes());
  const char *d = static_cast<const char *>(data);
  reader_index_ -= len;
  std::copy(d, d + len, Begin() + reader_index_);
}

ssize_t Buffer::ReadFd(int fd, int *errno_info) {
  char extra_buf[65536]{};
  struct iovec vec[2]{};
  const size_t write_able = WriteableBytes();
  vec[0].iov_base = WriteableAddress();
  vec[0].iov_len = write_able;
  vec[1].iov_base = extra_buf;
  vec[1].iov_len = sizeof(extra_buf);

  const int iovcnt = (write_able < sizeof(extra_buf)) ? 2 : 1;
  const ssize_t n = sockets::ReadV(fd, vec, iovcnt);
  if (n < 0) {
	*errno_info = errno;
  } else if (n < static_cast<ssize_t>(write_able)) {
	MoveWriteBackward(n);
  } else {
	write_index = buffer_.size();
	PushBack(extra_buf, n - write_index);
  }
  return n;
}

size_t Buffer::WriteFd(int fd, int *errno_info) {
  const ssize_t n = sockets::Write(fd, ReadableAddress(), ReadableBytes());
  if (n < 0) {
	*errno_info = errno;
  } else {
	MoveReadBackward(n);
  }
  return n;
}

const char *Buffer::FindCRLF() const {
  const char *crlf = std::search(ReadableAddress(), WriteableAddress(), kCRLF, kCRLF + 2);
  return crlf == WriteableAddress() ? nullptr : crlf;
}

}