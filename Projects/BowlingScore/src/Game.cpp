#include "Game.h"
#include "Frame.h"

Game::Game() : m_Frames(MAX_FRAMES_AMOUNT)
{
    for (auto iter = m_Frames.begin(); iter != m_Frames.end(); ++iter)
    {
        m_Frames.emplace(iter, new Frame);
    }

    m_currFrame = *(m_Frames.begin());
}

Game::~Game()
{
    //
}

void Game::ThrowBall()
{
    points = waitForPoints();
    m_currFrame->SetTrialPoints()
}

bool Game::IsAnotherThrowAllowed() 
{
    //
}

void Game::CloseFrame() 
{
    //
}

void Game::GameOver()
{
    //
}

unsigned short Game::waitForPoints() 
{
    unsigned short points = 0;

    //TODO: By using TCP Listen Socket return a number of points

    return points;
}