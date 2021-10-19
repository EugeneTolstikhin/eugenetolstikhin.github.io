#ifndef __ILOGGER_H__
#define __ILOGGER_H__

#include <string>

class ILogger
{
public:
    ILogger() = default;
    virtual ~ILogger() = default;

    virtual void LogMe(const char*, int, const char*, ...) = 0;
};

#endif // __ILOGGER_H__
