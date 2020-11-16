#ifndef __IFRAME_H__
#define __IFRAME_H__

enum class Flag
{
    NOTHING,
    SPARE,
    STRIKE
};

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
    virtual ~IFrame() = default;
    virtual void SetTrialPoints(const unsigned short) = 0;
    virtual bool isAllowedThrow() const noexcept = 0;
};

#endif // __IFRAME_H__
