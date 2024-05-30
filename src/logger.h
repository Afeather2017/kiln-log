#pragma once
#include "async_log.h"
#include "config.h"
#include <memory>
// the sigleton of log
namespace klog {
namespace detail {
class Logger {
 public:
  static Logger& Get() {
    static Logger logger;
    return logger;
  }

  void LogWithConfig(const Message &msg) {
    if (!Config::Get().ShouleLog(msg.context_->level)) {
      return;
    }
    if (Config::Get().ShouldLogToFile()) {
      al_->Append(msg);
    }
    if (Config::Get().ShouldLogToConsole()) {
      fmt::println(stderr, "{}", msg.Format());
    }
  }

  void CleanUp() {
    al_.reset(nullptr);
  }

 private:
  DISALLOW_COPYMOVE(Logger);

  Logger(): al_{std::make_unique<AsyncLog>("./logs", 65535, 3)} {}
  
  std::unique_ptr<AsyncLog> al_;
};
} // namespace detail
} // namespace klog
