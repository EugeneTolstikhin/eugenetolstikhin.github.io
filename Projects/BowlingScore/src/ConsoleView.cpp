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
    m_loggerFactory(std::make_unique<LoggerFactory>())
    ,m_log(m_loggerFactory->CreateLogger(m_typeLogger))
{
    setlocale(LC_ALL, "");
    initscr();
    m_canBeColored = !!has_colors();

    if (m_canBeColored)
    {
        start_color();
        init_pair(1, COLOR_RED, COLOR_BLACK);
    }

    refresh();
    curs_set(0);
    cbreak();
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
                delwin(frame.first);
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
                    wrefresh(frames.at(0).first);

                    if (frames.size() == 3)
                    {
                        wrefresh(frames.at(2).first);
                        wrefresh(frames.at(1).first);
                    }
                    else if (frames.size() == 4)
                    {
                        wrefresh(frames.at(1).first);
                        wrefresh(frames.at(3).first);
                        wrefresh(frames.at(2).first);
                    }
                }
            }

            for (auto w: m_wPlayers)
            {
                wrefresh(w.second);
            }

            m_ActiveFrame = &m_wGames.front().front().front();
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

    wborder(w, 0, 0, 0, 0, 0, 0, 0, 0);
    if (m_wPlayers.size() == 1)
    {
        if (m_canBeColored)
        {
            wattron(w, COLOR_PAIR(1));
        }

        wattron(w, A_BOLD); // First user mark as active. He will start the 1st
    }

    PrintPlayerName(w, displayName);
}

void ConsoleView::DrawFrameScore(const bool isLastFrame)
{
    std::vector<std::string> m_FrameLines;

    size_t playerIdx = m_wPlayers.size();
    size_t gameIdx = m_Game.size();

    if (!isLastFrame)
    {
        FRAMES fr = {
            //                      nlines,      ncols,          begin_y,                                            begin_x 
            std::make_pair(newwin(m_cellWidth, m_cellWidth,    playerIdx * (m_cellWidth * 2 - 1),                  m_nameWidth + 1 + gameIdx * m_cellWidth * 2), Flag::NOTHING),
            std::make_pair(newwin(m_cellWidth, m_cellWidth,    playerIdx * (m_cellWidth * 2 - 1),                  m_nameWidth + m_cellWidth + 1 + gameIdx * m_cellWidth * 2), Flag::NOTHING),
            std::make_pair(newwin(m_cellWidth, m_cellWidth * 2,playerIdx * (m_cellWidth * 2 - 1) + m_cellWidth - 1,m_nameWidth + 1 + gameIdx * m_cellWidth * 2), Flag::NOTHING)
        };

        wborder(fr.at(0).first, 0, ' ', 0, ' ', 0, ' ', ' ', ' ');
        wborder(fr.at(1).first, 0, 0, 0, 0, 0, 0, 0, 0);
        wborder(fr.at(2).first, 0, 0, ' ', 0, '|', '|', 0, 0);

        m_Game.push_back(std::move(fr));
    }
    else
    {
        FRAMES fr = {
            std::make_pair(newwin(m_cellWidth, m_cellWidth,    playerIdx * (m_cellWidth * 2 - 1),                  m_nameWidth + 1 + gameIdx * m_cellWidth * 2), Flag::NOTHING),
            std::make_pair(newwin(m_cellWidth, m_cellWidth,    playerIdx * (m_cellWidth * 2 - 1),                  m_nameWidth + m_cellWidth + 1 + gameIdx * m_cellWidth * 2), Flag::NOTHING),
            std::make_pair(newwin(m_cellWidth, m_cellWidth,    playerIdx * (m_cellWidth * 2 - 1),                  m_nameWidth + 2 * m_cellWidth + 1 + gameIdx * m_cellWidth * 2), Flag::NOTHING),
            std::make_pair(newwin(m_cellWidth, m_cellWidth * 3,playerIdx * (m_cellWidth * 2 - 1) + m_cellWidth - 1,m_nameWidth + 1 + gameIdx * m_cellWidth * 2), Flag::NOTHING)
        };

        wborder(fr.at(0).first, 0, ' ', 0, ' ', 0, ' ', ' ', ' ');
        wborder(fr.at(1).first, ' ', ' ', 0, ' ', ' ', ' ', ' ', ' ');
        wborder(fr.at(2).first, 0, 0, 0, 0, 0, 0, 0, 0);
        wborder(fr.at(3).first, 0, 0, ' ', 0, '|', '|', 0, 0);

        m_Game.push_back(std::move(fr));
    }
}

