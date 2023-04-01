// copyright (c) 2023
//
// file:     event_loop.h
// brief:    EventLoop class.
// author:   CnLzh

#ifndef TURBONET_SRC_NET_EVENT_LOOP_H_
#define TURBONET_SRC_NET_EVENT_LOOP_H_

#include "public_define.h"

namespace turbo {

class Channel;

class EventLoop {
 public:
  EventLoop();
  ~EventLoop();

  [[nodiscard]] bool IsInLoopThread() const;
  void AssertInLoopThread() const;

  void Loop();

  void UpdateChannel(Channel *channel);
  void RemoveChannel(Channel *channel);
  bool HaveChannel(Channel *channel);

  static EventLoop *GetEventLoopCurrentThread();

 private:
  void AbortNotInLoopThread() const;

  bool looping_;
  const pid_t thread_id_;
  DISALLOW_COPY_AND_ASSIGN(EventLoop)
};

} // turbo

#endif //TURBONET_SRC_NET_EVENT_LOOP_H_
