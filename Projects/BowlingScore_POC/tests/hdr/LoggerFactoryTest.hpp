#ifndef __LOGGERFACTORYTEST_H__
#define __LOGGERFACTORYTEST_H__

#include <gtest/gtest.h>
#include "LoggerFactory.h"

class TestLoggerFactory
{
public:
    TestLoggerFactory() : m_factory(new LoggerFactory){}
    ~TestLoggerFactory() = default;

    FRIEND_TEST(TestBase, InitLoggerFactoryTestCase);

    std::unique_ptr<ILoggerFactory> m_factory;
};
#endif // __LOGGERFACTORYTEST_H__
