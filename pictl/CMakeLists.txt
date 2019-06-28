
cmake_minimum_required(VERSION 3.4.1)

add_library(pictl STATIC pictl.cpp)

target_link_libraries(pictl pthread wiringPi)