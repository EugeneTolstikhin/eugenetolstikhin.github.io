#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "IPlayer.h"
#include "IGame.h"
#include "ILoggerFactory.h"
#include "ILogger.h"

class Player : public IPlayer
{
public:
    Player(IView*, GetPointsFunction);
    Player(const std::string&, IView*, GetPointsFunction);
    virtual ~Player();

	Player() = delete;
    Player(const Player&) = delete;
    Player(Player&&) = delete;
    Player& operator = (const Player&) = delete;
    Player&& operator = (Player&&) = delete;

    virtual void Play(std::function<void()> gameover) override;

private:
    LoggerType m_typeLogger = LoggerType::TO_FILE;

    std::string m_PlayerName;
    std::unique_ptr<IGame> m_Game;
    std::unique_ptr<ILoggerFactory> m_loggerFactory;
    std::unique_ptr<ILogger> m_log;

    IView* m_view;
};

#endif // __PLAYER_H__
