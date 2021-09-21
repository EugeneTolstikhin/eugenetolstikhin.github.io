#ifndef __POINTS_LISTENER_LOCAL_FACTORY_H__
#define __POINTS_LISTENER_LOCAL_FACTORY_H__

#include "IPointsListenerFactory.h"

class PointsListenerFactory : public IPointsListenerFactory
{
public:
    PointsListenerFactory();
    virtual ~PointsListenerFactory();

    PointsListenerFactory(const PointsListenerFactory&) = delete;
    PointsListenerFactory(PointsListenerFactory&&) = delete;
    PointsListenerFactory& operator = (const PointsListenerFactory&) = delete;
    PointsListenerFactory&& operator = (PointsListenerFactory&&) = delete;

    virtual IPointsListener* CreatePointsListener(const ListenerType&) override;
};

#endif //__POINTS_LISTENER_LOCAL_FACTORY_H__
