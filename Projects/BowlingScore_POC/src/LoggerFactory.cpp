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
            std::string error("Unknown type of Logger: ");
            error += std::to_string(static_cast<int>(type));
            throw std::out_of_range(error);
    }
    
    return nullptr;
}
