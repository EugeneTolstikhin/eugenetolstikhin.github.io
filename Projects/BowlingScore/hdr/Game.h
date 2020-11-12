#ifndef __GAME_H__
#define __GAME_H__

#include <memory>
#include <list>
#include "IGame.h"
#include "IFrame.h"
#include "IPointsListenerFactory.h"
#include "IView.h"

class Game : public IGame
{
public:
    Game();
    Game(const std::vector<std::shared_ptr<IView>>&);
    virtual ~Game();

    virtual void ThrowBall() override;
    virtual bool IsAnotherThrowAllowed() override;
    virtual void CloseFrame(std::function<void()> gameover) override;

private:
    unsigned short waitForPoints();

    ListenerType m_listenerType = ListenerType::SIMULATION;
    std::list<std::shared_ptr<IFrame>> m_Frames;
    std::pair<std::shared_ptr<IFrame>, decltype(m_Frames.begin())> m_currFrame;
    std::unique_ptr<IPointsListenerFactory> m_pointsListenerFactory;
    std::vector<std::shared_ptr<IView>> m_Views;
};

#endif // __GAME_H__
