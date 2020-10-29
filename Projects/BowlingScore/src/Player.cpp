#include "Player.h"
#include "Game.h"

Player::Player() : m_PlayerName("Test"), m_Game(new Game)
{
    //
}

Player::Player(const std::string& name) : m_PlayerName(name)
{
    //
}

Player::~Player()
{
    //
}

void Player::Play()
{
    while (m_Game->IsAnotherThrowAllowed())
    {
        m_Game->ThrowBall();
    }
    m_Game->CloseFrame([]{
        //TODO: Implement game over functionality
    });
}
