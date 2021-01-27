#ifndef __PLAYERTEST_H__
#define __PLAYERTEST_H__

#include <gtest/gtest.h>
#include "Player.h"
#include <memory>

class TestPlayer
{
public:
    TestPlayer() : m_player(new Player) {}
    ~TestPlayer() = default;

    FRIEND_TEST(TestBase, InitPlayerTestCase);

    std::unique_ptr<IPlayer> m_player;
};
#endif // __PLAYERTEST_H__
