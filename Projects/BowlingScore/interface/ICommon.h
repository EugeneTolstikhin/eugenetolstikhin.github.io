#ifndef __ICOMMON_H__
#define __ICOMMON_H__

#include <cstddef>
#include <utility>

enum class Flag : unsigned short
{
    ZERO,
    NOTHING,
    SPARE,
    STRIKE
};

static const unsigned short MAX_FRAME_AMOUNT = 10;
static const unsigned short MAX_POINTS = 10;

template <typename Enum>
constexpr auto enum_index(Enum value) noexcept -> std::size_t
{
    return static_cast<std::size_t>(std::to_underlying(value));
}

#endif // __ICOMMON_H__

