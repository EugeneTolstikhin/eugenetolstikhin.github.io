#ifndef __POINTS_LISTENER_LOCAL_FACTORY_H__
#define __POINTS_LISTENER_LOCAL_FACTORY_H__

enum class ListenerType
{
    SIMULATION,
    LOCAL,
    NETWORK
};

class IPointsListener;
class PointsListenerFactory
{
public:
    PointsListenerFactory();
    ~PointsListenerFactory();

    IPointsListener* CreatePointsListener(const ListenerType&);
};

#endif //__POINTS_LISTENER_LOCAL_FACTORY_H__
