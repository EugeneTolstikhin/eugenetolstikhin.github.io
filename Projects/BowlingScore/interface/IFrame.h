#ifndef __IFRAME_H__
#define __IFRAME_H__

class IFrame
{
public:
    IFrame() = default;
    virtual ~IFrame() = default;

    virtual void SetFlag() = 0;
};

#endif // __IFRAME_H__