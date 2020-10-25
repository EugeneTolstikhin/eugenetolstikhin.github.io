find_package (GTest REQUIRED)

add_compile_definitions(UNIT_TEST)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ../includes)
set (CMAKE_CXX_STANDARD 17)