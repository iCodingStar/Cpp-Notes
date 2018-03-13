project(daytime)

set(CMAKE_C_COMPILER )
add_executable(daytimetcpcli daytimetcpcli.c)
add_executable(daytimetcpsrv daytimetcpsrv.c)

target_link_libraries(daytimetcpsrv -lunp)