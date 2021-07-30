#include "PointsListenerSimulation.h"
#include <stdexcept>

PointsListenerSimulation::PointsListenerSimulation()
{
    //
}

PointsListenerSimulation::~PointsListenerSimulation()
{
    //
}

void PointsListenerSimulation::Connect()
{
    //throw std::runtime_error("Unimplemented functionality");
}

unsigned short PointsListenerSimulation::Receive()
{
    std::discrete_distribution<unsigned short> distrib (0, 10);
    auto res = distrib(m_Generator);
    return res;
}

void PointsListenerSimulation::Shutdown()
{
    //throw std::runtime_error("Unimplemented functionality");
}
