cmake_minimum_required(VERSION 3.4.1)

include_directories(${CMAKE_SOURCE_DIR}/hpsocket/include)

add_library(hpsocket STATIC IMPORTED)
set_target_properties(hpsocket PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/hpsocket/lib/libhpsocket.a)

add_executable(pidemo pidemo.cpp hpsocket/helper.cpp)

add_subdirectory(pictl)

target_link_libraries(pidemo pthread hpsocket pictl)
