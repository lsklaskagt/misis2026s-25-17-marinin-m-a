cmake_minimum_required(VERSION 3.15)
project(marinin-m-a-25-17)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(prj.labs)
add_subdirectory(prj.test)

add_executable(test main.cpp)
