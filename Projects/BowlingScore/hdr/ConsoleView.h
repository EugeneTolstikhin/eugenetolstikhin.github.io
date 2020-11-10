#ifndef __CONSOLE_VIEW_H__
#define __CONSOLE_VIEW_H__

#include "IView.h"
#include <string>

class ConsoleView : public IView
{
public:
    ConsoleView();
    virtual ~ConsoleView();

    virtual void InitScoreTableFrame(const std::vector<std::string>&);
    virtual void InitPlayerScore(const std::string&);
    virtual void InitFlush();
    virtual void UpdateScore();
    virtual void CleanScore();
};

#endif //__CONSOLE_VIEW_H__
