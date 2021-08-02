#ifndef __LOGGER_FACTORY_H__
#define __LOGGER_FACTORY_H__

#include "ILoggerFactory.h"

class LoggerFactory : public ILoggerFactory
{
public:
    LoggerFactory();
    virtual ~LoggerFactory();

    virtual ILogger* CreateLogger(const LoggerType&) override;
};

#endif //__LOGGER_FACTORY_H__
