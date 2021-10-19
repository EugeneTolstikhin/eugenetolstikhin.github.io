#ifndef __GAME_H__
#define __GAME_H__

#include <memory>
#include <vector>
#include <list>
#include <functional>
#include "IGame.h"
#include "IFrame.h"
#include "IView.h"
#include "ILoggerFactory.h"

class Game : public IGame
{
public:
    Game() = delete;
    Game(IView*, GetPointsFunction);
    virtual ~Game();

    Game(const Game&) = delete;
    Game(Game&&) = delete;
    Game& operator = (const Game&) = delete;
    Game&& operator = (Game&&) = delete;

    virtual void ThrowBall() override;
    virtual bool IsAnotherThrowAllowed() const noexcept override;
    virtual void CloseGame(std::function<void()> gameover) override;
    virtual void UpdateTotalScore(const short shift = 0) override;

private:
    LoggerType m_typeLogger = LoggerType::TO_FILE;

    GetPointsFunction m_getPoints;

    std::pair<std::shared_ptr<IFrame>, std::vector<std::shared_ptr<IFrame>>::iterator> m_currFrame;

    std::vector<std::shared_ptr<IFrame>> m_Frames;
    std::vector<unsigned short> m_framePoints;

    std::list<Flag> m_lastFlags;

    std::unique_ptr<ILoggerFactory> m_loggerFactory;
    std::unique_ptr<ILogger> m_log;
    
    IView* m_view;

    unsigned short m_frameTotalPoints = 0;
    short m_lastFrameCounter = -2;

    bool m_gameOver = false;
};

#endif // __GAME_H__
