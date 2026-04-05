#include "PointsListenerFactory.h"
#include "PointsListenerLocal.h"
#include "PointsListenerSimulation.h"

#include <stdexcept>

PointsListenerFactory::PointsListenerFactory()
{
    //
}

PointsListenerFactory::~PointsListenerFactory()
{
    //
}

std::unique_ptr<IPointsListener> PointsListenerFactory::CreatePointsListener(const ListenerType& type)
{
    switch (type)
    {
        case ListenerType::SIMULATION:
            return std::make_unique<PointsListenerSimulation>();
        case ListenerType::LOCAL:
            return std::make_unique<PointsListenerLocal>();
        case ListenerType::NETWORK:
            //TODO: return the proper instance of class here when it will be implemented
            throw std::runtime_error("Unimplemented type of Listener");
        default:
            throw std::out_of_range("Unknown type of Listener");
    }
    
    return {};
}

