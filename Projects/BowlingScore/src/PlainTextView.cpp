#include "PlainTextView.h"

#include <iostream>
#include <stdexcept>

void PlainTextView::Draw(const ViewElement& element, void* params)
{
    switch (element)
    {
        case ViewElement::FRAME:
        {
            break;
        }
        case ViewElement::PLAYER:
        {
            std::string* name = static_cast<std::string*>(params);
            if (name != nullptr)
            {
                m_players.push_back(*name);
            }
            break;
        }
        case ViewElement::FLUSH:
        {
            std::cout << "Starting bowling game";
            if (!m_players.empty())
            {
                std::cout << " for ";
                for (size_t i = 0; i < m_players.size(); ++i)
                {
                    if (i > 0)
                    {
                        std::cout << ", ";
                    }
                    std::cout << m_players.at(i);
                }
            }
            std::cout << std::endl;
            break;
        }
        default:
        {
            throw std::runtime_error("Unknown type of view element");
        }
    }
}

void PlainTextView::UpdateFrameScore(const unsigned short score, const Flag& flag)
{
    std::cout
        << "[frame " << (m_activeFrameIdx + 1)
        << " throw " << (m_activeThrowIdx + 1)
        << "] " << getActivePlayerName()
        << " rolled " << score
        << " (" << flagToString(flag) << ")"
        << std::endl;
}

void PlainTextView::UpdateScore(const unsigned short score, const short shift, const bool gameover)
{
    std::cout
        << "[score] " << getActivePlayerName()
        << " total: " << score;

    if (shift != 0)
    {
        std::cout << " (bonus update)";
    }

    if (gameover)
    {
        std::cout << " [game over]";
    }

    std::cout << std::endl;
}

void PlainTextView::CleanScore()
{
    std::cout << "Game finished. Clearing board." << std::endl;
    m_players.clear();
    m_activePlayerIdx = 0;
    m_activeFrameIdx = 0;
    m_activeThrowIdx = 0;
}

void PlainTextView::SetNextFrameActive(const bool last)
{
    if (m_players.empty())
    {
        return;
    }

    if (!last)
    {
        ++m_activeThrowIdx;
        return;
    }

    m_activeThrowIdx = 0;

    if (m_activePlayerIdx < static_cast<short>(m_players.size()) - 1)
    {
        ++m_activePlayerIdx;
    }
    else
    {
        m_activePlayerIdx = 0;
        ++m_activeFrameIdx;
    }
}

const std::string& PlainTextView::getActivePlayerName() const
{
    static const std::string unknownPlayer("Unknown player");
    if (m_players.empty())
    {
        return unknownPlayer;
    }

    return m_players.at(m_activePlayerIdx);
}

const char* PlainTextView::flagToString(const Flag& flag)
{
    switch (flag)
    {
        case Flag::ZERO:
            return "zero";
        case Flag::NOTHING:
            return "open";
        case Flag::SPARE:
            return "spare";
        case Flag::STRIKE:
            return "strike";
        default:
            return "unknown";
    }
}
