#ifndef __IVIEW_H__
#define __IVIEW_H__

#include <vector>
#include <string>

enum class ViewElement
{
    FRAME,
    GAME,
    PLAYER,
    LANE,
    FLUSH
};

class IView
{
public:
    IView() = default;
    virtual ~IView() = default;

    virtual void Draw(const ViewElement&, void* params = nullptr) = 0;
    virtual void UpdateScore() = 0;
    virtual void CleanScore() = 0;
};

#endif //__IVIEW_H__
