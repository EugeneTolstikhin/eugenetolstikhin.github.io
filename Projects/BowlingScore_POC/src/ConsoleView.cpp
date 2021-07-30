#include "ConsoleView.h"
#include "LoggerFactory.h"
#include "FileLogger.h"
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <regex>

#ifdef __linux__
#include <sys/ioctl.h> //ioctl() and TIOCGWINSZ
#include <unistd.h> // for STDOUT_FILENO
#elif _WIN32 
#endif

ConsoleView::ConsoleView() :
    m_loggerFactory(new LoggerFactory)
    ,m_log(m_loggerFactory->CreateLogger(LoggerType::TO_FILE))
{
    initscr();
    refresh();
    curs_set(0);
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
    m_wGames.reserve(m_normalFramesAmount + 1);

    size_t cellWidth = (m_RowWidth - m_nameWidth - 1) / m_columnAmount;
    if (cellWidth < 2)
    {
        throw std::runtime_error("The screen is narrower then the min width is supported");
    }
#elif _WIN32
#endif

    //mvwprintw(w0, 2, 1, "Gene");
    //mvwprintw(w1, 1, 1, "1");
    //mvwprintw(w2, 1, 1, "X");
    //mvwprintw(w3, 1, 1, "300");
}

ConsoleView::~ConsoleView()
{
    for (auto& w: m_wPlayers)
    {
        delwin(w.second);
    }

    for (auto& game: m_wGames)
    {
        for (auto& frames: game)
        {
            for (auto& frame: frames)
            {
                delwin(frame.second);
            }
        }
    }

    endwin();
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

        case ViewElement::PLAYER:
        {
            std::string* name = static_cast<std::string*>(params);
            DrawPlayerScore(*name);

            if (!m_wPlayers.empty())
            {
                m_wGames.push_back(std::move(m_Game));
            }

            break;
        }

        case ViewElement::FLUSH:
        {
            for (auto& game: m_wGames)
            {
                for (auto& frames: game)
                {
                    /*m_log->LogMe(__FILE__, __LINE__, std::string("Update frames ") +
                                                     std::to_string(idx) +
                                                     std::string(" for the player with idx = ") +
                                                     std::to_string(frames.at(0).first) +
                                                     std::string(" His name is ") +
                                                     m_wPlayers.at(frames.at(0).first).first);*/

                    wrefresh(frames.at(0).second);

                    if (frames.size() == 3)
                    {
                        wrefresh(frames.at(2).second);
                        wrefresh(frames.at(1).second);
                    }
                    else if (frames.size() == 4)
                    {
                        wrefresh(frames.at(1).second);
                        wrefresh(frames.at(3).second);
                        wrefresh(frames.at(2).second);
                    }
                }
            }

            for (auto w: m_wPlayers)
            {
                wrefresh(w.second);
            }
            break;
        }

        default:
        {
            throw std::runtime_error("Unknown type of view element");
        }
    }
}

void ConsoleView::DrawPlayerScore(const std::string& name)
{
    std::string displayName((name.length() <= m_nameWidth ? name : (name.substr(0, m_nameWidth - 3) + std::string("..."))));
    size_t cellWidth = (m_RowWidth - m_nameWidth - 1) / m_columnAmount;

    cellWidth = 3; // TMP

    m_wPlayers.emplace_back(std::make_pair(displayName, newwin(cellWidth * 2 - 1, m_nameWidth + 2, m_wPlayers.size() * (cellWidth * 2 - 1), 0)));
    WINDOW* w = m_wPlayers.back().second;

    if (m_wPlayers.size() == 1)
    {
        wattron(w, A_BOLD); // First user mark as active. He will start the 1st
    }

    wborder(w, 0, 0, 0, 0, 0, 0, 0, 0);
    mvwprintw(w, 2, 1, displayName.c_str());
    wattroff(w, A_BOLD);
}

