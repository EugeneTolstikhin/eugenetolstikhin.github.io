#include "Game.h"
#include "Frame.h"
#include "IPointsListener.h"
#include "PointsListenerFactory.h"
#include "LoggerFactory.h"
#include "FileLogger.h"

#include <numeric>
#include <algorithm>

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
    size_t counter = 0;
    while (++counter <= MAX_FRAME_AMOUNT)
    {
        m_Frames.emplace_back(new Frame(counter == MAX_FRAME_AMOUNT, m_Views));
    }

    m_currFrame = std::make_pair(m_Frames.front(), m_Frames.begin());
}

Game::~Game()
{
    //
}

void Game::ThrowBall()
{
    auto points = m_currFrame.first->isLastFrame() ? 0 : 10;//waitForPoints();

    Flag& flag = m_currFrame.first->SetTrialPoints(points);
    
    m_log->LogMe(__FILE__, __LINE__, std::string("flag = ") +
                                    std::to_string(static_cast<int>(flag)));

    if (!m_lastFlags.empty())
    {
        if (m_lastFlags.back() == Flag::SPARE)
        {
            UpdateTotalScore(-1);
            m_lastFlags.pop_back();
        }
    }

    m_lastFlags.push_back(flag);

    if (m_currFrame.first->isLastFrame())
    {
        /*if (m_lastFrameCounter < 0)
        {
            UpdateTotalScore(m_lastFrameCounter);
            ++m_lastFrameCounter;
        }*/
    }
}

bool Game::IsAnotherThrowAllowed() const noexcept
{
    return m_currFrame.first->isAllowedThrow();
}

void Game::UpdateTotalScore(const short shift)
{
    auto total = m_currFrame.first->GetTotalFramePoints();
    m_frameTotalPoints += total;

    m_log->LogMe(__FILE__, __LINE__, std::string("m_frameTotalPoints = " + std::to_string(m_frameTotalPoints)));

    if (shift == 0 && m_lastFlags.back() == Flag::STRIKE)
    {
        m_framePoints.push_back(m_frameTotalPoints);
    }

    if (shift == 0 && m_lastFlags.back() != Flag::STRIKE)
    {
        m_log->LogMe(__FILE__, __LINE__, std::string("m_framePoints.size() = " + std::to_string(m_framePoints.size())));
        unsigned short counter = 0;
        for (auto& val: m_framePoints)
        {
            val += total + 10 * (m_framePoints.size() - 1 + counter);
            m_frameTotalPoints += total + 10 * (m_framePoints.size() - 2 + counter);

            m_log->LogMe(__FILE__, __LINE__, std::string("val = " + std::to_string(val)));
            m_log->LogMe(__FILE__, __LINE__, std::string("counter = " + std::to_string(counter)));
            m_log->LogMe(__FILE__, __LINE__, std::string("m_frameTotalPoints = " + std::to_string(m_frameTotalPoints)));
            ++counter;

            for (auto& view : m_Views)
            {
                view->UpdateScore(m_frameTotalPoints, -(m_framePoints.size() - counter + 1));
            }
        }

        m_lastFlags.remove(Flag::STRIKE);
        m_framePoints.clear();
    }
    else if (shift == 0 && m_lastFlags.back() == Flag::STRIKE && m_lastFlags.size() == 3)
    {
        m_frameTotalPoints += 20;

        for (auto& val: m_framePoints)
        {
            val += 20;
        }

        for (auto& view : m_Views)
        {
            view->UpdateScore(m_framePoints.front(), -(m_lastFlags.size() - 1));
        }

        m_lastFlags.pop_front();
        m_framePoints.erase(m_framePoints.begin());
    }

    m_log->LogMe(__FILE__, __LINE__, std::string("m_frameTotalPoints = ") +
                                    std::to_string(m_frameTotalPoints) +
                                    std::string("  shift = ") + 
                                    std::to_string(shift));

    for (auto& view : m_Views)
    {
        view->UpdateScore(m_frameTotalPoints, shift);

        if (shift == 0)
        {
            view->SetNextFrameActive(true);
        }
    }
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
    points = listener->Receive();
    listener->Shutdown();
    return points;
}
