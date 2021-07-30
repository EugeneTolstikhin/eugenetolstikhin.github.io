#ifndef __CONSOLE_VIEW_H__
#define __CONSOLE_VIEW_H__

#include "IView.h"
#include "ILoggerFactory.h"
#include <string>
#include <ncurses.h>
#include <memory>

typedef std::vector<std::pair<unsigned short, WINDOW*>> FRAMES;
typedef std::vector<FRAMES> GAME;
typedef std::pair<std::string, WINDOW*> PLAYER;

class ConsoleView : public IView
{
public:
    ConsoleView();
    virtual ~ConsoleView();

    virtual void Draw(const ViewElement&, void*);
    virtual void UpdateScore(unsigned short);
    virtual void CleanScore();
    virtual void SetNextPlayerActive();
    virtual void SetNextFrameActive();

private:
    void DrawPlayerScore(const std::string&);
    void DrawFrameScore(bool, char sign = ' ');

    size_t m_RowWidth = 0;
    size_t m_nameWidth = 0;
    size_t m_columnAmount = 0;
    size_t m_normalFrameCellsAmount = 0;
    size_t m_normalFramesAmount = 0;
    size_t m_lastFrameCellsAmount = 0;

    std::string m_PlayerGame;
    std::string m_ScoreTable;

    short m_ActivePlayerIdx = 0;
    short m_ActiveFrameIdx = 0;

    GAME m_Game;

    std::vector<PLAYER> m_wPlayers;
    std::vector<GAME> m_wGames;

    std::unique_ptr<ILoggerFactory> m_loggerFactory;
    std::unique_ptr<ILogger> m_log;
};

#endif //__CONSOLE_VIEW_H__
