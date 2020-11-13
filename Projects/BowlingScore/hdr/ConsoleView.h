#ifndef __CONSOLE_VIEW_H__
#define __CONSOLE_VIEW_H__

#include "IView.h"
#include <string>

class ConsoleView : public IView
{
public:
    ConsoleView();
    virtual ~ConsoleView();

    virtual void Draw(const ViewElement&, void*);
    virtual void UpdateScore();
    virtual void CleanScore();

private:
    void InitLaneScore();
    void InitPlayerScore(const std::string&);
    void InitGameScore();
    void InitFrameScore(bool);

    size_t m_RowWidth = 0;
    size_t m_nameWidth = 0;
    size_t m_columnAmount = 0;
    size_t m_normalFrameCellsAmount = 0;
    size_t m_normalFramesAmount = 0;
    size_t m_lastFrameCellsAmount = 0;

    std::string m_PlayerGame;
    std::vector<std::vector<std::string>> m_Frames;

    std::string m_ScoreTable;
};

#endif //__CONSOLE_VIEW_H__
