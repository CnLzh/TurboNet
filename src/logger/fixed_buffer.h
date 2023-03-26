// Copyright (C) 2023
//
// File:     fixed_buffer.h
// Brief:    FixedBuffer class is used to implement a buffer for storing log information.
// Author:   CnLzh

#ifndef TURBONET_SRC_LOGGER_FIXED_BUFFER_H_
#define TURBONET_SRC_LOGGER_FIXED_BUFFER_H_

#include "public_define.h"

#include <string>
#include <cstring>

namespace turbo {

template<int SIZE>
class FixedBuffer final {
 public:
  FixedBuffer() noexcept: data_(), cur_(data_) {};

  void Append(const char *buf, size_t len) {
	if (implicit_cast<size_t>(Avail()) > len) {
	  memcpy(cur_, buf, len);
	  cur_ += len;
	}
  }

  void Add(unsigned long long len) { cur_ += len; };

  [[nodiscard]] const char *Data() const { return data_; };
  [[nodiscard]] int Length() const { return static_cast<int>(cur_ - data_); };

  [[nodiscard]] char *Current() const { return cur_; };
  [[nodiscard]] int Avail() const { return static_cast<int>(End() - cur_); };

  void Reset() { cur_ = data_; };
  void BZero() { MemZero(data_, sizeof(data_)); };

  [[nodiscard]] std::string ToString() const { return std::string(data_, Length()); };

 private:
  [[nodiscard]] const char *End() const { return data_ + sizeof(data_); };

  char data_[SIZE];
  char *cur_ = nullptr;

  DISALLOW_COPY_AND_ASSIGN(FixedBuffer)
};

} // turbo

#endif //TURBONET_SRC_LOGGER_FIXED_BUFFER_H_
