include(GoogleTest)
find_package(fmt)

add_executable(buffer_test
               buffer_test.cc
)
target_link_libraries(buffer_test GTest::gtest_main fmt::fmt kilnlog)
gtest_discover_tests(buffer_test)

add_executable(log_file_test
               log_file_test.cc
)
target_link_libraries(log_file_test GTest::gtest_main fmt::fmt kilnlog)
gtest_discover_tests(log_file_test)

add_executable(async_log_test
               async_log_test.cc
)
target_link_libraries(async_log_test GTest::gtest_main fmt::fmt kilnlog)
gtest_discover_tests(async_log_test)

add_executable(log_test
               log_test.cc
)
target_link_libraries(log_test GTest::gtest_main fmt::fmt kilnlog)
gtest_discover_tests(log_test)

add_executable(log_ex log_ex.cc)
target_link_libraries(log_ex GTest::gtest_main fmt::fmt kilnlog)

