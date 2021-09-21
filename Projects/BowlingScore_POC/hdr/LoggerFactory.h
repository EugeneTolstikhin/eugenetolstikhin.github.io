#ifndef __LOGGER_FACTORY_H__
#define __LOGGER_FACTORY_H__

#include "ILoggerFactory.h"

class LoggerFactory : public ILoggerFactory
{
public:
    LoggerFactory();
    virtual ~LoggerFactory();

    LoggerFactory(const LoggerFactory&) = delete;
    LoggerFactory(LoggerFactory&&) = delete;
    LoggerFactory& operator = (const LoggerFactory&) = delete;
    LoggerFactory&& operator = (LoggerFactory&&) = delete;

    virtual ILogger* CreateLogger(const LoggerType&) override;
};

#endif //__LOGGER_FACTORY_H__
