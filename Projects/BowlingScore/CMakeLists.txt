cmake_minimum_required(VERSION 3.10)

set (CMAKE_CONFIGURATION_TYPES ${CMAKE_BUILD_TYPE} CACHE STRING "" FORCE)
project(Bowling)

find_package(Curses REQUIRED)
include(CTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lncurses")

include_directories("/usr/lib/x86_64-linux-gnu")
include_directories(interface)
include_directories(hdr)

file(GLOB SOURCES "src/*.cpp")

add_executable(Bowling ${SOURCES})
target_link_libraries(Bowling curses)

add_custom_command(
        TARGET Bowling POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/config.cfg
                ${CMAKE_CURRENT_BINARY_DIR}/config.cfg)

if(BUILD_TESTING)
    add_executable(BowlingScoringRulesTests
        tests/ScoringRulesTests.cpp
        src/Game.cpp
        src/Frame.cpp
        src/LoggerFactory.cpp
        src/FileLogger.cpp
        src/ConsoleLogger.cpp)
    target_include_directories(BowlingScoringRulesTests PRIVATE interface hdr /usr/lib/x86_64-linux-gnu)
    target_compile_features(BowlingScoringRulesTests PRIVATE cxx_std_17)
    add_test(NAME BowlingScoringRulesTests COMMAND BowlingScoringRulesTests)
endif()
