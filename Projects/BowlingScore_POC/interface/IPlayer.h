#ifndef __IPLAYER_H__
#define __IPLAYER_H__

#include <functional>
#include <vector>
#include <memory>

#include "IView.h"

class IPlayer
{
public:
    IPlayer() = default;
    virtual ~IPlayer() = default;

    virtual void Play(std::function<void()> gameover) = 0;
};

#endif // __IPLAYER_H__
