#ifndef __GAME_H__
#define __GAME_H__

#include <memory>
#include <vector>
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

    Game(const Game&) = delete;
    Game(Game&&) = delete;
    Game& operator = (const Game&) = delete;
    Game&& operator = (Game&&) = delete;

    virtual void ThrowBall() override;
    virtual bool IsAnotherThrowAllowed() override;
    virtual void CloseFrame(std::function<void()> gameover) override;

private:
    unsigned short waitForPoints();

    ListenerType m_listenerType = ListenerType::SIMULATION;
    std::vector<std::shared_ptr<IFrame>> m_Frames;
    std::pair<std::shared_ptr<IFrame>, std::vector<std::shared_ptr<IFrame>>::iterator> m_currFrame;
    std::unique_ptr<IPointsListenerFactory> m_pointsListenerFactory;
    std::vector<std::shared_ptr<IView>> m_Views;
    size_t m_FramesAmount = 0;
};

#endif // __GAME_H__
