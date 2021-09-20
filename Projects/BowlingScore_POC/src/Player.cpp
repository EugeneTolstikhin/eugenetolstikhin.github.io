#include "Player.h"
#include "Game.h"

Player::Player(const std::vector<std::shared_ptr<IView>>& views, GetPointsFunction getPoints) :
    m_PlayerName("Test")
    ,m_Views(views)
    ,m_Game(new Game(m_Views, getPoints))
{
    for (auto& view : m_Views)
    {
        view->Draw(ViewElement::PLAYER, &m_PlayerName);
    }
}

Player::Player(const std::string& name, const std::vector<std::shared_ptr<IView>>& views, GetPointsFunction getPoints) :
        m_PlayerName(name)
        ,m_Views(views)
        ,m_Game(new Game(m_Views, getPoints))
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

    m_Game->UpdateTotalScore();

    m_Game->CloseGame([&gameover]
    {
        gameover();
    });
}
