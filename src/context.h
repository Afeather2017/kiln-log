#include <string>
namespace klog {
namespace detail {
struct Context {
  // 用于输出日志的上下文
  int          level{};
  unsigned int tid{};
  int          line{};
  int          err{};
  const char*  short_filename{};
  const char*  long_filename{};
  const char*  func_name{};
  std::string  text;
};
} // namespace detail
} // namespace klog
