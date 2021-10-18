#include "ConfigFactory.h"
#include "ConfigFile.h"
#include <stdexcept>
#include <stdarg.h>

ConfigFactory::ConfigFactory()
{
    //
}

ConfigFactory::~ConfigFactory()
{
    //
}

IConfig* ConfigFactory::CreateConfig(const ConfigType& type)
{
    switch (type)
    {
        case ConfigType::FILE:
            return new ConfigFile;
        case ConfigType::DB:
            //TODO: return the proper instance of class here when it will be implemented
            throw std::runtime_error("Unimplemented type of Config");
        default:
            throw std::out_of_range("Unknown type of Config");
    }
    
    return nullptr;
}
