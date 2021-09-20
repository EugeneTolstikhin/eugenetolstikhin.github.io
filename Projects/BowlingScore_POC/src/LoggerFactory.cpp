#include "LoggerFactory.h"
#include "ConsoleLogger.h"
#include "FileLogger.h"
#include <stdexcept>
#include <stdarg.h>

LoggerFactory::LoggerFactory()
{
    //
}

LoggerFactory::~LoggerFactory()
{
    //
}

ILogger* LoggerFactory::CreateLogger(const LoggerType& type)
{
    switch (type)
    {
        case LoggerType::CLS:
            return new ConsoleLogger;
        case LoggerType::TO_FILE:
            return new FileLogger;
        case LoggerType::NETWORK:
            //TODO: return the proper instance of class here when it will be implemented
            throw std::runtime_error("Unimplemented type of Logger");
        default:
            throw std::out_of_range("Unknown type of Logger");
    }
    
    return nullptr;
}
