prOJECT(main)
 CMAKE_MINIMUM_REQUIRED(VERSION 3.15.2)
 AUX_SOURCE_DIRECTORY(. DIR_SRCS)
include_directories(.)
 ADD_EXECUTABLE(main ${DIR_SRCS})
set(CMAKE_CXX_COMPILER "g++")
add_definitions(-std=c++11)

target_link_libraries (main pthread)
