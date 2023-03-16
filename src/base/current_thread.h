// Copyright (C) 2023
//
// File:     current_thread.h
// Brief:    Provide information about the current thread.
// Author:   CnLzh

#ifndef TURBONET_SRC_BASE_CURRENT_THREAD_H_
#define TURBONET_SRC_BASE_CURRENT_THREAD_H_

#include "public_define.h"

namespace turbo::CurrentThread{

extern __thread tb_s32 t_cached_tid;
extern __thread tb_s8 t_tid_string[32];
extern __thread tb_s32 t_tid_string_length;
extern __thread const tb_s8* t_thread_name;

void CachedTid();

inline tb_s32 Tid(){
  if(__builtin_expect(t_cached_tid==0,0)) CachedTid();
  return t_cached_tid;
}

} // turbo

#endif //TURBONET_SRC_BASE_CURRENT_THREAD_H_
