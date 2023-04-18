cmake_minimum_required(VERSION 3.24)

project(safe-read)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")

add_library(libsaferead libsaferead.cpp libsaferead.h)
add_executable(test test.cpp)

target_link_libraries(test libsaferead)