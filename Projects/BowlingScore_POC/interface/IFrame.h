#ifndef __IFRAME_H__
#define __IFRAME_H__

#include "ICommon.h"
#include <vector>

enum class Trial : short
{
    ZERO = -1,
    FIRST,
    SECOND,
    THIRD
};

static const unsigned short MAX_POINTS = 10;

class IFrame
{
public:
    IFrame() = default;
    virtual ~IFrame() = default;

    IFrame(const IFrame&) = delete;
    IFrame(IFrame&&) = delete;
    IFrame& operator = (const IFrame&) = delete;
    IFrame&& operator = (IFrame&&) = delete;

    virtual void SetTrialPoints(const unsigned short) = 0;
    virtual bool isAllowedThrow() const noexcept = 0;
    virtual unsigned short GetTotalFramePoints() const noexcept = 0;
};

#endif // __IFRAME_H__
