#ifndef __ILOGGER_FACTORY_H__
#define __ILOGGER_FACTORY_H__

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

    virtual ILogger* CreateLogger(const LoggerType&) = 0;
};

#endif //__ILOGGER_FACTORY_H__
