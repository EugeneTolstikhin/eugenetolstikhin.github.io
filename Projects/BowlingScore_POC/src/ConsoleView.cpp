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
    m_wFrames.reserve(m_normalFramesAmount + 1);

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
    for (auto w: m_wPlayers)
    {
        delwin(w);
    }

    for (auto w: m_wFrames)
    {
        delwin(w.at(0));
        delwin(w.at(1));
        delwin(w.at(2));
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
            break;
        }

        case ViewElement::FLUSH:
        {
            for (auto w: m_wFrames)
            {
                wrefresh(w.at(0));

                if (w.size() == 3)
                {
                    wrefresh(w.at(2));
                    wrefresh(w.at(1));
                }
                else if (w.size() == 4)
                {
                    wrefresh(w.at(1));
                    wrefresh(w.at(3));
                    wrefresh(w.at(2));
                }
            }

            for (auto w: m_wPlayers)
            {
                wrefresh(w);
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

    m_wPlayers.emplace_back(newwin(cellWidth * 2 - 1, m_nameWidth + 2, m_wPlayers.size() * (cellWidth * 2 - 1), 0));
    WINDOW* w = m_wPlayers.back();

    if (m_wPlayers.size() == 1)
    {
        wattron(w, A_BOLD); // FIrst user mark as active. He will start the 1st
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

    if (!isLastFrame)
    {
        FRAMES fr = {
            newwin(cellWidth, cellWidth + 1, m_wPlayers.size() * (cellWidth * 2 - 1), m_nameWidth + 1 + (m_wFrames.size() - 10 * m_wPlayers.size()) * cellWidth * 2),
            newwin(cellWidth, cellWidth, m_wPlayers.size() * (cellWidth * 2 - 1), m_nameWidth + cellWidth + 1 + (m_wFrames.size() - 10 * m_wPlayers.size()) * cellWidth * 2),
            newwin(cellWidth, cellWidth * 2, m_wPlayers.size() * (cellWidth * 2 - 1) + cellWidth - 1, m_nameWidth + 1 + (m_wFrames.size() - 10 * m_wPlayers.size()) * cellWidth * 2)
        };

        wborder(fr.at(0), 0, ' ', 0, ' ', 0, 0, ' ', ' ');
        wborder(fr.at(1), 0, 0, 0, 0, 0, 0, 0, 0);
        wborder(fr.at(2), 0, 0, ' ', 0, '|', '|', 0, 0);

        m_wFrames.push_back(std::move(fr));
    }
    else
    {
        FRAMES fr = {
            newwin(cellWidth, cellWidth + 1, m_wPlayers.size() * (cellWidth * 2 - 1), m_nameWidth + 1 + (m_wFrames.size() - 10 * m_wPlayers.size()) * cellWidth * 2),
            newwin(cellWidth, cellWidth + 1, m_wPlayers.size() * (cellWidth * 2 - 1), m_nameWidth + cellWidth + 1 + (m_wFrames.size() - 10 * m_wPlayers.size()) * cellWidth * 2),
            newwin(cellWidth, cellWidth, m_wPlayers.size() * (cellWidth * 2 - 1), m_nameWidth + 2 * cellWidth + 1 + (m_wFrames.size() - 10 * m_wPlayers.size()) * cellWidth * 2),
            newwin(cellWidth, cellWidth * 3, m_wPlayers.size() * (cellWidth * 2 - 1) + cellWidth - 1, m_nameWidth + 1 + (m_wFrames.size() - 10 * m_wPlayers.size()) * cellWidth * 2)
        };

        wborder(fr.at(0), 0, ' ', 0, ' ', 0, 0, ' ', ' ');
        wborder(fr.at(1), ' ', ' ', 0, ' ', ' ', ' ', ' ', ' ');
        wborder(fr.at(2), 0, 0, 0, 0, 0, 0, 0, 0);
        wborder(fr.at(3), 0, 0, ' ', 0, '|', '|', 0, 0);

        m_wFrames.push_back(std::move(fr));
    }
}

void ConsoleView::UpdateScore()
{
    throw std::runtime_error("Unimplemented functionality");
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
