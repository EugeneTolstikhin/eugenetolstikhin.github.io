#ifndef __POINTSLISTENERSIMULATIONTEST_H__
#define __POINTSLISTENERSIMULATIONTEST_H__

#include <gtest/gtest.h>
#include "PointsListenerSimulation.h"

class TestPointsListenerSimulation
{
public:
    TestPointsListenerSimulation() : m_listener(new PointsListenerSimulation){}
    ~TestPointsListenerSimulation() = default;

    FRIEND_TEST(TestBase, InitPointsListenerSimulationTestCase);

    std::unique_ptr<IPointsListener> m_listener;
};
#endif // __POINTSLISTENERSIMULATIONTEST_H__
