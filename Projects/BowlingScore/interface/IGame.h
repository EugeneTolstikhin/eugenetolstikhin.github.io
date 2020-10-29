#ifndef __IGAME_H__
#define __IGAME_H__

class IGame
{
public:
    IGame() = default;
    virtual ~IGame() = default;

    virtual void ThrowBall() = 0;
    virtual bool IsAnotherThrowAllowed() = 0;
    virtual void CloseFrame() = 0;
    virtual void GameOver() = 0;
};

#endif // __IGAME_H__