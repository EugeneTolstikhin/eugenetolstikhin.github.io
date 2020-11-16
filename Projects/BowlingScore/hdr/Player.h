#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "IPlayer.h"
#include "IGame.h"

class Player : public IPlayer
{
public:
    Player();
    Player(const std::string &, const std::vector<std::shared_ptr<IView>>&);
    virtual ~Player();

    Player(const Player&) = delete;
    Player(Player&&) = delete;
    Player& operator = (const Player&) = delete;
    Player&& operator = (Player&&) = delete;


    virtual void Play(const std::vector<std::shared_ptr<IView>>&, std::function<void()> gameover) override;

private:
    std::string m_PlayerName;
    std::vector<std::shared_ptr<IView>> m_Views;
    std::unique_ptr<IGame> m_Game;
};

#endif // __PLAYER_H__
