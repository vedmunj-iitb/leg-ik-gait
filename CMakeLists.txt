cmake_minimum_required(VERSION 3.10)
project(leg_ik_gait CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(gait main.cpp)
add_executable(leg_test test.cpp)

enable_testing()
add_test(NAME leg_test COMMAND leg_test)
