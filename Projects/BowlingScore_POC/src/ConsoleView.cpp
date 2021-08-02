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

    m_cellWidth = (m_RowWidth - m_nameWidth - 1) / m_columnAmount;

    m_log->LogMe(__FILE__, __LINE__, std::string("Cell width = ") + std::to_string(m_cellWidth));

    if (m_cellWidth < 3)
    {
        throw std::runtime_error("The screen is narrower then the min width is supported");
    }
#elif _WIN32
#endif
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

            m_ActiveFrame = m_wGames.front().front().front();
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

    m_wPlayers.emplace_back(std::make_pair(displayName, newwin(m_cellWidth * 2 - 1, m_nameWidth + 2, m_wPlayers.size() * (m_cellWidth * 2 - 1), 0)));
    WINDOW* w = m_wPlayers.back().second;

    if (m_wPlayers.size() == 1)
    {
        wattron(w, A_BOLD); // First user mark as active. He will start the 1st
    }

    wborder(w, 0, 0, 0, 0, 0, 0, 0, 0);
    PrintPlayerName(w, displayName);
}

void ConsoleView::DrawFrameScore(bool isLastFrame)
{
    std::vector<std::string> m_FrameLines;

    size_t playerIdx = m_wPlayers.size();
    size_t gameIdx = m_Game.size();

    if (!isLastFrame)
    {
        FRAMES fr = {
            //                                 nlines,      ncols,          begin_y,                                            begin_x 
            std::make_pair(playerIdx, newwin(m_cellWidth, m_cellWidth,  playerIdx * (m_cellWidth * 2 - 1),                  m_nameWidth + 1 + gameIdx * m_cellWidth * 2)),
            std::make_pair(playerIdx, newwin(m_cellWidth, m_cellWidth,      playerIdx * (m_cellWidth * 2 - 1),                  m_nameWidth + m_cellWidth + 1 + gameIdx * m_cellWidth * 2)),
            std::make_pair(playerIdx, newwin(m_cellWidth, m_cellWidth * 2,  playerIdx * (m_cellWidth * 2 - 1) + m_cellWidth - 1,m_nameWidth + 1 + gameIdx * m_cellWidth * 2))
        };

        wborder(fr.at(0).second, 0, ' ', 0, ' ', 0, ' ', ' ', ' ');
        wborder(fr.at(1).second, 0, 0, 0, 0, 0, 0, 0, 0);
        wborder(fr.at(2).second, 0, 0, ' ', 0, '|', '|', 0, 0);

        m_Game.push_back(std::move(fr));
    }
    else
    {
        FRAMES fr = {
            std::make_pair(playerIdx, newwin(m_cellWidth, m_cellWidth,  playerIdx * (m_cellWidth * 2 - 1),                m_nameWidth + 1 + gameIdx * m_cellWidth * 2)),
            std::make_pair(playerIdx, newwin(m_cellWidth, m_cellWidth,  playerIdx * (m_cellWidth * 2 - 1),                m_nameWidth + m_cellWidth + 1 + gameIdx * m_cellWidth * 2)),
            std::make_pair(playerIdx, newwin(m_cellWidth, m_cellWidth,      playerIdx * (m_cellWidth * 2 - 1),                m_nameWidth + 2 * m_cellWidth + 1 + gameIdx * m_cellWidth * 2)),
            std::make_pair(playerIdx, newwin(m_cellWidth, m_cellWidth * 3,  playerIdx * (m_cellWidth * 2 - 1) + m_cellWidth - 1,m_nameWidth + 1 + gameIdx * m_cellWidth * 2))
        };

        wborder(fr.at(0).second, 0, ' ', 0, ' ', 0, ' ', ' ', ' ');
        wborder(fr.at(1).second, ' ', ' ', 0, ' ', ' ', ' ', ' ', ' ');
        wborder(fr.at(2).second, 0, 0, 0, 0, 0, 0, 0, 0);
        wborder(fr.at(3).second, 0, 0, ' ', 0, '|', '|', 0, 0);

        m_Game.push_back(std::move(fr));
    }
}

void ConsoleView::UpdateFrameScore(unsigned short score)
{
    m_log->LogMe(__FILE__, __LINE__, std::string("Update frame score with value = ") + std::to_string(score));
    mvwprintw(m_ActiveFrame.second, m_cellWidth / 2, m_cellWidth / 2, std::to_string(score).c_str());
    wrefresh(m_ActiveFrame.second);
    usleep(2000000);
}

void ConsoleView::UpdateScore(unsigned short score)
{
    std::string sscore = std::to_string(score);
    m_log->LogMe(__FILE__, __LINE__, std::string("Update score with value = ") + std::to_string(score));
    auto frame = m_wGames.at(m_ActivePlayerIdx).at(m_ActiveFramesIdx).at(m_ActiveFrameIdx);
    mvwprintw(frame.second, m_cellWidth / 2, m_cellWidth + sscore.length() / 2 - 1, std::to_string(score).c_str());
    wrefresh(frame.second);
    usleep(2000000);
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
    PrintPlayerName(w, displayName);

    if (m_ActivePlayerIdx < m_wPlayers.size() - 1)
    {
        ++m_ActivePlayerIdx;
    }
    else
    {
        m_ActivePlayerIdx = 0;
        ++m_ActiveFramesIdx;
    }

    w = m_wPlayers.at(m_ActivePlayerIdx).second;
    displayName = m_wPlayers.at(m_ActivePlayerIdx).first;

    m_log->LogMe(__FILE__, __LINE__, std::string("Activate next player with name = ") + displayName);

    wattron(w, A_BOLD);
    PrintPlayerName(w, displayName);
    usleep(2000000);
}

void ConsoleView::SetNextFrameActive(bool last)
{
    if (!last)
    {
        ++m_ActiveFrameIdx;
    }
    else
    {
        SetNextPlayerActive();
        m_ActiveFrameIdx = 0;
    }

    m_log->LogMe(__FILE__, __LINE__, std::string("Activate next frame with active player idx = ") + std::to_string(m_ActivePlayerIdx));

    m_ActiveFrame = m_wGames.at(m_ActivePlayerIdx).at(m_ActiveFramesIdx).at(m_ActiveFrameIdx);
}

void ConsoleView::PrintPlayerName(WINDOW* w, const std::string& name)
{
    mvwprintw(w, m_cellWidth - 1, 1, name.c_str());
    wrefresh(w);
}
