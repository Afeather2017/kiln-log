#include "async_log.h"
#include "context.h"
#include "gtest/gtest.h"
TEST(AsyncLog, PutManyTest) {
  klog::detail::SharedContext ctx = {std::make_shared<klog::detail::Context>()};
  klog::detail::Message msg = {ctx};
  ctx->text = "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcde\n";
  ctx->short_filename = __FILE__;
  ctx->func_name = __PRETTY_FUNCTION__;
  ctx->line = __LINE__;
  ctx->tid = gettid();
  klog::detail::AsyncLog al{"./logs", 4096, 1};
  for (int i = 0; i < 10000000; i++) {
    al.Append(msg);
  }
}
