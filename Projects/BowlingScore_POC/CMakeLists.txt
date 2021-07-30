cmake_minimum_required(VERSION 3.10)

set (CMAKE_CONFIGURATION_TYPES ${CMAKE_BUILD_TYPE} CACHE STRING "" FORCE)
project(Bowling)

find_package(Curses REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lncurses")

include_directories(interface)
include_directories(hdr)

file(GLOB SOURCES "src/*.cpp")

add_executable(Bowling ${SOURCES})

#add_subdirectory (tests)
#enable_testing ()
#add_test (NAME BowlingTests COMMAND TestBase)
