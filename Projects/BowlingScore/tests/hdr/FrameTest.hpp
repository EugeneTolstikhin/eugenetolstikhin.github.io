#include <gtest/gtest.h>
#include "Frame.h"
#include <memory>

class TestFrame
{
public:
    TestFrame() : m_frame(new Frame) {}
    ~TestFrame() = default;

    FRIEND_TEST(TestBase, InitFrameTestCase);
    FRIEND_TEST(TestBase, ModifyFlagsTestCase);

    std::unique_ptr<IFrame> m_frame;
};