#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <memory>
#include "IPlayer.h"
#include "IGame.h"

class Player : public IPlayer
{
public:
    Player();
    Player(const std::string &);
    virtual ~Player();

    virtual void Play(std::function<void()> gameover);

private:
    std::string m_PlayerName;
    std::unique_ptr<IGame> m_Game;
};

#endif // __PLAYER_H__