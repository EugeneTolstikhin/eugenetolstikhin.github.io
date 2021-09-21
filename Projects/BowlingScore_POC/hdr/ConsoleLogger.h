#ifndef __CONSOLE_LOGGER_H__
#define __CONSOLE_LOGGER_H__

#include "ILogger.h"

class ConsoleLogger : public ILogger
{
public:
    ConsoleLogger();
    virtual ~ConsoleLogger();

    ConsoleLogger(const ConsoleLogger&) = delete;
    ConsoleLogger(ConsoleLogger&&) = delete;
    ConsoleLogger& operator = (const ConsoleLogger&) = delete;
    ConsoleLogger&& operator = (ConsoleLogger&&) = delete;

    virtual void LogMe(const std::string&, int, const std::string&, ...) override;
};

#endif //__CONSOLE_LOGGER_H__
