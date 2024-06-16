cmake_minimum_required(VERSION 3.14)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

find_package(fmt)
add_library(kilnlog
            ${PROJECT_SOURCE_DIR}/src/async_log.cc
            ${PROJECT_SOURCE_DIR}/src/file.cc)
set_target_properties(kilnlog PROPERTIES LINKER_LANGUAGE CXX)

target_sources(kilnlog
               PUBLIC FILE_SET kilnlogheader TYPE HEADERS
               FILES ${PROJECT_SOURCE_DIR}/src/kiln-log.h)
