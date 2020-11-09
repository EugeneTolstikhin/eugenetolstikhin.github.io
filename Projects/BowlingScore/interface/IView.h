#ifndef __IVIEW_H__
#define __IVIEW_H__

class IView
{
public:
    IView() = default;
    virtual ~IView() = default;

    virtual void InitScoreTable() = 0;
    virtual void UpdateScore() = 0;
};

#endif //__IVIEW_H__
