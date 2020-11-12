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
#elif _WIN32
#endif
}

ConsoleView::~ConsoleView()
{
    //
}

void ConsoleView::InitScoreTableFrame(const std::vector<std::string>& players)
{
    std::string picture;
    size_t cellWidth = (m_RowWidth - m_nameWidth - 1) / m_columnAmount;

    picture.append(1, '|');
    picture.append(cellWidth * m_columnAmount + m_nameWidth + 1, '-');
    picture.append(1, '|');
    picture.append(1, '\n');

    std::cout << picture;
}

void ConsoleView::InitPlayerScore(const std::string& name)
{
    std::string displayName((name.length() <= this->m_nameWidth ? name : (name.substr(0, this->m_nameWidth - 3) + std::string("..."))));

    std::string picture;

    auto drawAboveName = [this, &picture]{
        picture.append(1, '|');
        picture.append(this->m_nameWidth + 1, ' ');
        picture.append(1, '|');

        size_t cellWidth = (this->m_RowWidth - this->m_nameWidth - 1) / this->m_columnAmount;
        std::string toFindFirst = std::string(cellWidth - 1, ' ') + std::string(1, '|') + std::string(cellWidth - 1, ' ') + std::string(1, '|');
        std::string toFindLast = std::string("\n");
        size_t start = this->m_PlayerGame.find(toFindFirst);
        size_t end = this->m_PlayerGame.find(toFindLast, start);
        std::string aboveNameGame = this->m_PlayerGame.substr(start, end + 1);
        picture.append(aboveNameGame);
    };

    auto drawName = [this, &picture, &displayName]{
        picture.append(1, '|');
        picture.append(displayName);
        picture.append(this->m_nameWidth - displayName.length() + 1, ' ');
        picture.append(1, '|');

        size_t cellWidth = (this->m_RowWidth - this->m_nameWidth - 1) / this->m_columnAmount;
        std::string toFindFirst = std::string(cellWidth - 1, ' ') + std::string(1, '|') + std::string(cellWidth - 1, '-') + std::string(1, '|');
        std::string toFindLast = std::string(cellWidth - 1, '-') + std::string(1, '|');
        size_t start = this->m_PlayerGame.find(toFindFirst);
        size_t end = this->m_PlayerGame.find(toFindLast, start);
        std::string nameGame = this->m_PlayerGame.substr(start, end - toFindLast.length());
        picture.append(nameGame);
    };

    auto drawBelowName = [this, &picture]{
        picture.append(1, '|');
        picture.append(this->m_nameWidth + 1, ' ');
        picture.append(1, '|');

        size_t cellWidth = (this->m_RowWidth - this->m_nameWidth - 1) / this->m_columnAmount;
        std::string toFindFirst = std::string(m_normalFrameCellsAmount * cellWidth - 1, ' ') + std::string(1, '|');
        std::string toFindLast = std::string("\n");
        size_t start = this->m_PlayerGame.find(toFindFirst);
        size_t end = this->m_PlayerGame.find(toFindLast, start);
        std::string belowNameGame = this->m_PlayerGame.substr(start, end + 1);
        picture.append(belowNameGame);
    };

    auto drawFooterLine = [this, &picture]{
        size_t cellWidth = (this->m_RowWidth - this->m_nameWidth - 1) / this->m_columnAmount;

        picture.append(1, '|');
        picture.append(cellWidth * this->m_columnAmount + this->m_nameWidth + 1, '-');
        picture.append(1, '|');
        picture.append(1, '\n');
    };

    drawAboveName();
    drawName();
    drawBelowName();
    drawFooterLine();

    std::cout << picture;
}

void ConsoleView::InitGameScore()
{
    m_PlayerGame = "";

    std::string picture;

    auto drawAboveName = [this, &picture]{
        size_t cellWidth = (this->m_RowWidth - this->m_nameWidth - 1) / this->m_columnAmount;

        for (size_t i = 0; i < m_normalFramesAmount; ++i)
        {
            picture.append(cellWidth - 1, ' ');
            picture.append(1, '|');
            picture.append(cellWidth - 1, ' ');
            picture.append(1, '|');
        }

        for (size_t i = 0; i < m_lastFrameCellsAmount; ++i)
        {
            picture.append(cellWidth - 1, ' ');
            picture.append(1, '|');
        }

        picture.append(1, '\n');
    };

    auto drawName = [this, &picture]{
        size_t cellWidth = (this->m_RowWidth - this->m_nameWidth - 1) / 21;

        for (size_t i = 0; i < m_normalFramesAmount; ++i)
        {
            picture.append(cellWidth - 1, ' ');
            picture.append(1, '|');
            picture.append(cellWidth - 1, '-');
            picture.append(1, '|');
        }

        for (size_t i = 0; i < m_lastFrameCellsAmount; ++i)
        {
            picture.append(cellWidth - 1, '-');
            picture.append(1, '|');
        }

        picture.append(1, '\n');
    };

    auto drawBelowName = [this, &picture]{
        size_t cellWidth = (this->m_RowWidth - this->m_nameWidth - 1) / this->m_columnAmount;

        for (size_t i = 0; i < m_normalFramesAmount; ++i)
        {
            picture.append(m_normalFrameCellsAmount * cellWidth - 1, ' ');
            picture.append(1, '|');
        }

        picture.append(m_lastFrameCellsAmount * cellWidth - 1, ' ');
        picture.append(1, '|');
        picture.append(1, '\n');
    };

    drawAboveName();
    drawName();
    drawBelowName();

    m_PlayerGame = picture;
}

void ConsoleView::InitFlush() 
{
    std::cout << std::flush;
}

void ConsoleView::UpdateScore()
{
    throw std::runtime_error("Unimplemented functionality");
}

void ConsoleView::CleanScore()
{
    throw std::runtime_error("Unimplemented functionality");
}
