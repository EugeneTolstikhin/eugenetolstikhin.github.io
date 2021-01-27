#ifndef __VIEWFACTORYTEST_H__
#define __VIEWFACTORYTEST_H__

#include <gtest/gtest.h>
#include "ViewFactory.h"

class TestViewFactory
{
public:
    TestViewFactory() : m_factory(new ViewFactory){}
    ~TestViewFactory() = default;

    FRIEND_TEST(TestBase, InitViewFactoryFactoryTestCase);

    std::unique_ptr<IViewFactory> m_factory;
};
#endif // __VIEWFACTORYTEST_H__
