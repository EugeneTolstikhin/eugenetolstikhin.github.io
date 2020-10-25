#ifndef __FRAME_H__
#define __FRAME_H__

#include "IFrame.h"

class Frame : public IFrame
{
public:
    Frame();
    virtual ~Frame();

    virtual void SetFlag();
private:
};

#endif // __FRAME_H__