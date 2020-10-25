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
    virtual ~Lane();

    virtual void Play();
    virtual void AddPlayer(const IPlayer&);
private:

    std::vector<std::unique_ptr<IPlayer>> m_Players;
};

#endif // __LANE_H__