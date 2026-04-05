#ifndef __ILOGGER_FACTORY_H__
#define __ILOGGER_FACTORY_H__

#include <memory>

enum class LoggerType
{
    CLS,
    TO_FILE,
    NETWORK
};

class ILogger;
class ILoggerFactory
{
public:
    ILoggerFactory() = default;
    virtual ~ILoggerFactory() = default;

    virtual std::unique_ptr<ILogger> CreateLogger(const LoggerType&) = 0;
};

#endif //__ILOGGER_FACTORY_H__

