#include "ConsoleView.h"
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <regex>

#ifdef __linux__
#include <sys/ioctl.h> //ioctl() and TIOCGWINSZ
#include <unistd.h> // for STDOUT_FILENO
#elif _WIN32 
#endif

ConsoleView::ConsoleView()
{
#ifdef __linux__
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

    //The following parameters can be read from a config files
    m_RowWidth = size.ws_col;
    m_normalFrameCellsAmount = 2;
    m_normalFramesAmount = 9;
    m_lastFrameCellsAmount = 3;
    m_columnAmount = m_normalFrameCellsAmount * m_normalFramesAmount + m_lastFrameCellsAmount;
    m_nameWidth = m_RowWidth / m_columnAmount;
    m_Frames.reserve(m_normalFramesAmount + 1);

    size_t cellWidth = (m_RowWidth - m_nameWidth - 1) / m_columnAmount;
    if (cellWidth < 2)
    {
        throw std::runtime_error("The screen is narrower then the min width is supported");
    }
#elif _WIN32
#endif
}

ConsoleView::~ConsoleView()
{
    //
}

void ConsoleView::Draw(const ViewElement& element, void* params)
{
    switch (element)
    {
        case ViewElement::FRAME:
        {
            bool* isLastFrame = static_cast<bool*>(params);
            DrawFrameScore(*isLastFrame);
            break;
        }

        case ViewElement::GAME:
        {
            DrawGameScore();
            break;
        }

        case ViewElement::PLAYER:
        {
            std::string* name = static_cast<std::string*>(params);
            DrawPlayerScore(*name);
            break;
        }

        case ViewElement::LANE:
        {
            DrawLaneScore();
            break;
        }

        case ViewElement::FLUSH:
        {
            std::cout << m_ScoreTable << std::flush;
            break;
        }

        default:
        {
            throw std::runtime_error("Unknown type of view element");
        }
    }
}

void ConsoleView::DrawLaneScore()
{
    std::string line;
    size_t cellWidth = (m_RowWidth - m_nameWidth - 1) / m_columnAmount;

    line.append(1, '|');
    line.append(cellWidth * m_columnAmount + m_nameWidth + 1, '-');
    line.append(1, '|');
    line.append(1, '\n');

    m_ScoreTable.append(line);
}

void ConsoleView::DrawPlayerScore(const std::string& name)
{
    std::string displayName((name.length() <= m_nameWidth ? name : (name.substr(0, m_nameWidth - 3) + std::string("..."))));
    size_t cellWidth = (m_RowWidth - m_nameWidth - 1) / m_columnAmount;

    auto drawAboveName = [this, &cellWidth]
    {
        std::string line;
        line.append(1, '|');
        line.append(m_nameWidth + 1, ' ');
        line.append(1, '|');

        size_t signPos = (cellWidth - 1) / 2;
        bool isWidthOdd = (cellWidth - 1) % 2 == 0;
        std::string pattern;
        for (size_t i = 0; i < 2; ++i)
        {
            pattern += std::string("[\\s]{")
                    + std::to_string(signPos - (isWidthOdd ? 1 : 0))
                    + std::string("}[\\s0-9X\\/-]{1}[\\s]{")
                    + std::to_string(signPos)
                    + std::string("}[|]{1}");
        }

        std::regex regexp(pattern);
        std::smatch m;
        
        if (!std::regex_search(m_PlayerGame, m, regexp))
        { 
            throw std::runtime_error("Cannot find the string");
        }

        std::string toFindFirst = *m.begin();
        std::string toFindLast = std::string("\n");
        size_t start = m_PlayerGame.find(toFindFirst);
        size_t end = m_PlayerGame.find(toFindLast, start);
        std::string aboveNameGame = m_PlayerGame.substr(start, end + 1);

        line.append(aboveNameGame);

        return std::move(line);
    };

    auto drawName = [this, &displayName, &cellWidth]
    {
        std::string line;
        line.append(1, '|');
        line.append(displayName);
        line.append(m_nameWidth - displayName.length() + 1, ' ');
        line.append(1, '|');

        std::string toFindFirst = std::string(cellWidth - 1, ' ') + std::string(1, '|') + std::string(cellWidth - 1, '-') + std::string(1, '|');
        std::string toFindLast = std::string(cellWidth - 1, '-') + std::string(1, '|');
        size_t start = m_PlayerGame.find(toFindFirst);
        size_t end = m_PlayerGame.find(toFindLast, start);
        std::string nameGame = m_PlayerGame.substr(start, end - toFindLast.length());
        line.append(nameGame);

        return line;
    };

    auto drawBelowName = [this, &cellWidth]
    {
        std::string line;
        line.append(1, '|');
        line.append(m_nameWidth + 1, ' ');
        line.append(1, '|');

        size_t cellWidth = (m_RowWidth - m_nameWidth - 1) / m_columnAmount;
        std::string toFindFirst = std::string(m_normalFrameCellsAmount * cellWidth - 1, ' ') + std::string(1, '|');
        std::string toFindLast = std::string("\n");
        size_t start = m_PlayerGame.find(toFindFirst);
        size_t end = m_PlayerGame.find(toFindLast, start);
        std::string belowNameGame = m_PlayerGame.substr(start, end + 1);
        line.append(belowNameGame);

        return line;
    };

    auto drawFooterLine = [this, &cellWidth]
    {
        std::string line;
        line.append(1, '|');
        line.append(cellWidth * m_columnAmount + m_nameWidth + 1, '-');
        line.append(1, '|');
        line.append(1, '\n');

        return line;
    };

    m_ScoreTable.append(drawAboveName() + drawName() + drawBelowName() + drawFooterLine());
}

