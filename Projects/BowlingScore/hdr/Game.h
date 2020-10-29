#ifndef __GAME_H__
#define __GAME_H__

#include <memory>
#include <list>
#include "IGame.h"
#include "IFrame.h"

class Game : public IGame
{
public:
    Game();
    virtual ~Game();

    virtual void ThrowBall();
    virtual bool IsAnotherThrowAllowed();
    virtual void CloseFrame();

    virtual void GameOver();
private:
    unsigned short waitForPoints();

    std::list<std::shared_ptr<IFrame>> m_Frames;
    std::shared_ptr<IFrame> m_currFrame;
};

#endif // --GAME_H__