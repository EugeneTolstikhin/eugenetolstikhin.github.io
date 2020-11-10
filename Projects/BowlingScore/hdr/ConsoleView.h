#ifndef __CONSOLE_VIEW_H__
#define __CONSOLE_VIEW_H__

#include "IView.h"

class ConsoleView : public IView
{
public:
    ConsoleView();
    virtual ~ConsoleView();

    virtual void InitScoreTable();
    virtual void UpdateScore();
};

#endif //__CONSOLE_VIEW_H__
