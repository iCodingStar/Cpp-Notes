# 指定cmake的最低版本号
cmake_minimum_required(VERSION 2.8)

# 制定项目名称
project(Tutorial-1)

# The version number
set(Tutorial_VERSION_MAJOR 55)
set(Tutorial_VERSION_MINOR 0)

# configure_file
configure_file(
        "${PROJECT_SOURCE_DIR}/TutorialConfig.h.in"
        "${PROJECT_BINARY_DIR}/TutorialConfig.h"
)

include_directories(${PROJECT_BINARY_DIR})
#
add_executable(Tutorial-1 main.cpp)