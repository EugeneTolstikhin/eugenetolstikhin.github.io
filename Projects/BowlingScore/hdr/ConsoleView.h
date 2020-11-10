#ifndef __CONSOLE_VIEW_H__
#define __CONSOLE_VIEW_H__

#include "IView.h"

class ConsoleView : public IView
{
public:
    ConsoleView();
    virtual ~ConsoleView();

    virtual void InitScoreTable(const std::vector<std::string>&);
    virtual void UpdateScore();
    virtual void CleanScore();
};

#endif //__CONSOLE_VIEW_H__
