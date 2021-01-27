#include "PointsListenerLocal.h"
#include <stdexcept>

PointsListenerLocal::PointsListenerLocal()
{
    //
}

PointsListenerLocal::~PointsListenerLocal()
{
    //
}

void PointsListenerLocal::Connect()
{
    throw std::runtime_error("Unimplemented functionality");
}

unsigned short PointsListenerLocal::Receive()
{
    throw std::runtime_error("Unimplemented functionality");
}

void PointsListenerLocal::Shutdown()
{
    throw std::runtime_error("Unimplemented functionality");
}
