#ifndef __IGAME_H__
#define __IGAME_H__

#include "ICommon.h"
#include <functional>

class IGame
{
public:
    IGame() = default;
    virtual ~IGame() = default;

    virtual void ThrowBall() = 0;
    virtual bool IsAnotherThrowAllowed() const noexcept = 0;
    virtual void CloseGame(std::function<void()> gameover) = 0;
    virtual void UpdateTotalScore() = 0;
};

#endif // __IGAME_H__
