#include "current_thread.h"

#include <thread>

namespace turbo::CurrentThread {

thread_local pid_t t_cached_tid = 0;
thread_local std::string t_tid_string = std::string();
thread_local std::string t_thread_name = "unknown";

void ConstructTid(){
  if (t_cached_tid == 0)
  {
	t_cached_tid = static_cast<pid_t>(::syscall(SYS_gettid));
	t_tid_string = std::to_string(t_cached_tid);
  }
}



} // turbo