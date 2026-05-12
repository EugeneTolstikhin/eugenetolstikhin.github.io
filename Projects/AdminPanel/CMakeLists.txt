cmake_minimum_required(VERSION 3.16)

set (CMAKE_CONFIGURATION_TYPES ${CMAKE_BUILD_TYPE} CACHE STRING "" FORCE)

project(AdminPanel LANGUAGES C CXX)

file(GLOB SOURCES "src/AdminPanel.c")

add_executable(AdminPanel ${SOURCES})

find_package(Qt6 COMPONENTS Core Gui Qml Quick Network QUIET)

if(Qt6_FOUND)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    add_executable(AdminPanelQt src/AdminPanelQt.cpp qml/qml.qrc)
    target_compile_features(AdminPanelQt PRIVATE cxx_std_17)
    target_link_libraries(AdminPanelQt PRIVATE Qt6::Core Qt6::Gui Qt6::Qml Qt6::Quick Qt6::Network)
endif()

add_custom_command(
        TARGET AdminPanel POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/src/config.cfg
                ${CMAKE_CURRENT_BINARY_DIR}/config.cfg)

if(Qt6_FOUND)
    add_custom_command(
            TARGET AdminPanelQt POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
                    ${CMAKE_SOURCE_DIR}/src/config.cfg
                    ${CMAKE_CURRENT_BINARY_DIR}/config.cfg)
endif()
