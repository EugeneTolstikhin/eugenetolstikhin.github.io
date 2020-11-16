#ifndef __ILOGGER_H__
#define __ILOGGER_H__

#include <string>

class ILogger
{
public:
    virtual ~ILogger() = default;
    virtual void LogMe(const std::string&, int, const std::string&) const noexcept = 0;
};

#endif // __ILOGGER_H__
