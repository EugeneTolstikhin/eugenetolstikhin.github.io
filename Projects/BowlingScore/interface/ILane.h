#ifndef __ILANE_H__
#define __ILANE_H__

#include <functional>

class ILane
{
public:
    ILane() = default;
    virtual ~ILane() = default;

    virtual void Play(std::function<void()> gameover) = 0;
};

#endif // __ILANE_H__
