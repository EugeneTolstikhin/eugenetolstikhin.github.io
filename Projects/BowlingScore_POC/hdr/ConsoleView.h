#ifndef __CONSOLE_VIEW_H__
#define __CONSOLE_VIEW_H__

#include "IView.h"
#include <string>
#include <ncurses.h>

typedef std::vector<WINDOW*> FRAMES;

class ConsoleView : public IView
{
public:
    ConsoleView();
    virtual ~ConsoleView();

    virtual void Draw(const ViewElement&, void*);
    virtual void UpdateScore();
    virtual void CleanScore();

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

    std::vector<WINDOW*> m_wPlayers;
    std::vector<FRAMES> m_wFrames;
};

#endif //__CONSOLE_VIEW_H__
