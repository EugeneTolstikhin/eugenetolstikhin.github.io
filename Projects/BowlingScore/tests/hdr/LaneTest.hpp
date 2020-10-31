#ifndef __LANETEST_H__
#define __LANETEST_H__

#include <gtest/gtest.h>
#include "Lane.h"
#include <memory>

class TestLane
{
public:
    TestLane() : m_lane(new Lane) {}
    ~TestLane() = default;

    FRIEND_TEST(TestBase, InitLaneTestCase);

    std::unique_ptr<ILane> m_lane;
};
#endif // __LANETEST_H__
