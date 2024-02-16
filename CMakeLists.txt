cmake_minimum_required(VERSION 3.26)
project(btc)

set(CMAKE_CXX_STANDARD 98)

include_directories(./src)

AUX_SOURCE_DIRECTORY(./src SRCS)

add_executable(a.out ${SRCS})
