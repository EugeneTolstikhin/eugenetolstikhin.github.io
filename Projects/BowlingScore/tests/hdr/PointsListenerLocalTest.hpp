#ifndef __POINTSLISTENERLOCALTEST_H__
#define __POINTSLISTENERLOCALTEST_H__

#include <gtest/gtest.h>
#include "PointsListenerLocal.h"

class TestPointsListenerLocal
{
public:
    TestPointsListenerLocal() : m_listener(new PointsListenerLocal){}
    ~TestPointsListenerLocal() = default;

    FRIEND_TEST(TestBase, InitPointsListenerLocalTestCase);

    std::unique_ptr<IPointsListener> m_listener;
};
#endif // __POINTSLISTENERLOCALTEST_H__
