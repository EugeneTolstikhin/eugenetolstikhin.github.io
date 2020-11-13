#ifndef __IPOINTS_LISTENER_H__
#define __IPOINTS_LISTENER_H__

class IPointsListener
{
public:
    IPointsListener() = default;
    virtual ~IPointsListener() = default;

    virtual void Connect() = 0;
    virtual unsigned short Receive() = 0;
    virtual void Shutdown() = 0;
};

#endif //__IPOINTS_LISTENER_H__
