#ifndef __GAME_H__
#define __GAME_H__

#include <memory>
#include <forward_list>
#include "IGame.h"
#include "IFrame.h"

class Game : public IGame
{
public:
    Game();
    virtual ~Game();

    virtual void ThrowBall() override;
    virtual bool IsAnotherThrowAllowed() const noexcept override;
    virtual void CloseFrame(std::function<void()> gameover) override;

private:
    unsigned short waitForPoints();

    std::forward_list<std::shared_ptr<IFrame>> m_Frames;
    std::pair<std::shared_ptr<IFrame>, decltype(m_Frames.begin())> m_currFrame;
};

#endif // --GAME_H__