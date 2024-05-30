#include <atomic>
#include <cassert>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>
#include "macros.h"
#include "countdown_latch.h"
namespace klog {
namespace detail {
struct Context;
using SharedContext = std::shared_ptr<Context>;
struct Message {

  SharedContext context;
};

struct Buffer {
  static const int BUFFER_SIZE = 4096;
  DISALLOW_COPY(Buffer);
  Buffer() {
    // 如果成员均没有默认初始化，此处不默认初始化，则会出现垃圾值。
    size_ = 0;
    data_ = new Message[BUFFER_SIZE];
  }

  auto begin()   { return data_; }
  auto end()     { return data_ + size_; }
  auto size()    { return size_; }
  auto empty()   { return size_ == 0; }
  auto IsFull()  { return size_ == BUFFER_SIZE; }
  auto IsValid() { return data_ != nullptr; }
  void Reset()   { size_ = 0; }

  Buffer(Buffer &&another) {
    assert((another.data_ == nullptr && another.size_ == -1) ||
           (another.data_ != nullptr && another.size_ >= 0));
    data_ = another.data_;
    size_ = another.size_;
    another.size_ = -1;
    another.data_ = nullptr;
  }

  Buffer &operator=(Buffer &&another) {
    assert((data_ == nullptr && size_ == -1) || (data_ != nullptr && size_ >= 0));
    assert((another.data_ == nullptr && another.size_ == -1) ||
           (another.data_ != nullptr && another.size_ >= 0));
    // 移动赋值，需要检查赋值符号左边是否已经释放。
    if (IsValid()) {
      delete[] data_;
    }
    data_ = another.data_;
    size_ = another.size_;
    another.size_ = -1;
    another.data_ = nullptr;
    return *this;
  }

  ~Buffer() {
    if (IsValid()) {
      delete[] data_;
    }
  }

  void Push(const Message &msg) {
    assert(size_ <= BUFFER_SIZE);
    data_[size_++] = msg;
  }

  Message *data_{};
  int size_{};
};

class AsyncLog {
 public:
  AsyncLog(const char *log_path, size_t roll_size, time_t flush_interval);
  void Append(const Message &message);
  void WorkerFunc();
  ~AsyncLog();
 private:
  DISALLOW_COPY(AsyncLog);
  std::thread worker_;
  size_t      roll_size_{};
  time_t      flush_interval_{};
  const char *log_path_{};

  Buffer                  current_buf_{};
  Buffer                  next_buffer_{};
  std::vector<Buffer>     buffers_{};

  std::mutex              latch_;
  std::condition_variable cv_;
  CountDownLatch          countdown_;
  std::atomic_bool        done_{false};

  std::once_flag          call_once_;

};
} // namespace detail
} // namespace klog
