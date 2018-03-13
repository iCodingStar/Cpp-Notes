cmake_minimum_required(VERSION 3.9)
project(Cpp_Notes)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-std=c++11")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2 -Wall")

# 设置boost库的根路径
set(BOOST_ROOT "/usr/local/include/boost")
set(UNP_ROOT "/usr/lib")
# 查找boost的功能模块
find_package(Boost 1.66.0 COMPONENTS log thread system regex date_time REQUIRED)

include_directories(.)
add_subdirectory(thread)
add_executable(main main.cpp)

# cmake tutorials
add_subdirectory(cmake/tutorials-1)
add_subdirectory(cmake/tutorials-2)
add_subdirectory(AliTest)
add_subdirectory(io)