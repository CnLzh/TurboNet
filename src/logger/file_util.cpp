#include "file_util.h"
#include "logger.h"
namespace turbo {

FileUtil::FileUtil(const std::string &file_name) noexcept
	: fp_(::fopen(file_name.c_str(), "ae")),
	  written_bytes_(0) {
  ::setbuffer(fp_, buffer_, sizeof(buffer_));
}

FileUtil::~FileUtil() noexcept {
  ::fclose(fp_);
}

void FileUtil::Append(const char *data, size_t len) {
  size_t n = Write(data, len);
  size_t remain = len - n;
  while (remain > 0) {
	size_t x = Write(data + n, remain);
	if (x == 0) {
	  int err = ferror(fp_);
	  if (err) {
		fprintf(stderr, "AppendFile::Append() failed %s\n", StrErrorR(err));
	  }
	  break;
	}
	n += x;
	remain = len - n;
  }
  written_bytes_ += static_cast<off_t>(len);
}

size_t FileUtil::Write(const char *data, size_t len) {
  return ::fwrite_unlocked(data, 1, len, fp_);
}

void FileUtil::Flush() {
  ::fflush(fp_);
}

off_t FileUtil::WrittenBytes() const {
  return written_bytes_;
}

} // turbo