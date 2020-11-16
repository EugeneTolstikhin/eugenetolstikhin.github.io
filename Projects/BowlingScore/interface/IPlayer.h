#ifndef __IPLAYER_H__
#define __IPLAYER_H__

#include <functional>
#include <vector>
#include <memory>

#include "IView.h"

class IPlayer
{
public:
    virtual ~IPlayer() = default;
    virtual void Play(const std::vector<std::shared_ptr<IView>>&, std::function<void()> gameover) = 0;
};

#endif // __IPLAYER_H__
