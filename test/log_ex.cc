#include "kiln-log.h"
#include "macros.h"
int main() {
  Config::Get().PutToConsole(true).PutToLogFile(true).SetLogLevel(klog::INFO_LEVEL);
  Trace("this is a trace trace log");
  Debug("this {}", "is a simple debug log");
  Info("this is {}", "a simple info log");
  Warn("this is a {}", "simple warn log");
  Error("this is a simple {}", "error log");
  Fatal("this is a simple fatal {}", "log");
}
