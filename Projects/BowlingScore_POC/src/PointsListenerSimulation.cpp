#include "PointsListenerSimulation.h"
#include "LoggerFactory.h"
#include "FileLogger.h"
#include <stdexcept>

PointsListenerSimulation::PointsListenerSimulation() :
    m_loggerFactory(new LoggerFactory)
    ,m_log(m_loggerFactory->CreateLogger(LoggerType::TO_FILE))
    ,m_generator(m_device())
    ,m_distrib(0, 10)
{
    //m_log->LogMe(__FILE__, __LINE__, std::string("Enter points listener simulator ctr"));
}

PointsListenerSimulation::~PointsListenerSimulation()
{
    //m_log->LogMe(__FILE__, __LINE__, std::string("Enter points listener simulator dtr"));
}

void PointsListenerSimulation::Connect()
{
    //m_log->LogMe(__FILE__, __LINE__, std::string("Connect to points listener simulator"));
}

unsigned short PointsListenerSimulation::Receive()
{
    //m_log->LogMe(__FILE__, __LINE__, std::string("Generate random number between 0 and 10"));
    auto res = m_distrib(m_generator);
    //m_log->LogMe(__FILE__, __LINE__, std::string("Result random number = ") + std::to_string(res));
    return res;
}

void PointsListenerSimulation::Shutdown()
{
    //m_log->LogMe(__FILE__, __LINE__, std::string("Shutdown points listener simulator"));
}
