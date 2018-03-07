add_executable(thread thread.cpp)
# 目标进行链接
target_link_libraries(thread ${Boost_LIBRARIES})