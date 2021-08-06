#ifndef __GAME_H__
#define __GAME_H__

#include <memory>
#include <vector>
#include <list>
#include "IGame.h"
#include "IFrame.h"
#include "IPointsListenerFactory.h"
#include "IView.h"
#include "ILoggerFactory.h"

class Game : public IGame
{
public:
    Game();
    Game(const std::vector<std::shared_ptr<IView>>&);
    virtual ~Game();

    virtual void ThrowBall() override;
    virtual bool IsAnotherThrowAllowed() const noexcept override;
    virtual void CloseGame(std::function<void()> gameover) override;
    virtual void UpdateTotalScore(const short shift = 0) override;

private:
    unsigned short waitForPoints();

    ListenerType m_listenerType = ListenerType::SIMULATION;
    std::vector<std::shared_ptr<IFrame>> m_Frames;
    std::pair<std::shared_ptr<IFrame>, std::vector<std::shared_ptr<IFrame>>::iterator> m_currFrame;
    std::unique_ptr<IPointsListenerFactory> m_pointsListenerFactory;
    std::vector<std::shared_ptr<IView>> m_Views;

    std::unique_ptr<ILoggerFactory> m_loggerFactory;
    std::unique_ptr<ILogger> m_log;

    std::list<Flag> m_lastFlags;
    unsigned short m_frameTotalPoints = 0;
};

#endif // __GAME_H__
