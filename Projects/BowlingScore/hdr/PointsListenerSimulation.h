#ifndef __POINTS_LISTENER_SIMULATION_H__
#define __POINTS_LISTENER_SIMULATION_H__

#include "IPointsListener.h"

#include <random>
#include <memory>

class PointsListenerSimulation : public IPointsListener
{
public:
    PointsListenerSimulation();
    virtual ~PointsListenerSimulation();

    PointsListenerSimulation(const PointsListenerSimulation&) = delete;
    PointsListenerSimulation(PointsListenerSimulation&&) = delete;
    PointsListenerSimulation& operator = (const PointsListenerSimulation&) = delete;
    PointsListenerSimulation&& operator = (PointsListenerSimulation&&) = delete;

    virtual unsigned short Receive() override;

private:
    std::random_device m_device;
    std::mt19937 m_generator;
    std::uniform_int_distribution<std::mt19937::result_type> m_distrib;
};

#endif //__POINTS_LISTENER_SIMULATION_H__
