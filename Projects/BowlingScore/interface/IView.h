#ifndef __IVIEW_H__
#define __IVIEW_H__

#include <vector>
#include <string>

class IView
{
public:
    IView() = default;
    virtual ~IView() = default;

    virtual void InitScoreTable(const std::vector<std::string>&) = 0;
    virtual void UpdateScore() = 0;
    virtual void CleanScore() = 0;
};

#endif //__IVIEW_H__
