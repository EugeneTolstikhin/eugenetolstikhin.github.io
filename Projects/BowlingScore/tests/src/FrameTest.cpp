#include "FrameTest.hpp"

TEST(TestBase, InitFrameTestCase)
{
    TestFrame test;
    ASSERT_TRUE(test.m_frame->GetFlag() == Flag::NOTHING);
    ASSERT_TRUE(test.m_frame->GetCurrentTrialNumber() == Trial::ZERO);
    ASSERT_FALSE(test.m_frame->is10thFrame());
    ASSERT_EQ(test.m_frame->GetTrialPoints(Trial::FIRST), 0);
    ASSERT_EQ(test.m_frame->GetTrialPoints(Trial::SECOND), 0);
    ASSERT_EQ(test.m_frame->GetTrialPoints(Trial::THIRD), 0);
}