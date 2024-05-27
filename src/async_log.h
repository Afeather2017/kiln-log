#include <memory>
#include <thread>
#include "macros.h"
namespace klog {
namespace detail {
struct Context;
using SharedContext = std::shared_ptr<Context>;
struct Message {

  SharedContext context;
};
class AsyncLog {
 public:
  void Append(const Message &message);
  AsyncLog(size_t roll_size, time_t flush_interval);
 private:
  DISALLOW_COPY(AsyncLog);
  std::thread worker_;
  size_t roll_size_;
  time_t flush_interval_;
};
} // namespace detail
} // namespace klog
