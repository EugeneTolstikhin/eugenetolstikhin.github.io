#ifndef __CONSOLELOGGERTEST_H__
#define __CONSOLELOGGERTEST_H__

#include <gtest/gtest.h>
#include "ConsoleLogger.h"

class TestConsoleLogger
{
public:
    TestConsoleLogger() : m_logger(new ConsoleLogger){}
    ~TestConsoleLogger() = default;

    FRIEND_TEST(TestBase, InitConsoleLoggerTestCase);

    std::unique_ptr<ILogger> m_logger;
};
#endif // __CONSOLEVIEWTEST_H__
