#ifndef __POINTS_LISTENER_SIMULATION_H__
#define __POINTS_LISTENER_SIMULATION_H__

#include "IPointsListener.h"
#include "ILoggerFactory.h"
#include <random>
#include <memory>

class PointsListenerSimulation : public IPointsListener
{
public:
    PointsListenerSimulation();
    virtual ~PointsListenerSimulation();

    virtual void Connect();
    virtual unsigned short Receive();
    virtual void Shutdown();

private:
    std::uniform_int_distribution<int> distrib;

    std::unique_ptr<ILoggerFactory> m_loggerFactory;
    std::unique_ptr<ILogger> m_log;
};

#endif //__POINTS_LISTENER_SIMULATION_H__
