#ifndef __LANE_H__
#define __LANE_H__

#include <vector>
#include <memory>
#include "ILane.h"
#include "IPlayer.h"

class Lane : public ILane
{
public:
    Lane();
    Lane(const std::vector<std::string>&);
    virtual ~Lane();

    virtual void Play(std::function<void()> gameover);
private:

    std::vector<std::unique_ptr<IPlayer>> m_Players;
};

#endif // __LANE_H__
