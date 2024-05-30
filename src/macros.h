#pragma once
#define DISALLOW_COPY(classname) \
  classname(const classname &) = delete;\
  const classname& operator=(const classname &) = delete;
#define DISALLOW_COPYMOVE(classname) \
  DISALLOW_COPY(classname); \
  classname(classname &&) = delete;\
  classname&& operator=(classname &&) = delete;
namespace klog {
const int TRACE_LEVEL = 0;
const int DEBUG_LEVEL = 1;
const int INFO_LEVEL  = 2;
const int WARN_LEVEL  = 3;
const int ERROR_LEVEL = 4;
const int FATAL_LEVEL = 5;
} // namespace klog
