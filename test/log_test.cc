#include "gtest/gtest.h"
#include <thread>
#include "kiln-log.h"

TEST(Log, ManyTimes) {
  Config::Get().SetLogLevel(klog::DEBUG_LEVEL).PutToConsole(false);
  auto dbg_fn = []{
    for (int i = 0; i < 3000000; i++) {
      Debug("{}", "warn message");
    }
  };
  auto wrn_fn = []{
    for (int i = 0; i < 3000000; i++) {
      Warn("{}", "warn message");
    }
  };
  auto err_fn  = []{
    for (int i = 0; i < 3000000; i++) {
      Error("{}", "error message");
    }
  };
  std::thread th1{dbg_fn};
  std::thread th2{wrn_fn};
  std::thread th3{err_fn};
  th1.join();
  th2.join();
  th3.join();
}

TEST(Log, Basic) {
  Config::Get().SetLogLevel(klog::WARN_LEVEL);
  Warn("{}", "warn message");
  Error("{}", "error message");
  try {
    Fatal("{}", "fatal message");
  } catch (...) {
    ASSERT_NE("this should", "be reached");
  }
}
