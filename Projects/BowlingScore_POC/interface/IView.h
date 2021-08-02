#ifndef __IVIEW_H__
#define __IVIEW_H__

#include <vector>
#include <string>

enum class ViewElement
{
    FRAME,
    PLAYER,
    FLUSH
};

class IView
{
public:
    IView() = default;
    virtual ~IView() = default;

    virtual void Draw(const ViewElement&, void* params = nullptr) = 0;
    virtual void UpdateScore(unsigned short) = 0;
    virtual void CleanScore() = 0;
    virtual void SetNextFrameActive(bool) = 0;
};

#endif //__IVIEW_H__
