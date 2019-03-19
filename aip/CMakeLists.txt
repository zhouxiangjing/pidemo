
cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -static-libgcc -static-libstdc++")

ADD_LIBRARY(aip STATIC aip.cpp)

target_link_libraries(aip curl jsoncpp crypto)
