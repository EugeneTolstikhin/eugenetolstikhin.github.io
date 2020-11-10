#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "IPlayer.h"
#include "IGame.h"

class Player : public IPlayer
{
public:
    Player();
    Player(const std::string &);
    virtual ~Player();

    virtual void Play(const std::vector<std::shared_ptr<IView>>&, std::function<void()> gameover);

private:
    std::string m_PlayerName;
    std::unique_ptr<IGame> m_Game;
};

#endif // __PLAYER_H__
