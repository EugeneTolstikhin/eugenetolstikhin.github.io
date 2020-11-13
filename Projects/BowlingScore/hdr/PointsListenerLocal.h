#ifndef __POINTS_LISTENER_H__
#define __POINTS_LISTENER_H__

#include "IPointsListener.h"

class PointsListenerLocal : public IPointsListener
{
public:
    PointsListenerLocal();
    virtual ~PointsListenerLocal();

    virtual void Connect();
    virtual unsigned short Receive();
    virtual void Shutdown();
};

#endif //__POINTS_LISTENER_H__
