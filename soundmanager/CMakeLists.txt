
cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -static-libgcc -static-libstdc++")

add_LIBRARY(soundmanager

		STATIC

		soundmanager.cpp
		soundplayer.cpp
		soundrecorder.cpp
		soundstream.cpp
		soundwavstream.cpp
		soundrecordstream.cpp
		soundpcm.cpp
		soundpcmenums.cpp
		soundpcmhwparams.cpp)

target_link_libraries(soundmanager pthread asound)
