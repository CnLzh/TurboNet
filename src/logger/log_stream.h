// Copyright (C) 2023
//
// File:     log_stream.h
// Brief:    LogStream class provides C++ style logging stream operations.
// Author:   CnLzh

#ifndef TURBONET_SRC_LOGGER_LOG_STREAM_H_
#define TURBONET_SRC_LOGGER_LOG_STREAM_H_

#include "fixed_buffer.h"

namespace turbo {

const tb_s32 kSmallBuffer = 4000;
const tb_s32 kLargeBuffer = 4000 * 1000;

class LogStream final {
  using Self = LogStream;
  using Buffer = FixedBuffer<kSmallBuffer>;

 public:
  LogStream() = default;
  void Append(const tb_s8* data, tb_s32 len);
  void ResetBuffer();
  [[nodiscard]] const Buffer& GetBuffer() const;

  Self &operator<<(bool);
  Self &operator<<(short);
  Self &operator<<(unsigned short);
  Self &operator<<(int);
  Self &operator<<(unsigned int);
  Self &operator<<(long);
  Self &operator<<(unsigned long);
  Self &operator<<(long long);
  Self &operator<<(unsigned long long);
  Self &operator<<(float);
  Self &operator<<(double);
  Self &operator<<(char);
  Self &operator<<(const char *);
  Self &operator<<(const unsigned char *);
  Self &operator<<(const void *);
  Self &operator<<(const std::string &);
  Self &operator<<(const Buffer &);

 private:
  template<typename T>
  void FormatInteger(T);

  static const tb_s32 kMaxNumericSize = 48;

  Buffer buffer_;

  DISALLOW_COPY_AND_ASSIGN(LogStream)

};

} // turbo

#endif //TURBONET_SRC_LOGGER_LOG_STREAM_H_
