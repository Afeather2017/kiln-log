#pragma once
#include "kiln-log.h"
#include "macros.h"
#include <cstdlib>
class Config {
 public:
  DISALLOW_COPYMOVE(Config);
  static Config &Get() {
    static Config conf;
    return conf;
  }

  bool ShouldLogToConsole() const {
    return to_console_;
  }

  bool ShouldLogToFile() const {
    return to_log_file_;
  }

  bool ShouleLog(const int log_level) const {
    return log_level >= current_log_level_;
  }

  Config &PutToConsole(bool val) {
    to_console_ = val;
    return *this;
  }

  Config &PutToLogFile(bool val) {
    to_log_file_ = val;
    return *this;
  }

  Config &SetLogLevel(int level) {
    current_log_level_ = level;
    return *this;
  }

 private:
  Config() {
    to_console_ = nullptr == getenv("KILN_TO_CONSOLE");
    to_log_file_ = nullptr == getenv("KILN_FILE_CONSOLE");
    const char *val = getenv("KILN_LOG_LEVEL");
    if (val == nullptr) {
      current_log_level_ = klog::WARN_LEVEL;
    } else {
      sscanf(val, "%d", &current_log_level_);
    }
  }
  bool to_console_;
  bool to_log_file_;
  int  current_log_level_;
};
