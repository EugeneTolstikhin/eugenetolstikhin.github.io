#ifndef __ICONFIG_FACTORY_H__
#define __ICONFIG_FACTORY_H__

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

    virtual IConfig* CreateConfig(const ConfigType&) = 0;
};

#endif //__ICONFIG_FACTORY_H__