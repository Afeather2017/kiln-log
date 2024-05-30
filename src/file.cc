#include <cstring> // NOLINT
#include <memory>
#include <stdexcept>
#include <unistd.h>
#include "macros.h"
#include "file.h"
#include "util.h"
namespace klog {
namespace detail {

RollingLog::RollingLog(const char *log_path, size_t roll_size, int flush_interval) {
  log_path_ = log_path;
  roll_size_ = roll_size;
  flush_interval_ = flush_interval;
  Roll();
}

void RollingLog::Roll() {
  // 关闭旧文件，并重新打开一个文件
  time_t now = time(nullptr);
  auto path_name = GetLogPathName(log_path_, now);
  file_ = std::make_unique<LogFile>(path_name.c_str());
}

void RollingLog::Flush() {
  file_->Flush();
}

void RollingLog::Append(const char *buf, int len) {
  file_->Append(buf, len);
  if (file_->WrittenBytes() >= roll_size_) {
    Roll();
  } else {
    time_t now = time(nullptr); // 可能是瓶颈
    if (now > last_flush_time_ + flush_interval_) {
      file_->Flush();
      last_flush_time_ = now;
    }
  }
}

void LogFile::Flush() {
  fflush(fp_);
}

size_t LogFile::WrittenBytes() {
  return written_;
}

LogFile::LogFile(const char *file_path) {
  (void)strlen; // 解决标明cstring头文件未使用的问题。实际上使用了strerror_r
  // e的含义是O_EXEC，避免fork后文件描述符未关闭的问题。
  fp_ = fopen(file_path, "ae");
  if (fp_ == nullptr) {
    throw std::runtime_error{"open log file failed"};
  }
  setvbuf(fp_, buffer_, _IOFBF, sizeof(buffer_));
}

void LogFile::Append(const char *buf, int len) {
  int cur = 0;
  while (cur < len) {
    int writed = fwrite(buf + cur, 1, len - cur, fp_);
    if (writed != len) {
      int error = ferror(fp_);
      if (error != 0) {
        char err_buf[1024]{};
        strerror_r(error, err_buf, 1024);
        fprintf(stderr, "%s failed: %s", __PRETTY_FUNCTION__, err_buf);
      }
      break;
    }
    cur += writed;
  }
  written_ += cur;
}

void LogFile::ResetWritten() {
  written_ = 0;
}

LogFile::~LogFile() {
  fflush(fp_);
  fclose(fp_);
}
} // namespace detail
} // namespace klog
