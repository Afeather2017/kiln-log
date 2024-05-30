#pragma once
#include <string>
#include <ctime>
#include <unistd.h>
#include <fmt/core.h>
namespace klog {
namespace detail {
inline std::string TimeString(time_t tm) {
  struct tm tm_struct;
  localtime_r(&tm, &tm_struct);
  char buf[64];
  strftime(buf, 64, "%Y%m%d-%H%M%S", &tm_struct);
  return buf;
}
// 日志文件格式：日期-时间-PID.log
inline std::string GetLogPathName(const char *log_path, time_t log_time) {
  return fmt::format("{}/{}-{}.log", log_path, TimeString(log_time), getpid());
}
} // namespace detail
} // namespace klog
