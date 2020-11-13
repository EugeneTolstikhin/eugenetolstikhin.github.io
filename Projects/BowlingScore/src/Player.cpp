#include "Player.h"
#include "Game.h"

Player::Player() : m_PlayerName("Test"), m_Game(new Game)
{
    //
}

Player::Player(const std::string& name, const std::vector<std::shared_ptr<IView>>& views) :
        m_PlayerName(name)
        ,m_Views(views)
        ,m_Game(new Game(m_Views))
{
    for (auto& view : m_Views)
    {
        view->Draw(ViewElement::PLAYER, &m_PlayerName);
    }
}

Player::~Player()
{
    //
}

void Player::Play(const std::vector<std::shared_ptr<IView>>& views, std::function<void()> gameover)
{
    while (m_Game->IsAnotherThrowAllowed())
    {
        m_Game->ThrowBall();
    }

    m_Game->CloseFrame([&gameover]{
        gameover();
    });
}
