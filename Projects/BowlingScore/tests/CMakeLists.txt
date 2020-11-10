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
set (CONSOLE_VIEW_INC ../interface/IView.h ../hdr/ConsoleView.h ../src/ConsoleView.cpp hdr/ConsoleViewTest.hpp src/ConsoleViewTest.cpp)
set (POINTS_LISTENER_SIMULATION_INC ../interface/IPointsListener.h ../hdr/PointsListenerSimulation.h ../src/PointsListenerSimulation.cpp hdr/PointsListenerSimulationTest.hpp src/PointsListenerSimulationTest.cpp)
set (POINTS_LISTENER_LOCAL_INC ../interface/IPointsListener.h ../hdr/PointsListenerLocal.h ../src/PointsListenerLocal.cpp hdr/PointsListenerLocalTest.hpp src/PointsListenerLocalTest.cpp)
set (POINTS_LISTENER_FACTORY_INC ../interface/IPointsListenerFactory.h ../hdr/PointsListenerFactory.h ../src/PointsListenerFactory.cpp hdr/PointsListenerFactoryTest.hpp src/PointsListenerFactoryTest.cpp)

# Add test cpp file
add_executable (TestBase
    ${FRAME_INC}
    ${GAME_INC}
    ${LANE_INC}
    ${PLAYER_INC}
    ${CONSOLE_VIEW_INC}
    ${POINTS_LISTENER_SIMULATION_INC}
    ${POINTS_LISTENER_LOCAL_INC}
    ${POINTS_LISTENER_FACTORY_INC}
)

# Link test executable against gtest & gtest_main
target_link_libraries (TestBase ${GTEST_INCLUDE})