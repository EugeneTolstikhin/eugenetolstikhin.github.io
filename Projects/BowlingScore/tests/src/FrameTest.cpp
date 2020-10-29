#include "FrameTest.hpp"

TEST(TestBase, InitFrameTestCase)
{
    TestFrame test;
    ASSERT_TRUE(test.m_frame->GetCurrentTrialNumber() == Trial::ZERO);
}

TEST(TestBase, ModifyFlagsTestCase)
{
    TestFrame test;
    test.m_frame->SetCurrentTrialNumber(Trial::FIRST);
    ASSERT_TRUE(test.m_frame->GetCurrentTrialNumber() == Trial::FIRST);
    test.m_frame->SetCurrentTrialNumber(Trial::SECOND);
    ASSERT_TRUE(test.m_frame->GetCurrentTrialNumber() == Trial::SECOND);
    test.m_frame->SetCurrentTrialNumber(Trial::THIRD);
    ASSERT_TRUE(test.m_frame->GetCurrentTrialNumber() == Trial::THIRD);
}