void ConsoleView::DrawFrameScore(bool isLastFrame, char sign)
{
    std::vector<std::string> m_FrameLines;
    size_t cellWidth = (m_RowWidth - m_nameWidth - 1) / m_columnAmount;

    cellWidth = 3; // TMP

    size_t playerIdx = m_wPlayers.size();
    size_t gameIdx = m_Game.size();

    if (!isLastFrame)
    {
        FRAMES fr = {
            //                                 nlines,  ncols,          begin_y,                                        begin_x 
            std::make_pair(playerIdx, newwin(cellWidth, cellWidth + 1,  playerIdx * (cellWidth * 2 - 1),                m_nameWidth + 1 + gameIdx * cellWidth * 2)),
            std::make_pair(playerIdx, newwin(cellWidth, cellWidth,      playerIdx * (cellWidth * 2 - 1),                m_nameWidth + cellWidth + 1 + gameIdx * cellWidth * 2)),
            std::make_pair(playerIdx, newwin(cellWidth, cellWidth * 2,  playerIdx * (cellWidth * 2 - 1) + cellWidth - 1,m_nameWidth + 1 + gameIdx * cellWidth * 2))
        };

        wborder(fr.at(0).second, 0, ' ', 0, ' ', 0, 0, ' ', ' ');
        wborder(fr.at(1).second, 0, 0, 0, 0, 0, 0, 0, 0);
        wborder(fr.at(2).second, 0, 0, ' ', 0, '|', '|', 0, 0);

        m_Game.push_back(std::move(fr));
    }
    else
    {
        FRAMES fr = {
            std::make_pair(playerIdx, newwin(cellWidth, cellWidth + 1,  playerIdx * (cellWidth * 2 - 1),                m_nameWidth + 1 + gameIdx * cellWidth * 2)),
            std::make_pair(playerIdx, newwin(cellWidth, cellWidth + 1,  playerIdx * (cellWidth * 2 - 1),                m_nameWidth + cellWidth + 1 + gameIdx * cellWidth * 2)),
            std::make_pair(playerIdx, newwin(cellWidth, cellWidth,      playerIdx * (cellWidth * 2 - 1),                m_nameWidth + 2 * cellWidth + 1 + gameIdx * cellWidth * 2)),
            std::make_pair(playerIdx, newwin(cellWidth, cellWidth * 3,  playerIdx * (cellWidth * 2 - 1) + cellWidth - 1,m_nameWidth + 1 + gameIdx * cellWidth * 2))
        };

        wborder(fr.at(0).second, 0, ' ', 0, ' ', 0, 0, ' ', ' ');
        wborder(fr.at(1).second, ' ', ' ', 0, ' ', ' ', ' ', ' ', ' ');
        wborder(fr.at(2).second, 0, 0, 0, 0, 0, 0, 0, 0);
        wborder(fr.at(3).second, 0, 0, ' ', 0, '|', '|', 0, 0);

        m_Game.push_back(std::move(fr));
    }
}

void ConsoleView::UpdateScore(unsigned short score)
{
    //throw std::runtime_error("Unimplemented functionality");
}

void ConsoleView::CleanScore()
{
    //std::cout << "Game over!" << std::endl << std::flush;
    //m_ScoreTable.clear();
    //m_Frames.clear();
    //m_PlayerGame.clear();

    refresh();
    getch();
}

void ConsoleView::SetNextPlayerActive()
{
    WINDOW* w = m_wPlayers.at(m_ActivePlayerIdx).second;
    auto displayName = m_wPlayers.at(m_ActivePlayerIdx).first;
    wattroff(w, A_BOLD); // Should be already OFF, but just in case mentioned OFF here
    mvwprintw(w, 2, 1, displayName.c_str());
    wrefresh(w);

    if (m_ActivePlayerIdx < m_wPlayers.size() - 1)
    {
        ++m_ActivePlayerIdx;
    }
    else
    {
        m_ActivePlayerIdx = 0;
    }

    w = m_wPlayers.at(m_ActivePlayerIdx).second;
    displayName = m_wPlayers.at(m_ActivePlayerIdx).first;

    wattron(w, A_BOLD);
    mvwprintw(w, 2, 1, displayName.c_str());
    wattroff(w, A_BOLD);
    wrefresh(w);
    usleep(5000000);
}

void ConsoleView::SetNextFrameActive()
{
    //
}
