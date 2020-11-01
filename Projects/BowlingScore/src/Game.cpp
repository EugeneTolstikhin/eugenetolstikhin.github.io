#include "Game.h"
#include "Frame.h"

Game::Game()
{
    for (auto iter = m_Frames.begin(); iter != m_Frames.end(); ++iter)
    {
        m_Frames.emplace_front(new Frame);
    }

    m_currFrame = std::make_pair(*(m_Frames.begin()), m_Frames.begin());
}

Game::~Game()
{
    //
}

void Game::ThrowBall()
{
    auto points = waitForPoints();
    m_currFrame.first->SetTrialPoints(points);
}

bool Game::IsAnotherThrowAllowed() const noexcept 
{
    m_currFrame.first->isAllowedThrow();
}

void Game::CloseFrame(std::function<void()> gameOver) 
{
    m_currFrame.second = ++m_currFrame.second;

    if (m_currFrame.second == m_Frames.end())
    {
        gameOver();
    }
    else
    {
        m_currFrame.first = *m_currFrame.second;
    }
}

unsigned short Game::waitForPoints() 
{
    unsigned short points = 0;

    //TODO: By using TCP Listen Socket return a number of points

    return points;
}
