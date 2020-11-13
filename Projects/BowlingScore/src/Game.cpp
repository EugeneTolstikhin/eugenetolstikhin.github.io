#include "Game.h"
#include "Frame.h"
#include "IPointsListener.h"
#include "PointsListenerFactory.h"

Game::Game() : m_pointsListenerFactory(new PointsListenerFactory)
{
    //
}

Game::Game(const std::vector<std::shared_ptr<IView>>& views) :
    m_pointsListenerFactory(new PointsListenerFactory)
    ,m_Views(views)
{
    //This parameter can be read from a config file
    m_FramesAmount = 10;
    m_Frames.reserve(m_FramesAmount);

    size_t counter = 0;
    while (++counter <= m_FramesAmount)
    {
        m_Frames.emplace_back(new Frame(counter == m_FramesAmount, m_Views));
    }

    m_currFrame = std::make_pair(*(m_Frames.begin()), m_Frames.begin());

    for (auto& view : m_Views)
    {
        view->Draw(ViewElement::GAME);
    }
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

bool Game::IsAnotherThrowAllowed() 
{
    return m_currFrame.first->isAllowedThrow();
}

void Game::CloseFrame(std::function<void()> gameOver) 
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
    points = listener->Receive();
    listener->Shutdown();
    return points;
}
