#ifndef __IGAME_H__
#define __IGAME_H__

#include <functional>

static const unsigned short MAX_FRAME_AMOUNT = 10;

class IGame
{
public:
    IGame() = default;
    virtual ~IGame() = default;

    virtual void ThrowBall() = 0;
    virtual bool IsAnotherThrowAllowed() = 0;
    virtual void CloseGame(std::function<void()> gameover) = 0;
};

#endif // __IGAME_H__
