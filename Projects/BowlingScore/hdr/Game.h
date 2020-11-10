#ifndef __GAME_H__
#define __GAME_H__

#include <memory>
#include <forward_list>
#include "IGame.h"
#include "IFrame.h"
#include "IPointsListenerFactory.h"

class Game : public IGame
{
public:
    Game();
    virtual ~Game();

    virtual void ThrowBall() override;
    virtual bool IsAnotherThrowAllowed() override;
    virtual void CloseFrame(std::function<void()> gameover) override;

private:
    unsigned short waitForPoints();

    ListenerType m_listenerType = ListenerType::SIMULATION;
    std::forward_list<std::shared_ptr<IFrame>> m_Frames;
    std::pair<std::shared_ptr<IFrame>, decltype(m_Frames.begin())> m_currFrame;
    std::unique_ptr<IPointsListenerFactory> m_pointsListenerFactory;
};

#endif // __GAME_H__
