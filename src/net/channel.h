// copyright (c) 2023
//
// file:     channel.h
// brief:    A channel belongs to only one EventLoop and is responsible for dispatching different I/O events to different callbacks.
// author:   CnLzh

#ifndef TURBONET_SRC_NET_CHANNEL_H_
#define TURBONET_SRC_NET_CHANNEL_H_

#include "public_define.h"
#include "timestamp.h"

#include <functional>
namespace turbo {

class EventLoop;

class Channel {
 public:
  using EventCallBack = std::function<void()>;
  using ReadEventCallBack = std::function<void(Timestamp)>;
  Channel(EventLoop *loop, int fd);
  ~Channel();

  void HandleEvent(Timestamp receive_time);
  void SetReadCallBack(ReadEventCallBack cb);
  void SetWriteCallBack(EventCallBack cb);
  void SetCloseCallBack(EventCallBack cb);
  void SetErrorCallBack(EventCallBack cb);

  [[nodiscard]] int GetFd() const;
  [[nodiscard]] int GetEvents() const;
  void SetRevents(int event);
  [[nodiscard]] bool IsNoneEvents() const;

  void EnableReading();
  void DisableReading();
  void EnableWriting();
  void DisableWriting();
  void DisableAll();

  [[nodiscard]] bool IsReading() const;
  [[nodiscard]] bool IsWriting() const;

  [[nodiscard]] int GetIndex() const;
  void SetIndex(int index);

  [[nodiscard]] EventLoop *OwnerLoop() const;

 private:
  void Update();
  void Remove();

  EventLoop *loop_;
  const int fd_;
  int events_;
  int revents_;
  int index_;

  bool event_handing_;
  bool add_to_loop_;

  static const int kNoneEvent;
  static const int kReadEvent;
  static const int kWriteEvent;

  ReadEventCallBack read_call_back_;
  EventCallBack write_call_back_;
  EventCallBack close_call_back_;
  EventCallBack error_call_back_;

  DISALLOW_COPY_AND_ASSIGN(Channel)
};

} // turbo

#endif //TURBONET_SRC_NET_CHANNEL_H_
