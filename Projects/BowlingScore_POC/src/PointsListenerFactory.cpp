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

IPointsListener* PointsListenerFactory::CreatePointsListener(const ListenerType& type)
{
    switch (type)
    {
        case ListenerType::SIMULATION:
            return new PointsListenerSimulation;
        case ListenerType::LOCAL:
            return new PointsListenerLocal;
        case ListenerType::NETWORK:
            //TODO: return the proper instance of class here when it will be implemented
            throw std::runtime_error("Unimplemented type of Listener");
        default:
            throw std::out_of_range("Unknown type of Listener");
    }
    
    return nullptr;
}
