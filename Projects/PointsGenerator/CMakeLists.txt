cmake_minimum_required(VERSION 3.10)

set (CMAKE_CONFIGURATION_TYPES ${CMAKE_BUILD_TYPE} CACHE STRING "" FORCE)

project(PointsGenerator)

file(GLOB SOURCES "src/PointsGenerator.c")

add_executable(PointsGenerator ${SOURCES})

add_custom_command(
        TARGET PointsGenerator POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/src/config.cfg
                ${CMAKE_CURRENT_BINARY_DIR}/config.cfg)
