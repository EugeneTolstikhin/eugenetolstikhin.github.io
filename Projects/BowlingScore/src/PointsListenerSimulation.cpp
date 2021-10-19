#include "PointsListenerSimulation.h"

PointsListenerSimulation::PointsListenerSimulation() :
    m_generator(m_device())
    ,m_distrib(0, 10)
{
    //
}

PointsListenerSimulation::~PointsListenerSimulation()
{
    //
}

unsigned short PointsListenerSimulation::Receive()
{
    auto res = m_distrib(m_generator);
    return res;
}
