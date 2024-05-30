#pragma once
#include "fmt/core.h"
#include <unistd.h>
#include <memory>
#include "context.h"
#include "logger.h"
namespace klog {
namespace detail {

template <typename ...Args>
void LogBase(const char *path, const char *fn, int line, int level, // NOLINT
             fmt::format_string<Args...> format, Args&&... args) {
  // 此代码必须在头文件。因为编译后的.o没有相关的函数
  auto ctx = std::make_shared<Context>();
  ctx->level = level;
  ctx->func_name = fn;
  ctx->short_filename = path;
  ctx->line = line;
  ctx->tid = gettid();
  ctx->text = fmt::format(format, args...);
  Logger::Get().LogWithConfig({ctx});
}

template <typename ...Args>
void LogFatal(const char *path, const char *fn, int line,           // NOLINT
           fmt::format_string<Args...> format, Args&&... args) { // NOLINT
  // 此代码必须在头文件。因为编译后的.o没有相关的函数
  auto ctx = std::make_shared<Context>();
  ctx->level = FATAL_LEVEL;
  ctx->func_name = fn;
  ctx->short_filename = path;
  ctx->line = line;
  ctx->tid = gettid();
  ctx->text = fmt::format(format, args...);
  Logger::Get().LogWithConfig({ctx});
  Logger::Get().CleanUp();
  std::terminate();
}
} // namespace detail
} // namespace klog
#define Trace(...) klog::detail::LogBase(__FILE__, __PRETTY_FUNCTION__, __LINE__, klog::TRACE_LEVEL, __VA_ARGS__)
#define Debug(...) klog::detail::LogBase(__FILE__, __PRETTY_FUNCTION__, __LINE__, klog::DEBUG_LEVEL, __VA_ARGS__)
#define Info(...) klog::detail::LogBase(__FILE__, __PRETTY_FUNCTION__, __LINE__, klog::INFO_LEVEL, __VA_ARGS__)
#define Warn(...) klog::detail::LogBase(__FILE__, __PRETTY_FUNCTION__, __LINE__, klog::WARN_LEVEL, __VA_ARGS__)
#define Error(...) klog::detail::LogBase(__FILE__, __PRETTY_FUNCTION__, __LINE__, klog::ERROR_LEVEL, __VA_ARGS__)
#define Fatal(...) klog::detail::LogFatal(__FILE__, __PRETTY_FUNCTION__, __LINE__, __VA_ARGS__)
