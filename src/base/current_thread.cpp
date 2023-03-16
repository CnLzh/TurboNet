#include <csignal>
#include <unistd.h>
#include <sys/syscall.h>

#include "current_thread.h"

namespace turbo::CurrentThread {

void CachedTid(){
  if (t_cached_tid == 0)
  {
	t_cached_tid = static_cast<pid_t>(::syscall(SYS_gettid));
  }
}



} // turbo