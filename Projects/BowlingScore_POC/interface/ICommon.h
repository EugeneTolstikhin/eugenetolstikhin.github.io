#ifndef __ICOMMON_H__
#define __ICOMMON_H__

enum class Flag : unsigned short
{
    ZERO,
    NOTHING,
    SPARE,
    STRIKE
};

static const unsigned short MAX_FRAME_AMOUNT = 10;

#endif // __ICOMMON_H__