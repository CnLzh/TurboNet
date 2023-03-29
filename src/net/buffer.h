// copyright (c) 2023
//
// file:     buffer.h
// brief:    Buffer class.
// author:   CnLzh

#ifndef TURBONET_SRC_NET_BUFFER_H_
#define TURBONET_SRC_NET_BUFFER_H_

#include <vector>
#include <string>

namespace turbo {

class Buffer {
 public:
  static const size_t kCheapPrepend = 8;
  static const size_t kInitialSize = 1024;

  explicit Buffer(size_t initial_size = kInitialSize);

  [[nodiscard]] size_t ReadableBytes() const;
  [[nodiscard]] size_t WriteableBytes() const;
  [[nodiscard]] size_t PrependableBytes() const;

  void Swap(Buffer &rhs);

  // return readable address
  [[nodiscard]] const char *ReadableAddress() const;

  // return writeable address
  [[nodiscard]] const char *WriteableAddress() const;
  char *WriteableAddress();

  void EnsureWriteableBytes(size_t len);

  void MoveWriteBackward(size_t len);
  void MoveReadBackward(size_t len);
  void MoveReadAll();

  std::string ReadToString(size_t len);
  std::string ReadAllToString();

  void PushBack(const char *data, size_t len);
  void PushBack(const std::string &data);
  void PushBack(const void *data, size_t len);
  void PushFront(const void *data, size_t len);

  ssize_t ReadFd(int fd, int *errno_info);
  size_t WriteFd(int fd, int *errno_info);

  [[nodiscard]] const char *FindCRLF() const;

 private:
  char *Begin();
  [[nodiscard]] const char *Begin() const;

  void MakeSpace(size_t len);

  std::vector<char> buffer_;
  size_t reader_index_;
  size_t write_index;

  static const char kCRLF[];
};

} // turbo
#endif //TURBONET_SRC_NET_BUFFER_H_
