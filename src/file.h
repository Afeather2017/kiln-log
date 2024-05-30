#include <cstdio>
#include <ctime>
#include <memory>
#include <unistd.h>
#include "macros.h"
namespace klog {
namespace detail {
class LogFile {
 public:
  DISALLOW_COPYMOVE(LogFile);
  LogFile(const char *file_path);
  void Append(const char *buf, int len);
  void Flush();
  void ResetWritten();
  size_t WrittenBytes();
  ~LogFile();
 private:
  FILE  *fp_{};
  char   buffer_[64 * 1024]{};
  size_t written_{};
};

class RollingLog {
 public:
  DISALLOW_COPYMOVE(RollingLog);
  RollingLog(const char *log_path, size_t roll_size = 1024 * 1024 * 1024, int flush_interval = 3);
  void Flush();
  // 每一次Append日志时，会检查是否进行Roll和Flush
  void Append(const char *buf, int len);
 private:
  void Roll();
  const char *log_path_{};
  size_t      roll_size_{};
  int         flush_interval_{};
  time_t      last_flush_time_{};

  std::unique_ptr<LogFile> file_{};

  // roll log per day.
  static const time_t DAY_SECOND = 60 * 60 * 24;
};

} // namespace detail
} // namespace klog