void ConsoleView::DrawGameScore()
{
    m_PlayerGame = "";
    std::string picture;

    std::vector<std::string> lines(m_Frames.at(0).size());
    for (const auto& frame: m_Frames)
    {
        for (size_t i = 0; i < frame.size(); ++i)
        {
            lines.at(i) += frame.at(i);
        }
    }

    for (const auto& line: lines)
    {
        m_PlayerGame += line + std::string("\n");
    }

    m_Frames.clear();
}

void ConsoleView::DrawFrameScore(bool isLastFrame, char sign)
{
    std::vector<std::string> m_FrameLines;
    size_t cellWidth = (m_RowWidth - m_nameWidth - 1) / m_columnAmount;

    auto drawAboveName = [this, &isLastFrame, &cellWidth, &sign]
    { 
        std::string line;
        size_t signPos = (cellWidth - 1) / 2;
        bool isWidthOdd = (cellWidth - 1) % 2 == 0;

        if (isLastFrame)
        {
            for (size_t i = 0; i < m_lastFrameCellsAmount; ++i)
            {
                line.append(signPos - (isWidthOdd ? 1 : 0), ' ');
                line.append(1, sign);
                line.append(signPos, ' ');
                line.append(1, '|');
            }
        }
        else
        {
            for (size_t i = 0; i < 2; ++i)
            {
                line.append(signPos - (isWidthOdd ? 1 : 0), ' ');
                line.append(1, sign);
                line.append(signPos, ' ');
                line.append(1, '|');
            }
        }

        return std::move(line);
    };

    auto drawName = [this, &isLastFrame, &cellWidth]
    {
        std::string line;

        if (isLastFrame)
        {
            for (size_t i = 0; i < m_lastFrameCellsAmount; ++i)
            {
                line.append(cellWidth - 1, '-');
                line.append(1, '|');
            }
        }
        else
        {
            line.append(cellWidth - 1, ' ');
            line.append(1, '|');
            line.append(cellWidth - 1, '-');
            line.append(1, '|');
        }

        return std::move(line);
    };

    auto drawBelowName = [this, &isLastFrame, &cellWidth]
    {
        std::string line;

        if (isLastFrame)
        {
            line.append(m_lastFrameCellsAmount * cellWidth - 1, ' ');
            line.append(1, '|');
        }
        else
        {
            line.append(m_normalFrameCellsAmount * cellWidth - 1, ' ');
            line.append(1, '|');
        }

        return std::move(line);
    };

    m_FrameLines.push_back(drawAboveName());
    m_FrameLines.push_back(drawName());
    m_FrameLines.push_back(drawBelowName());

    m_Frames.push_back(std::move(m_FrameLines));
}

void ConsoleView::UpdateScore()
{
    throw std::runtime_error("Unimplemented functionality");
}

void ConsoleView::CleanScore()
{
    std::cout << "Game over!" << std::endl << std::flush;
    m_ScoreTable.clear();
    m_Frames.clear();
    m_PlayerGame.clear();
}
