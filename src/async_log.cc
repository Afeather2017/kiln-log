#include "async_log.h"
#include "context.h"
#include <chrono>
#include <mutex>
#include "file.h"
namespace klog {
namespace detail {
AsyncLog::AsyncLog(const char *log_path,                 // NOLINT  
                   size_t roll_size,                     // NOLINT
                   time_t flush_interval):               // NOLINT
                      log_path_{log_path},               // NOLINT
                      roll_size_{roll_size},             // NOLINT
                      flush_interval_{flush_interval},   // NOLINT
                      countdown_{1},                     // NOLINT
                      worker_{[this]{ WorkerFunc(); }} {
  // 等待线程启动
  countdown_.Wait();
}

void AsyncLog::WorkerFunc() {
  try {
    RollingLog rl{log_path_};

    Buffer new_buf_1, new_buf_2;

    std::vector<Buffer> buffer_to_write;
    buffer_to_write.reserve(16);

    while (!done_.load()) {
      {
        std::unique_lock<std::mutex> lck{latch_};
        if (buffers_.empty()) {
          // 仅仅在线程启动的时候调用一次，
          // 用于解决构造函数构造完成了而线程未准备好的问题
          std::call_once(call_once_, [this](){ countdown_.CountDown(); });
          
          // 等待flush_interval_秒后会结束，
          // 或者被由于current_buf_满了被唤醒
          cv_.wait_for(lck, std::chrono::seconds(flush_interval_));
        }
        
        if (current_buf_.IsValid() && !current_buf_.empty()) {
          buffers_.push_back(std::move(current_buf_));
          current_buf_ = std::move(new_buf_1);
        }

        std::swap(buffer_to_write, buffers_);
        if (!next_buffer_.IsValid()) {
          next_buffer_ = Buffer{};
        }

        if (buffer_to_write.empty()) {
          continue;
        }

        if (buffer_to_write.size() > 100) {
          // buffer过满，此时考虑丢弃日志。
          char buf[256];
          int len = sprintf(buf, "Drop %zd messages", buffer_to_write.size() - 2);
          rl.Append(buf, len);
          buffer_to_write.resize(2);
        }

        for (auto &buffer : buffer_to_write) {
          for (auto &message : buffer) {
            auto str = message.Format();
            rl.Append(str.c_str(), str.size());
          }
        }

        if (buffer_to_write.size() < 2) {
          buffer_to_write.resize(2);
        }

        if (!new_buf_1.IsValid()) {
          new_buf_1 = std::move(buffer_to_write.back());
          buffer_to_write.pop_back();
          new_buf_1.Reset();
        }

        if (!new_buf_2.IsValid()) {
          new_buf_2 = std::move(buffer_to_write.back());
          buffer_to_write.pop_back();
          new_buf_1.Reset();
        }
      }
      buffer_to_write.clear();
      rl.Flush();
    }
    rl.Flush();
  } catch (...) {
    fprintf(stderr, "Unknow error in kiln-log worker thread\n");
  }
}

void AsyncLog::Append(const Message &message) {
  std::unique_lock<std::mutex> lck{latch_};
  // 双缓冲代码，此为该代码的核心部分。
  if (!current_buf_.IsFull()) {
    // 此Buffer没有满，直接尾插新值。
    current_buf_.Push(message);
    return;
  }
  buffers_.push_back(std::move(current_buf_));
  if (next_buffer_.IsValid()) {
    // 备用内存还在，则直接用它。
    current_buf_ = std::move(next_buffer_);
  } else {
    current_buf_ = Buffer{};
  }
  current_buf_.Push(message);
  cv_.notify_one();
}

AsyncLog::~AsyncLog() {
  done_ = true;
  cv_.notify_one();
  worker_.join();
}
} // namespace detail
} // namespace klog
