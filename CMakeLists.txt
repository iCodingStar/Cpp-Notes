cmake_minimum_required(VERSION 3.9)
project(Cpp_Notes)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)
add_subdirectory(thread)
add_executable(main main.cpp)