#include <string>
#include <ctime>
#include <unistd.h>
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
  return log_path + ('/' + TimeString(log_time)) + '-' + std::to_string(getpid()) + ".log";
}
} // namespace detail
} // namespace klog
