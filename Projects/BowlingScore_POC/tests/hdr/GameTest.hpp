#ifndef __GAMETEST_H__
#define __GAMETEST_H__

#include <gtest/gtest.h>
#include "Game.h"
#include <memory>

class TestGame
{
public:
    TestGame() : m_game(new Game) {}
    ~TestGame() = default;

    FRIEND_TEST(TestBase, InitGameTestCase);

    std::unique_ptr<IGame> m_game;
};
#endif // __GAMETEST_H__
