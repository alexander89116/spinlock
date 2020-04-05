cmake_minimum_required(VERSION 3.12)
project(Project)

set(CMAKE_CXX_STANDARD 14)

add_executable(Project main.cpp)

include_directories(spinlock)

add_subdirectory(googletest)

include_directories(googletest/googletest/include)
include_directories(googletest/googlemock/include)

add_subdirectory(spinlock)

target_link_libraries(Project gtest gtest_main spinlockLib)