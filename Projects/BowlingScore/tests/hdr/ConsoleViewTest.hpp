#ifndef __CONSOLEVIEWTEST_H__
#define __CONSOLEVIEWTEST_H__

#include <gtest/gtest.h>
#include "ConsoleView.h"

class TestConsoleView
{
public:
    TestConsoleView() : m_view(new ConsoleView){}
    ~TestConsoleView() = default;

    FRIEND_TEST(TestBase, InitConsoleViewTestCase);

    std::unique_ptr<IView> m_view;
};
#endif // __CONSOLEVIEWTEST_H__
