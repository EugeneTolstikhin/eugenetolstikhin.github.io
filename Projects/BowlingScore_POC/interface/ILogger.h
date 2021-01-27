#ifndef __ILOGGER_H__
#define __ILOGGER_H__

#include <string>

class ILogger
{
public:
    ILogger() = default;
    virtual ~ILogger() = default;

    virtual void LogMe(const std::string&, int, const std::string&) = 0;
};

#endif // __ILOGGER_H__
