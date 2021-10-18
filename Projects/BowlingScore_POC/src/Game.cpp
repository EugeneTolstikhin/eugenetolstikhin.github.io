#include "Game.h"
#include "Frame.h"
#include "LoggerFactory.h"
#include "FileLogger.h"

#include <numeric>
#include <algorithm>

Game::Game(IView* view, GetPointsFunction getPoints) :
    m_getPoints(getPoints)
    ,m_loggerFactory(new LoggerFactory)
    ,m_log(m_loggerFactory->CreateLogger(m_typeLogger))
    ,m_view(view)
{
    size_t counter = 0;
    while (++counter <= MAX_FRAME_AMOUNT)
    {
        m_Frames.emplace_back(new Frame(counter == MAX_FRAME_AMOUNT, m_view));
    }

    m_currFrame = std::make_pair(m_Frames.front(), m_Frames.begin());
}

Game::~Game()
{
    //
}

void Game::ThrowBall()
{
    auto points = m_getPoints();

    m_gameOver = m_lastFrameCounter == 0;
    Flag& flag = m_currFrame.first->SetTrialPoints(points);
    
    if (!m_lastFlags.empty())
    {
        if (m_lastFlags.front() == Flag::NOTHING || m_lastFlags.front() == Flag::ZERO)
        {
            m_lastFlags.pop_front();
        }

        switch (flag)
        {
            case Flag::ZERO:
            case Flag::NOTHING:
            {
                if (    m_currFrame.first->isLastFrame()
                    &&  m_lastFlags.size() == 0)
                {
                    m_gameOver = true;
                }

                if (m_lastFlags.size() == 2)
                {
                    if (    m_lastFlags.back() == Flag::STRIKE
                        &&  !(
                                m_currFrame.first->isLastFrame() 
                            ||  m_gameOver))
                    {
                        UpdateTotalScore(-2);
                        m_lastFlags.pop_front();
                    }
                    else if (m_lastFlags.front() == Flag::STRIKE)
                    {
                        if (m_currFrame.first->isLastFrame())
                        {
                            UpdateTotalScore(m_lastFrameCounter);
                            m_lastFlags.pop_front();

                            if (m_lastFlags.front() == Flag::NOTHING || m_lastFlags.front() == Flag::ZERO)
                            {
                                UpdateTotalScore(0);
                                m_lastFlags.pop_front();
                            }
                            else
                            {
                                m_lastFlags.pop_front();
                            }
                        }
                        else
                        {
                            UpdateTotalScore(-1);
                            m_lastFlags.pop_front();
                        }
                    }
                    else
                    {
                        //
                    }
                }

                if (    m_lastFlags.size() == 1
                    &&  m_lastFlags.front() == Flag::SPARE
                    &&  (!m_currFrame.first->isLastFrame()
                            || (m_currFrame.first->isLastFrame()
                                    && m_lastFrameCounter < -1))
                    )
                {
                    UpdateTotalScore(-1);
                    m_lastFlags.pop_front();
                }

                break;
            }
            case Flag::STRIKE:
            {
                if (!m_currFrame.first->isLastFrame())
                {
                    switch (m_lastFlags.size())
                    {
                        case 2:
                        {
                            UpdateTotalScore(-2);
                            m_lastFlags.pop_front();
                            break;
                        }

                        case 1:
                        {
                            if (m_lastFlags.front() == Flag::SPARE)
                            {
                                UpdateTotalScore(-1);
                                m_lastFlags.pop_front();
                            }
                            break;
                        }
                    }
                }
                else
                {
                    if (m_lastFlags.size() > 0)
                    {
                        switch (m_lastFlags.size())
                        {
                            case 2:
                            {
                                UpdateTotalScore(m_lastFrameCounter);
                                m_lastFlags.pop_front();
                                break;
                            }

                            case 1:
                            {
                                 if (m_lastFrameCounter == -2
                                 && m_lastFlags.front() == Flag::SPARE)
                                {
                                    UpdateTotalScore(-1);
                                }

                                if (m_lastFlags.front() == Flag::SPARE)
                                {
                                    m_lastFlags.pop_front();
                                }

                                break;
                            }
                        }
                    }
                }

                break;
            }
            case Flag::SPARE:
            {
                if (m_lastFlags.size() >= 2)
                {
                    UpdateTotalScore(-1);
                    m_lastFlags.pop_front();
                    m_lastFlags.pop_front();
                }

                break;
            }
            default:
                break;
        }
    }

    m_lastFlags.push_back(flag);

    if (m_currFrame.first->isLastFrame() && m_lastFrameCounter < 0)
    {
        ++m_lastFrameCounter;
    }
}

void Game::UpdateTotalScore(const short shift)
{
    m_frameTotalPoints += m_currFrame.first->GetTotalFramePoints();

    if (m_view != nullptr)
    {
        m_view->UpdateScore(m_frameTotalPoints, shift, m_gameOver);

        if (shift == 0)
        {
            m_view->SetNextFrameActive(true);
        }
    }
    else
    {
        m_log->LogMe(__FILE__, __LINE__, "view is UNAVAILABLE");
    }

    if (shift == -2)
    {
        m_frameTotalPoints += MAX_POINTS;
    }       
}

bool Game::IsAnotherThrowAllowed() const noexcept
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
