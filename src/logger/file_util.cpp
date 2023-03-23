#include "file_util.h"

namespace turbo {

FileUtil::FileUtil(const std::string &file_name)
	: file_(file_name, std::ios_base::app),
	  written_bytes_(0) {
  file_.rdbuf()->pubsetbuf(buffer_, sizeof(buffer_));
}

FileUtil::~FileUtil() {
  file_.close();
}

} // turbo