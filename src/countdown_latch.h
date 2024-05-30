#include "macros.h"
#include <condition_variable>
#include <mutex>
class CountDownLatch {
 public:
  DISALLOW_COPY(CountDownLatch);
  CountDownLatch(int count): count_{count} {}
  void Wait() {
    std::unique_lock<std::mutex> lock{latch_};
    while (count_ > 0) {
      cv_.wait(lock);
    }
  }
  void CountDown() {
    std::unique_lock<std::mutex> lock{latch_};
    count_--;
    if (count_ == 0) {
      cv_.notify_all();
    }
  }
  int GetCount() {
    std::unique_lock<std::mutex> lock{latch_};
    return count_;
  }
 private:
  int count_;
  std::condition_variable cv_;
  std::mutex latch_;
};
