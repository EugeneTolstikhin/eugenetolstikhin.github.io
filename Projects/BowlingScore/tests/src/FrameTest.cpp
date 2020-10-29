#include "FrameTest.hpp"

TEST(TestBase, InitFrameTestCase)
{
    TestFrame test;
    ASSERT_TRUE(test.m_frame->GetFlag() == Flag::NOTHING);
    ASSERT_TRUE(test.m_frame->GetCurrentTrialNumber() == Trial::ZERO);
    ASSERT_FALSE(test.m_frame->isLastFrame());
    ASSERT_EQ(test.m_frame->GetTrialPoints(Trial::FIRST), 0);
    ASSERT_EQ(test.m_frame->GetTrialPoints(Trial::SECOND), 0);
    ASSERT_EQ(test.m_frame->GetTrialPoints(Trial::THIRD), 0);
}

TEST(TestBase, ModifyFlagsTestCase)
{
    TestFrame test;
    test.m_frame->SetFlag(Flag::SPARE);
    ASSERT_TRUE(test.m_frame->GetFlag() == Flag::SPARE);
    test.m_frame->SetFlag(Flag::STRIKE);
    ASSERT_TRUE(test.m_frame->GetFlag() == Flag::STRIKE);
    test.m_frame->SetCurrentTrialNumber(Trial::FIRST);
    ASSERT_TRUE(test.m_frame->GetCurrentTrialNumber() == Trial::FIRST);
    test.m_frame->SetCurrentTrialNumber(Trial::SECOND);
    ASSERT_TRUE(test.m_frame->GetCurrentTrialNumber() == Trial::SECOND);
    test.m_frame->SetCurrentTrialNumber(Trial::THIRD);
    ASSERT_TRUE(test.m_frame->GetCurrentTrialNumber() == Trial::THIRD);
    test.m_frame->SetLastFrame(true);
    ASSERT_TRUE(test.m_frame->isLastFrame());
    test.m_frame->SetLastFrame(false);
    ASSERT_FALSE(test.m_frame->isLastFrame());
    
    test.m_frame->SetTrialPoints(Trial::FIRST, 2);
    test.m_frame->SetTrialPoints(Trial::SECOND, 7);
    test.m_frame->SetTrialPoints(Trial::THIRD, 1);
    ASSERT_EQ(test.m_frame->GetTrialPoints(Trial::FIRST), 2);
    ASSERT_EQ(test.m_frame->GetTrialPoints(Trial::SECOND), 7);
    ASSERT_EQ(test.m_frame->GetTrialPoints(Trial::THIRD), 1);
}