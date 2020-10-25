cmake_minimum_required(VERSION 3.10)

project(Bowling)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(interface)
include_directories(hdr)

file(GLOB SOURCES "src/*.cpp")

add_executable(Bowling ${SOURCES})

enable_testing ()
#add_test (NAME BowlingFrameTests COMMAND FrameTest)
#add_test (NAME BowlingGameTests COMMAND GameTest)
#add_test (NAME BowlingPlayerTests COMMAND PlayerTest)
#add_test (NAME BowlingLaneTests COMMAND LaneTest)
