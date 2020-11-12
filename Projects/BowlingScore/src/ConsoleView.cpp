#include "ConsoleView.h"
#include <stdexcept>

#include <iostream>
#include <iomanip>

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
#elif _WIN32
#endif
}

ConsoleView::~ConsoleView()
{
    //
}

void ConsoleView::InitScoreTableFrame(const std::vector<std::string>& players)
{
    std::string line;
    size_t cellWidth = (m_RowWidth - m_nameWidth - 1) / m_columnAmount;

    line.append(1, '|');
    line.append(cellWidth * m_columnAmount + m_nameWidth + 1, '-');
    line.append(1, '|');
    line.append(1, '\n');

    m_ScoreTable.append(line);
}

void ConsoleView::InitPlayerScore(const std::string& name)
{
    std::string displayName((name.length() <= this->m_nameWidth ? name : (name.substr(0, this->m_nameWidth - 3) + std::string("..."))));

    auto drawAboveName = [this]
    {
        std::string line;
        line.append(1, '|');
        line.append(this->m_nameWidth + 1, ' ');
        line.append(1, '|');

        size_t cellWidth = (this->m_RowWidth - this->m_nameWidth - 1) / this->m_columnAmount;
        std::string toFindFirst = std::string(cellWidth - 1, ' ') + std::string(1, '|') + std::string(cellWidth - 1, ' ') + std::string(1, '|');
        std::string toFindLast = std::string("\n");
        size_t start = this->m_PlayerGame.find(toFindFirst);
        size_t end = this->m_PlayerGame.find(toFindLast, start);
        std::string aboveNameGame = this->m_PlayerGame.substr(start, end + 1);
        line.append(aboveNameGame);

        return std::move(line);
    };

    auto drawName = [this, &displayName]
    {
        std::string line;
        line.append(1, '|');
        line.append(displayName);
        line.append(this->m_nameWidth - displayName.length() + 1, ' ');
        line.append(1, '|');

        size_t cellWidth = (this->m_RowWidth - this->m_nameWidth - 1) / this->m_columnAmount;
        std::string toFindFirst = std::string(cellWidth - 1, ' ') + std::string(1, '|') + std::string(cellWidth - 1, '-') + std::string(1, '|');
        std::string toFindLast = std::string(cellWidth - 1, '-') + std::string(1, '|');
        size_t start = this->m_PlayerGame.find(toFindFirst);
        size_t end = this->m_PlayerGame.find(toFindLast, start);
        std::string nameGame = this->m_PlayerGame.substr(start, end - toFindLast.length());
        line.append(nameGame);

        return line;
    };

    auto drawBelowName = [this]
    {
        std::string line;
        line.append(1, '|');
        line.append(this->m_nameWidth + 1, ' ');
        line.append(1, '|');

        size_t cellWidth = (this->m_RowWidth - this->m_nameWidth - 1) / this->m_columnAmount;
        std::string toFindFirst = std::string(m_normalFrameCellsAmount * cellWidth - 1, ' ') + std::string(1, '|');
        std::string toFindLast = std::string("\n");
        size_t start = this->m_PlayerGame.find(toFindFirst);
        size_t end = this->m_PlayerGame.find(toFindLast, start);
        std::string belowNameGame = this->m_PlayerGame.substr(start, end + 1);
        line.append(belowNameGame);

        return line;
    };

    auto drawFooterLine = [this]
    {
        std::string line;
        size_t cellWidth = (this->m_RowWidth - this->m_nameWidth - 1) / this->m_columnAmount;

        line.append(1, '|');
        line.append(cellWidth * this->m_columnAmount + this->m_nameWidth + 1, '-');
        line.append(1, '|');
        line.append(1, '\n');

        return line;
    };

    m_ScoreTable.append(drawAboveName() + drawName() + drawBelowName() + drawFooterLine());
}

void ConsoleView::InitGameScore()
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

void ConsoleView::InitFrameScore(bool isLastFrame)
{
    std::vector<std::string> m_FrameLines;
    auto drawAboveName = [this, &isLastFrame]
    { 
        std::string line;
        size_t cellWidth = (this->m_RowWidth - this->m_nameWidth - 1) / this->m_columnAmount;

        if (isLastFrame)
        {
            for (size_t i = 0; i < m_lastFrameCellsAmount; ++i)
            {
                line.append(cellWidth - 1, ' ');
                line.append(1, '|');
            }
        }
        else
        {
            line.append(cellWidth - 1, ' ');
            line.append(1, '|');
            line.append(cellWidth - 1, ' ');
            line.append(1, '|');
        }

        return std::move(line);
    };

    auto drawName = [this, &isLastFrame]
    {
        std::string line;
        size_t cellWidth = (this->m_RowWidth - this->m_nameWidth - 1) / 21;

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

    auto drawBelowName = [this, &isLastFrame]
    {
        std::string line;
        size_t cellWidth = (this->m_RowWidth - this->m_nameWidth - 1) / this->m_columnAmount;

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

void ConsoleView::InitFlush() 
{
    std::cout << m_ScoreTable << std::flush;
}

void ConsoleView::UpdateScore()
{
    throw std::runtime_error("Unimplemented functionality");
}

void ConsoleView::CleanScore()
{
    m_ScoreTable.clear();
    m_Frames.clear();
    m_PlayerGame.clear();
}
