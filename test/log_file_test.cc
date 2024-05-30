#include "file.h"
#include "util.h"
#include "gtest/gtest.h"

TEST(LogFile, RollTest) {
  // 测试Append后是否新建了文件
  system("mkdir ./logs");
  klog::detail::RollingLog rl("./logs", 65536, 3);
  // 64 字节，写完到Roll需要65536/64=1024次
  char data[] = "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcde\n";
  int len = strlen(data);
  for (int i = 1020; i > 0; i--) {
    rl.Append(data, len);
  }
  system("sleep 2");
  auto command = "ls " + klog::detail::GetLogPathName("./logs", time(nullptr));
  printf("check with %s\n", command.c_str());
  for (int i = 5; i > 0; i--) {
    rl.Append(data, len);
  }
  ASSERT_EQ(system(command.c_str()), 0);
  system("rm -r ./logs");
}
