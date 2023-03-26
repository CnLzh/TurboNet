#include "log_stream.h"

#include <algorithm>

namespace turbo {

const char digits[] = "9876543210123456789";
const char *zero = digits + 9;
static_assert(sizeof(digits) == 20, "wrong number of digits");

const char digits_hex[] = "0123456789ABCDEF";
static_assert(sizeof digits_hex == 17, "wrong number of digitsHex");

void LogStream::Append(const char *data, size_t len) {
  buffer_.Append(data, len);
}

const LogStream::Buffer &LogStream::GetBuffer() const {
  return buffer_;
}

void LogStream::ResetBuffer() {
  buffer_.Reset();
}

LogStream &LogStream::operator<<(bool v) {
  buffer_.Append(v ? "1" : "0", 1);
  return *this;
}

LogStream &LogStream::operator<<(short v) {
  return *this << static_cast<int>(v);
}

LogStream &LogStream::operator<<(unsigned short v) {
  return *this << static_cast<unsigned int>(v);
}

LogStream &LogStream::operator<<(int v) {
  FormatInteger(v);
  return *this;
}

LogStream &LogStream::operator<<(unsigned int v) {
  FormatInteger(v);
  return *this;
}

LogStream &LogStream::operator<<(long v) {
  FormatInteger(v);
  return *this;
}

LogStream &LogStream::operator<<(unsigned long v) {
  FormatInteger(v);
  return *this;
}

LogStream &LogStream::operator<<(long long v) {
  FormatInteger(v);
  return *this;
}

LogStream &LogStream::operator<<(unsigned long long v) {
  FormatInteger(v);
  return *this;
}

LogStream &LogStream::operator<<(float v) {
  return *this << static_cast<double>(v);
}

LogStream &LogStream::operator<<(double v) {
  if (buffer_.Avail() < kMaxNumericSize)
	return *this;
  auto len = snprintf(buffer_.Current(), kMaxNumericSize, "%.12g", v);
  buffer_.Add(len);
  return *this;
}

LogStream &LogStream::operator<<(char v) {
  buffer_.Append(&v, 1);
  return *this;
}

LogStream &LogStream::operator<<(const char *v) {
  if (v)
	buffer_.Append(v, strlen(v));
  else
	buffer_.Append("(null)", 6);
  return *this;
}

LogStream &LogStream::operator<<(const unsigned char *v) {
  return *this << reinterpret_cast<const char *>(v);
}

LogStream &LogStream::operator<<(const void *v) {
  if (buffer_.Avail() < kMaxNumericSize)
	return *this;

  auto start = buffer_.Current();
  *start++ = '0';
  *start++ = 'x';

  auto cur = start;
  auto i = reinterpret_cast<unsigned long long>(v);

  do {
	auto lsd = static_cast<int >(i % 16);
	i /= 16;
	*cur++ = digits_hex[lsd];
  } while (i != 0);

  *cur = '\0';
  std::reverse(start, cur);
  buffer_.Add(static_cast<int >(cur - start + 2));

  return *this;
}

LogStream &LogStream::operator<<(const std::string &v) {
  buffer_.Append(v.c_str(), v.size());
  return *this;
}

LogStream &LogStream::operator<<(const Buffer &v) {
  return *this << v.ToString();
}

template<typename T>
void LogStream::FormatInteger(T v) {
  if (buffer_.Avail() < kMaxNumericSize)
	return;

  auto *start = buffer_.Current();
  auto *cur = buffer_.Current();
  auto i = v;

  do {
	auto lsd = static_cast<int>(i % 10);
	i /= 10;
	*cur++ = zero[lsd];
  } while (i != 0);

  if (v < 0) *cur++ = '-';
  *cur = '\0';
  std::reverse(start, cur);
  buffer_.Add(static_cast<int>(cur - start));
}

} // turbo