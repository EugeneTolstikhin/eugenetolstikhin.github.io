#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <memory>
#include "IPlayer.h"
#include "IGame.h"

class Player : public IPlayer
{
public:
    Player();
    virtual ~Player();

    virtual void GetName() const;
    virtual void GetGame() const;
private:

    std::unique_ptr<IGame> m_Game;
};

#endif // __PLAYER_H__