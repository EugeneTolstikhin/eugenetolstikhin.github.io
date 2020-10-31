#ifndef __FRAMETEST_H__
#define __FRAMETEST_H__

#include <gtest/gtest.h>
#include "Frame.h"
#include <memory>

class TestFrame
{
public:
    TestFrame() : m_frame(new Frame){}
    ~TestFrame() = default;

    FRIEND_TEST(TestBase, InitFrameTestCase);

    std::unique_ptr<IFrame> m_frame;
};
#endif // __FRAMETEST_H__
