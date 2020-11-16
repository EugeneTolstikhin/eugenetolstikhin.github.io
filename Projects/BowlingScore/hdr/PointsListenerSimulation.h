#ifndef __POINTS_LISTENER_SIMULATION_H__
#define __POINTS_LISTENER_SIMULATION_H__

#include "IPointsListener.h"

class PointsListenerSimulation : public IPointsListener
{
public:
    PointsListenerSimulation();
    virtual ~PointsListenerSimulation();

    virtual void Connect() override;
    virtual unsigned short Receive() override;
    virtual void Shutdown() override;
};

#endif //__POINTS_LISTENER_SIMULATION_H__
