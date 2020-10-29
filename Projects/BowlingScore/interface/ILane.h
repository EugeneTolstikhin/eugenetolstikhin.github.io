#ifndef __ILANE_H__
#define __ILANE_H__

class ILane
{
public:
    ILane() = default;
    virtual ~ILane() = default;

    virtual void Play() = 0;
};

#endif // __ILANE_H__