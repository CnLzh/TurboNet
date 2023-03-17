// Copyright (C) 2023
//
// File:     current_thread.h
// Brief:    Provide information about the current thread.
// Author:   CnLzh

#ifndef TURBONET_SRC_BASE_CURRENT_THREAD_H_
#define TURBONET_SRC_BASE_CURRENT_THREAD_H_

#include "public_define.h"

#include <string>
namespace turbo::CurrentThread{

extern thread_local pid_t t_cached_tid;
extern thread_local std::string t_tid_string;
extern thread_local std::string t_thread_name;

void ConstructTid();

inline pid_t CachedTid(){
  if(__builtin_expect(t_cached_tid==0,0)) ConstructTid();
  return t_cached_tid;
}

inline std::string TidString(){
  return t_tid_string;
}

inline std::string ThreadName(){
  return t_thread_name;
}

} // turbo

#endif //TURBONET_SRC_BASE_CURRENT_THREAD_H_
