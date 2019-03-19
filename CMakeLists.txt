
cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -static-libgcc -static-libstdc++")

add_subdirectory(aip)
add_subdirectory(soundmanager)

add_executable(pidemo pidemo.cpp)

target_link_libraries(pidemo aip soundmanager)