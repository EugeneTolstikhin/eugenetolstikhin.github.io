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
static const unsigned short MAX_FRAMES_AMOUNT = 10;

class IFrame
{
public:
    IFrame() = default;
    virtual ~IFrame() = default;

    IFrame(const IFrame&) = delete;
    IFrame(IFrame&&) = delete;
    IFrame& operator = (const IFrame&) = delete;
    IFrame&& operator = (IFrame&&) = delete;

    virtual void SetFlag(const Flag&) noexcept = 0;
    virtual void SetLastFrame(const bool) noexcept = 0;
    virtual void SetCurrentTrialIndex(const Trial&) noexcept = 0;
    virtual void SetTrialPoints(const unsigned short) = 0;

    virtual Flag GetFlag() const noexcept = 0;
    virtual bool isLastFrame() const noexcept = 0;
    virtual Trial GetCurrentTrialIndex() const noexcept = 0;
    virtual unsigned short GetTrialPoints(const Trial&) const noexcept = 0;
};

#endif // __IFRAME_H__
