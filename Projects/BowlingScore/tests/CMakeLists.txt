find_package (GTest REQUIRED)

add_compile_definitions(UNIT_TEST)
include_directories(
    ${gtest_SOURCE_DIR}/include 
    ${gtest_SOURCE_DIR} 
    ../hdr
    hdr
)

set (CMAKE_CXX_STANDARD 17)
set (GTEST_INCLUDE gtest gtest_main pthread)
set (FRAME_INC ../interface/IFrame.h ../hdr/Frame.h ../src/Frame.cpp)

# Add test cpp file
add_executable (TestBase
    ${FRAME_INC}
    hdr/FrameTest.hpp
    src/FrameTest.cpp
)

# Link test executable against gtest & gtest_main
target_link_libraries (TestBase ${GTEST_INCLUDE})