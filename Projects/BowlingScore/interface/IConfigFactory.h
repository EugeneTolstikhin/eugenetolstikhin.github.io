#ifndef __ICONFIG_FACTORY_H__
#define __ICONFIG_FACTORY_H__

#include <memory>

enum class ConfigType
{
    FILE,
    DB
};

class IConfig;
class IConfigFactory
{
public:
    IConfigFactory() = default;
    virtual ~IConfigFactory() = default;

    virtual std::unique_ptr<IConfig> CreateConfig(const ConfigType&) = 0;
};

#endif //__ICONFIG_FACTORY_H__

