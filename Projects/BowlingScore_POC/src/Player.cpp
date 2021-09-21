#include "Player.h"
#include "Game.h"
#include "LoggerFactory.h"
#include "FileLogger.h"

Player::Player(IView* view, GetPointsFunction getPoints) :
    m_PlayerName("Test")
    ,m_view(view)
    ,m_Game(new Game(view, getPoints))
    ,m_loggerFactory(new LoggerFactory)
    ,m_log(m_loggerFactory->CreateLogger(m_typeLogger))
{
    //m_log->LogMe(__FILE__, __LINE__, __FUNCTION__);
    if (m_view != nullptr)
    {
        m_view->Draw(ViewElement::PLAYER, &m_PlayerName);
    }
    else
    {
        //m_log->LogMe(__FILE__, __LINE__, "view is UNAVAILABLE");
    }
}

Player::Player(const std::string& name, IView* view, GetPointsFunction getPoints) :
        m_PlayerName(name)
        ,m_view(view)
        ,m_Game(new Game(view, getPoints))
{
    //m_log->LogMe(__FILE__, __LINE__, __FUNCTION__);
    if (m_view != nullptr)
    {
        m_view->Draw(ViewElement::PLAYER, &m_PlayerName);
    }
    else
    {
        //m_log->LogMe(__FILE__, __LINE__, "view is UNAVAILABLE");
    }
}

Player::~Player()
{
    //m_log->LogMe(__FILE__, __LINE__, __FUNCTION__);
}

void Player::Play(std::function<void()> gameover)
{
    //m_log->LogMe(__FILE__, __LINE__, __FUNCTION__);
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
