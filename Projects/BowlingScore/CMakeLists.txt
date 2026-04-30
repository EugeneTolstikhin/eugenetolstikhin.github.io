cmake_minimum_required(VERSION 3.20)

set(CMAKE_CONFIGURATION_TYPES ${CMAKE_BUILD_TYPE} CACHE STRING "" FORCE)

project(BowlingScore LANGUAGES CXX)

find_package(Curses REQUIRED)
find_package(Qt6 COMPONENTS Core Gui Qml Quick QUIET)
include(CTest)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB SOURCES CONFIGURE_DEPENDS "src/*.cpp")
set(QT_SOURCES "")

if(Qt6_FOUND)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    list(APPEND QT_SOURCES hdr/QtScoreView.h qml/qml.qrc)
else()
    list(FILTER SOURCES EXCLUDE REGEX ".*/QtScoreView\\.cpp$")
endif()

add_executable(BowlingScore ${SOURCES} ${QT_SOURCES})

target_compile_features(BowlingScore PRIVATE cxx_std_23)
target_include_directories(BowlingScore PRIVATE interface hdr /usr/lib/x86_64-linux-gnu)
target_link_libraries(BowlingScore PRIVATE ${CURSES_LIBRARIES})

if(Qt6_FOUND)
    target_compile_definitions(BowlingScore PRIVATE BOWLING_WITH_QT=1)
    target_link_libraries(BowlingScore PRIVATE Qt6::Core Qt6::Gui Qt6::Qml Qt6::Quick)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(BowlingScore PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_custom_command(
    TARGET BowlingScore POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/config.cfg
            ${CMAKE_CURRENT_BINARY_DIR}/config.cfg)

if(BUILD_TESTING)
    add_executable(BowlingScoreScoringRulesTests
        tests/ScoringRulesTests.cpp
        src/Game.cpp
        src/Frame.cpp
        src/LoggerFactory.cpp
        src/FileLogger.cpp
        src/ConsoleLogger.cpp)
    target_compile_features(BowlingScoreScoringRulesTests PRIVATE cxx_std_23)
    target_include_directories(BowlingScoreScoringRulesTests PRIVATE interface hdr /usr/lib/x86_64-linux-gnu)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(BowlingScoreScoringRulesTests PRIVATE -Wall -Wextra -Wpedantic)
    endif()
    add_test(NAME BowlingScoreScoringRulesTests COMMAND BowlingScoreScoringRulesTests)
endif()

