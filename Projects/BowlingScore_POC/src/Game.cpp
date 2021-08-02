#include "Game.h"
#include "Frame.h"
#include "IPointsListener.h"
#include "PointsListenerFactory.h"
#include "LoggerFactory.h"
#include "FileLogger.h"

Game::Game() :
    m_pointsListenerFactory(new PointsListenerFactory)
    ,m_loggerFactory(new LoggerFactory)
    ,m_log(m_loggerFactory->CreateLogger(LoggerType::TO_FILE))
{
    //
}

Game::Game(const std::vector<std::shared_ptr<IView>>& views) :
    m_pointsListenerFactory(new PointsListenerFactory)
    ,m_Views(views)
    ,m_loggerFactory(new LoggerFactory)
    ,m_log(m_loggerFactory->CreateLogger(LoggerType::TO_FILE))
{
    //This parameter can be read from a config file
    m_FramesAmount = MAX_FRAME_AMOUNT;
    m_Frames.reserve(m_FramesAmount);

    size_t counter = 0;
    while (++counter <= m_FramesAmount)
    {
        m_Frames.emplace_back(new Frame(counter == m_FramesAmount, m_Views));
    }

    m_currFrame = std::make_pair(m_Frames.front(), m_Frames.begin());
}

Game::~Game()
{
    //
}

void Game::ThrowBall()
{
    auto points = waitForPoints();

    //m_log->LogMe(__FILE__, __LINE__, std::string("Amount of points are ") + std::to_string(points));

    m_currFrame.first->SetTrialPoints(points);
}

bool Game::IsAnotherThrowAllowed() 
{
    return m_currFrame.first->isAllowedThrow();
}

void Game::CloseGame(std::function<void()> gameOver) 
{
    if (++m_currFrame.second == m_Frames.end())
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

    std::shared_ptr<IPointsListener> listener(m_pointsListenerFactory->CreatePointsListener(m_listenerType));
    listener->Connect();
    points = listener->Receive();   // TODO: Check why 0 is here
    listener->Shutdown();
    return points;
}