void ConsoleView::UpdateFrameScore(const unsigned short score, const Flag& flag)
{
    std::string res;

    switch (flag)
    {
        case Flag::STRIKE:
        {
            res = "X";
            break;
        }
        case Flag::SPARE:
        {
            res = "/";
            break;
        }
        case Flag::NOTHING:
        {
            res = std::to_string(score);
            break;
        }
        case Flag::ZERO:
        {
            res = "-";
            break;
        }
        default:
            break;
    }

    m_ActiveFrame->second = flag;
    mvwprintw(m_ActiveFrame->first, m_cellWidth / 2, m_cellWidth / 2, res.c_str());
    wrefresh(m_ActiveFrame->first);
    usleep(SLEEP_TIME);
}

void ConsoleView::UpdateScore(const unsigned short score, const short prevIdxShift, const bool gameover)
{
    if (gameover)
    {
        m_log->LogMe(__FILE__, __LINE__, "Gameover");
        UpdateScoreFrame(score, 0);
    }
    else
    {
        if (m_ActiveFramesIdx <= m_normalFramesAmount)
        {
            auto frames = m_wGames.at(m_ActivePlayerIdx).at(m_ActiveFramesIdx);
            if (   frames.at(frames.size() - 2).second == Flag::SPARE
                ||  frames.at(frames.size() - 3).second == Flag::STRIKE)
            {
                if (prevIdxShift < 0)
                {
                    UpdateScoreFrame(score, prevIdxShift);
                }
            }
            else
            {
                UpdateScoreFrame(score, prevIdxShift);
            }
        }
    }
}

void ConsoleView::UpdateScoreFrame(const unsigned short score, const short prevIdxShift)
{
    if (m_ActiveFramesIdx <= m_normalFramesAmount)
    {
        auto frame = m_wGames.at(m_ActivePlayerIdx).at(m_ActiveFramesIdx + prevIdxShift).back();
        std::string sscore = std::to_string(score);
        mvwprintw(frame.first, m_cellWidth / 2, m_cellWidth + sscore.length() / 2 - 2, std::to_string(score).c_str());
        wrefresh(frame.first);
        sscore.clear();
        usleep(SLEEP_TIME);
    }
}

void ConsoleView::CleanScore()
{
    clear();
    m_wPlayers.clear();
    m_wGames.clear();
}

void ConsoleView::SetNextPlayerActive()
{
    WINDOW* w = m_wPlayers.at(m_ActivePlayerIdx).second;
    auto displayName = m_wPlayers.at(m_ActivePlayerIdx).first;

    if (m_canBeColored)
    {
        wattroff(w, COLOR_PAIR(1));
    }

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

    if (m_canBeColored)
    {
        wattron(w, COLOR_PAIR(1));
    }
    
    wattron(w, A_BOLD);
    PrintPlayerName(w, displayName);
    usleep(SLEEP_TIME);
}

void ConsoleView::SetNextFrameActive(const bool last)
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

    if (m_ActiveFramesIdx <= m_normalFramesAmount)
    {
        m_ActiveFrame = &m_wGames.at(m_ActivePlayerIdx).at(m_ActiveFramesIdx).at(m_ActiveFrameIdx);
    }
}

void ConsoleView::PrintPlayerName(WINDOW* w, const std::string& name)
{
    mvwprintw(w, m_cellWidth - 1, 1, name.c_str());   // OK only for ASCII symbols
    wrefresh(w);
}
