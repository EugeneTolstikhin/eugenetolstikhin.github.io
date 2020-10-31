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
set (FRAME_INC ../interface/IFrame.h ../hdr/Frame.h ../src/Frame.cpp hdr/FrameTest.hpp src/FrameTest.cpp)
set (GAME_INC ../interface/IGame.h ../hdr/Game.h ../src/Game.cpp hdr/GameTest.hpp src/GameTest.cpp)
set (LANE_INC ../interface/ILane.h ../hdr/Lane.h ../src/Lane.cpp hdr/LaneTest.hpp src/LaneTest.cpp)
set (PLAYER_INC ../interface/IPlayer.h ../hdr/Player.h ../src/Player.cpp hdr/PlayerTest.hpp src/PlayerTest.cpp)

# Add test cpp file
add_executable (TestBase
    ${FRAME_INC}
    ${GAME_INC}
    ${LANE_INC}
    ${PLAYER_INC}
)

# Link test executable against gtest & gtest_main
target_link_libraries (TestBase ${GTEST_INCLUDE})