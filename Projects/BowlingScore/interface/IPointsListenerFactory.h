#ifndef __IPOINTS_LISTENER_FACTORY_H__
#define __IPOINTS_LISTENER_FACTORY_H__

enum class ListenerType
{
    SIMULATION,
    LOCAL,
    NETWORK
};

class IPointsListener;
class IPointsListenerFactory
{
public:
    IPointsListenerFactory() = default;
    virtual ~IPointsListenerFactory() = default;

    virtual IPointsListener* CreatePointsListener(const ListenerType&) = 0;
};

#endif //__IPOINTS_LISTENER_FACTORY_H__
