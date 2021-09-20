cmake_minimum_required(VERSION 3.10)

set (CMAKE_CONFIGURATION_TYPES ${CMAKE_BUILD_TYPE} CACHE STRING "" FORCE)
project(Bowling)

find_package(Curses REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lncurses")

include_directories("/usr/lib/x86_64-linux-gnu")
include_directories(interface)
include_directories(hdr)

file(GLOB SOURCES "src/*.cpp")

add_executable(Bowling ${SOURCES})
target_link_libraries(Bowling curses)
