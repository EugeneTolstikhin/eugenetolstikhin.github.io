cmake_minimum_required(VERSION 3.10)

set (CMAKE_CONFIGURATION_TYPES ${CMAKE_BUILD_TYPE} CACHE STRING "" FORCE)

project(PointsGenerator)

file(GLOB SOURCES "src/PointsGenerator.c")

add_executable(PointsGenerator ${SOURCES})
