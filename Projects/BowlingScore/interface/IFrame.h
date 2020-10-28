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

class IFrame
{
public:
    IFrame() = default;
    virtual ~IFrame() = default;

    IFrame(const IFrame&) = delete;
    IFrame(IFrame&&) = delete;
    IFrame& operator = (const IFrame&) = delete;
    IFrame&& operator = (IFrame&&) = delete;

    virtual void SetFlag(const Flag&) = 0;
    virtual void Set10thFrame(const bool) = 0;
    virtual void SetCurrentTrialNumber(const Trial&) = 0;
    virtual void SetTrialPoints(const Trial&, const unsigned short) = 0;

    virtual Flag GetFlag() const = 0;
    virtual bool is10thFrame() const = 0;
    virtual Trial GetCurrentTrialNumber() const = 0;
    virtual unsigned short GetTrialPoints(const Trial&) const = 0;
};

#endif // __IFRAME_H__
