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

    virtual void Connect() override;
    virtual unsigned short Receive() override;
    virtual void Shutdown() override;

private:
    std::unique_ptr<ILoggerFactory> m_loggerFactory;
    std::unique_ptr<ILogger> m_log;

    std::default_random_engine generator;
    std::uniform_int_distribution<int> distrib;
};

#endif //__POINTS_LISTENER_SIMULATION_H__
