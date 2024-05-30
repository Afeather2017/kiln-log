#include "async_log.h"
#include "context.h"
#include "gtest/gtest.h"

TEST(Buffer, MoveAssign) {
  // 测试移动赋值后，Buffer中的内存块确实被释放了
  klog::detail::Buffer a{}, b{};
  void *data_a = a.data_;
  void *data_b = b.data_;
  klog::detail::SharedContext ctx = {std::make_shared<klog::detail::Context>()};
  klog::detail::Message msg = {ctx};
  a.Push(msg);
  a = std::move(b);
  ASSERT_EQ(ctx.use_count(), 2);
  ASSERT_EQ(data_b, a.data_);
  ASSERT_EQ(b.data_, nullptr);
  ASSERT_EQ(b.size_, -1);
}

TEST(Buffer, MoveCtor) {
  // 测试移动构造后，内存块确实被转移了
  klog::detail::Buffer a{};
  void *data_a = a.data_;
  klog::detail::Buffer b{std::move(a)};
  void *data_b = b.data_;
  ASSERT_EQ(data_b, data_a);
  ASSERT_EQ(a.data_, nullptr);
}

TEST(Buffer, FullTest) {
  klog::detail::SharedContext ctx = {std::make_shared<klog::detail::Context>()};
  klog::detail::Message msg = {ctx};
  klog::detail::Buffer a;
  for (int i = 0; i < klog::detail::Buffer::BUFFER_SIZE; i++) {
    a.Push(msg);
  }
  ASSERT_EQ(a.IsFull(), true);
}
