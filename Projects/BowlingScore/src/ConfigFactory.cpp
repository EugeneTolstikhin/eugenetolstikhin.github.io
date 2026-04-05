#include "ConfigFactory.h"
#include "ConfigFile.h"
#include <stdexcept>

ConfigFactory::ConfigFactory()
{
    //
}

ConfigFactory::~ConfigFactory()
{
    //
}

std::unique_ptr<IConfig> ConfigFactory::CreateConfig(const ConfigType& type)
{
    switch (type)
    {
        case ConfigType::FILE:
            return std::make_unique<ConfigFile>();
        case ConfigType::DB:
            //TODO: return the proper instance of class here when it will be implemented
            throw std::runtime_error("Unimplemented type of Config");
        default:
            throw std::out_of_range("Unknown type of Config");
    }
    
    return {};
}

