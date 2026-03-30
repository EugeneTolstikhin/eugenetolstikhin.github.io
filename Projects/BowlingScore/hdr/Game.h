#ifndef __GAME_H__
#define __GAME_H__

#include <functional>
#include <memory>
#include <optional>
#include <vector>
#include "IGame.h"
#include "IFrame.h"
#include "IView.h"
#include "ILoggerFactory.h"

class Game : public IGame
{
public:
    Game(IView*, GetPointsFunction);
    virtual ~Game();

	Game() = delete;
    Game(const Game&) = delete;
    Game(Game&&) = delete;
    Game& operator = (const Game&) = delete;
    Game&& operator = (Game&&) = delete;

    virtual void ThrowBall() override;
    virtual bool IsAnotherThrowAllowed() const noexcept override;
    virtual void CloseGame(std::function<void()> gameover) override;
    virtual void UpdateTotalScore(const short shift = 0) override;

private:
    void recordRoll(unsigned short);
    std::vector<unsigned short> collectBonusRolls(std::size_t, std::size_t) const;
    std::optional<unsigned short> tryGetFrameScore(std::size_t) const;

    LoggerType m_typeLogger = LoggerType::TO_FILE;

    GetPointsFunction m_getPoints;

    std::pair<std::shared_ptr<IFrame>, std::vector<std::shared_ptr<IFrame>>::iterator> m_currFrame;

    std::vector<std::shared_ptr<IFrame>> m_Frames;
    std::vector<std::vector<unsigned short>> m_frameRolls;
    std::vector<std::optional<unsigned short>> m_reportedCumulativeScores;

    std::unique_ptr<ILoggerFactory> m_loggerFactory;
    std::unique_ptr<ILogger> m_log;
    
    IView* m_view;

    std::size_t m_currentFrameIndex = 0;
};

#endif // __GAME_H__
