#ifndef __CONSOLE_LOGGER_H__
#define __CONSOLE_LOGGER_H__

#include "ILogger.h"

class ConsoleLogger : public ILogger
{
public:
    ConsoleLogger();
    virtual ~ConsoleLogger();

    virtual void LogMe(const std::string&, int, const std::string&) const noexcept override;
};

#endif //__CONSOLE_LOGGER_H__
