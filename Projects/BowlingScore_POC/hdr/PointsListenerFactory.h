#ifndef __POINTS_LISTENER_LOCAL_FACTORY_H__
#define __POINTS_LISTENER_LOCAL_FACTORY_H__

#include "IPointsListenerFactory.h"

class PointsListenerFactory : public IPointsListenerFactory
{
public:
    PointsListenerFactory();
    virtual ~PointsListenerFactory();

    virtual IPointsListener* CreatePointsListener(const ListenerType&) override;
};

#endif //__POINTS_LISTENER_LOCAL_FACTORY_H__
