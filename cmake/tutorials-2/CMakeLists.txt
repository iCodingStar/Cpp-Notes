cmake_minimum_required(VERSION 3.5)

include_directories("${PROJECT_SOURCE_DIR}/Maths")

add_subdirectory(Maths)

add_executable(Tutorial-2 main.cpp)

