#ifndef __IGAME_H__
#define __IGAME_H__

#include <functional>

class IGame
{
public:
    virtual ~IGame() = default;
    virtual void ThrowBall() = 0;
    virtual bool IsAnotherThrowAllowed() = 0;
    virtual void CloseFrame(std::function<void()> gameover) = 0;
};

#endif // __IGAME_H__
