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

template<tb_s32 SIZE>
class FixedBuffer final {
 public:
  FixedBuffer() noexcept: cur_(data_) {};

  void Append(const tb_s8 *buf, const tb_u64 &len) {
	if (implicit_cast<tb_u64>(Avail()) > len) {
	  memcpy(cur_, buf, len);
	  cur_ += len;
	}
  }

  void Add(const tb_u64 &len) { cur_ += len; };

  [[nodiscard]] const tb_s8 *Data() const { return data_; };
  [[nodiscard]] tb_s32 Length() const { return static_cast<tb_s32>(cur_ - data_); };

  [[nodiscard]] tb_s8 *Current() const { return cur_; };
  [[nodiscard]] tb_s32 Avail() const { return static_cast<int>(End() - cur_); };

  void Reset() { cur_ = data_; };
  void BZero() { MemZero(data_, sizeof(data_)); };

  [[nodiscard]] std::string ToString() const { return std::string(data_, Length()); };

 private:
  [[nodiscard]] const tb_s8 *End() const { return data_ + sizeof(data_); };

  tb_s8 data_[SIZE] = {};
  tb_s8 *cur_ = nullptr;

  DISALLOW_COPY_AND_ASSIGN(FixedBuffer)
};

} // turbo

#endif //TURBONET_SRC_LOGGER_FIXED_BUFFER_H_
