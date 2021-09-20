#include "PointsListenerSimulation.h"
#include "LoggerFactory.h"
#include "FileLogger.h"
#include <stdexcept>

PointsListenerSimulation::PointsListenerSimulation() :
    m_loggerFactory(new LoggerFactory)
    ,m_log(m_loggerFactory->CreateLogger(m_typeLogger))
    ,m_generator(m_device())
    ,m_distrib(0, 10)
{
    //
}

PointsListenerSimulation::~PointsListenerSimulation()
{
    //
}

void PointsListenerSimulation::Connect()
{
    //
}

unsigned short PointsListenerSimulation::Receive()
{
    auto res = m_distrib(m_generator);
    return res;
}

void PointsListenerSimulation::Shutdown()
{
    //
}
