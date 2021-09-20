cmake_minimum_required(VERSION 3.10)

set (CMAKE_CONFIGURATION_TYPES ${CMAKE_BUILD_TYPE} CACHE STRING "" FORCE)

project(AdminPanel)

file(GLOB SOURCES "src/AdminPanel.c")

add_executable(AdminPanel ${SOURCES})