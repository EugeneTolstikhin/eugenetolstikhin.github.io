#include "Game.h"
#include "Frame.h"
#include "LoggerFactory.h"
#include "FileLogger.h"

#include <algorithm>
#include <numeric>
#include <stdexcept>

Game::Game(IView* view, GetPointsFunction getPoints) :
    m_getPoints(getPoints)
    ,m_loggerFactory(std::make_unique<LoggerFactory>())
    ,m_log(m_loggerFactory->CreateLogger(m_typeLogger))
    ,m_view(view)
{
    size_t counter = 0;
    while (++counter <= MAX_FRAME_AMOUNT)
    {
        m_Frames.emplace_back(new Frame(counter == MAX_FRAME_AMOUNT, m_view));
    }

    m_currFrame = std::make_pair(m_Frames.front(), m_Frames.begin());
    m_frameRolls.resize(MAX_FRAME_AMOUNT);
    m_reportedCumulativeScores.resize(MAX_FRAME_AMOUNT);
}

Game::~Game()
{
	//
}

void Game::ThrowBall()
{
    const auto points = m_getPoints();
    m_currFrame.first->SetTrialPoints(points);
    recordRoll(points);
}

void Game::UpdateTotalScore(const short)
{
    unsigned short cumulativeScore = 0;
    for (std::size_t frameIndex = 0; frameIndex <= m_currentFrameIndex && frameIndex < m_frameRolls.size(); ++frameIndex)
    {
        const auto frameScore = tryGetFrameScore(frameIndex);
        if (!frameScore.has_value())
        {
            break;
        }

        cumulativeScore = static_cast<unsigned short>(cumulativeScore + frameScore.value());

        auto& reportedScore = m_reportedCumulativeScores.at(frameIndex);
        if (reportedScore.has_value() && reportedScore.value() == cumulativeScore)
        {
            continue;
        }

        if (m_view != nullptr)
        {
            const auto shift = static_cast<short>(frameIndex) - static_cast<short>(m_currentFrameIndex);
            const bool gameover = frameIndex == m_currentFrameIndex
                && m_currentFrameIndex + 1 == MAX_FRAME_AMOUNT;
            m_view->UpdateScore(cumulativeScore, shift, gameover);
        }
        else
        {
            m_log->LogMe(__FILE__, __LINE__, "view is UNAVAILABLE");
        }

        reportedScore = cumulativeScore;
    }

    if (m_view != nullptr)
    {
        m_view->SetNextFrameActive(true);
    }
    else
    {
        m_log->LogMe(__FILE__, __LINE__, "view is UNAVAILABLE");
    }
}

bool Game::IsAnotherThrowAllowed() const noexcept
{
    return m_currFrame.first->isAllowedThrow();
}

void Game::CloseGame(std::function<void()> gameOver)
{
    auto nextFrame = m_currFrame.second;
    ++nextFrame;
    if (nextFrame == m_Frames.end())
    {
        gameOver();
    }
    else
    {
        m_currFrame.second = nextFrame;
        m_currFrame.first = *m_currFrame.second;
        ++m_currentFrameIndex;
    }
}

void Game::recordRoll(const unsigned short points)
{
    m_frameRolls.at(m_currentFrameIndex).push_back(points);
}

std::vector<unsigned short> Game::collectBonusRolls(const std::size_t frameIndex, const std::size_t amount) const
{
    std::vector<unsigned short> bonusRolls;
    for (std::size_t nextFrame = frameIndex + 1; nextFrame < m_frameRolls.size() && bonusRolls.size() < amount; ++nextFrame)
    {
        for (const auto roll : m_frameRolls.at(nextFrame))
        {
            bonusRolls.push_back(roll);
            if (bonusRolls.size() == amount)
            {
                break;
            }
        }
    }

    return bonusRolls;
}

std::optional<unsigned short> Game::tryGetFrameScore(const std::size_t frameIndex) const
{
    if (frameIndex >= m_frameRolls.size())
    {
        throw std::out_of_range("Frame index is out of range");
    }

    const auto& frameRolls = m_frameRolls.at(frameIndex);
    if (frameRolls.empty())
    {
        return std::nullopt;
    }

    if (frameIndex + 1 == MAX_FRAME_AMOUNT)
    {
        return static_cast<unsigned short>(std::accumulate(frameRolls.begin(), frameRolls.end(), 0));
    }

    if (frameRolls.front() == MAX_POINTS)
    {
        const auto bonusRolls = collectBonusRolls(frameIndex, 2);
        if (bonusRolls.size() < 2)
        {
            return std::nullopt;
        }

        return static_cast<unsigned short>(MAX_POINTS + bonusRolls.at(0) + bonusRolls.at(1));
    }

    if (frameRolls.size() < 2)
    {
        return std::nullopt;
    }

    const auto framePins = static_cast<unsigned short>(frameRolls.at(0) + frameRolls.at(1));
    if (framePins == MAX_POINTS)
    {
        const auto bonusRolls = collectBonusRolls(frameIndex, 1);
        if (bonusRolls.empty())
        {
            return std::nullopt;
        }

        return static_cast<unsigned short>(MAX_POINTS + bonusRolls.front());
    }

    return framePins;
}
