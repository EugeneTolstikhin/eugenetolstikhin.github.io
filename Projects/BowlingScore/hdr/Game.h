#ifndef __GAME_H__
#define __GAME_H__

#include <memory>
#include <vector>
#include "IGame.h"
#include "IFrame.h"

class Game : public IGame
{
public:
    Game();
    virtual ~Game();

    virtual void GameOver();
private:
    std::vector<std::unique_ptr<IFrame>> m_Frames;
};

#endif // --GAME_H__