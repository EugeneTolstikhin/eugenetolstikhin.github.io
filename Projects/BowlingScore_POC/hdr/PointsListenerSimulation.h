#ifndef __POINTS_LISTENER_SIMULATION_H__
#define __POINTS_LISTENER_SIMULATION_H__

#include "IPointsListener.h"
#include <random>

class PointsListenerSimulation : public IPointsListener
{
public:
    PointsListenerSimulation();
    virtual ~PointsListenerSimulation();

    virtual void Connect();
    virtual unsigned short Receive();
    virtual void Shutdown();

private:
    std::default_random_engine m_Generator;
};

#endif //__POINTS_LISTENER_SIMULATION_H__
