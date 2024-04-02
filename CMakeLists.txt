cmake_minimum_required(VERSION 3.27)
project(newcompress)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(newcompress src/main.cpp
        include/utils.h
        src/utils.cpp
        include/Rom.h
        src/Rom.cpp)
