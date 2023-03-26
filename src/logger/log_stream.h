// Copyright (C) 2023
//
// File:     log_stream.h
// Brief:    LogStream class provides C++ style logging stream operations.
// Author:   CnLzh

#ifndef TURBONET_SRC_LOGGER_LOG_STREAM_H_
#define TURBONET_SRC_LOGGER_LOG_STREAM_H_

#include "fixed_buffer.h"

namespace turbo {

const int kSmallBuffer = 4000;
const int kLargeBuffer = 4000 * 1000;

class LogStream final {
 public:
  using Buffer = FixedBuffer<kSmallBuffer>;

  LogStream() = default;
  void Append(const char *data, size_t len);
  void ResetBuffer();
  [[nodiscard]] const Buffer &GetBuffer() const;

  LogStream &operator<<(bool);
  LogStream &operator<<(short);
  LogStream &operator<<(unsigned short);
  LogStream &operator<<(int);
  LogStream &operator<<(unsigned int);
  LogStream &operator<<(long);
  LogStream &operator<<(unsigned long);
  LogStream &operator<<(long long);
  LogStream &operator<<(unsigned long long);
  LogStream &operator<<(float);
  LogStream &operator<<(double);
  LogStream &operator<<(char);
  LogStream &operator<<(const char *);
  LogStream &operator<<(const unsigned char *);
  LogStream &operator<<(const void *);
  LogStream &operator<<(const std::string &);
  LogStream &operator<<(const Buffer &);

 private:
  template<typename T>
  void FormatInteger(T);

  static const unsigned int kMaxNumericSize = 48;

  Buffer buffer_;

  DISALLOW_COPY_AND_ASSIGN(LogStream)

};

} // turbo

#endif //TURBONET_SRC_LOGGER_LOG_STREAM_H_
