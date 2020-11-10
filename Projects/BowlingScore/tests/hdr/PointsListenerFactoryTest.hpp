#ifndef __POINTSLISTENERFACTORYTEST_H__
#define __POINTSLISTENERFACTORYTEST_H__

#include <gtest/gtest.h>
#include "PointsListenerFactory.h"

class TestPointsListenerFactory
{
public:
    TestPointsListenerFactory() : m_factory(new PointsListenerFactory){}
    ~TestPointsListenerFactory() = default;

    FRIEND_TEST(TestBase, InitPointsListenerFactoryTestCase);

    std::unique_ptr<IPointsListenerFactory> m_factory;
};
#endif // __POINTSLISTENERFACTORYTEST_H__
