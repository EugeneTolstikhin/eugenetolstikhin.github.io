#ifndef __IPOINTS_LISTENER_H__
#define __IPOINTS_LISTENER_H__

class IPointsListener
{
public:
    IPointsListener() = default;
    virtual ~IPointsListener() = default;

    virtual unsigned short Receive() = 0;
};

#endif //__IPOINTS_LISTENER_H__
