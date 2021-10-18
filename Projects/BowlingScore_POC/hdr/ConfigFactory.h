#ifndef __CONFIG_FACTORY_H__
#define __CONFIG_FACTORY_H__

#include "IConfigFactory.h"

class ConfigFactory : public IConfigFactory
{
public:
    ConfigFactory();
    virtual ~ConfigFactory();

    ConfigFactory(const ConfigFactory&) = delete;
    ConfigFactory(ConfigFactory&&) = delete;
    ConfigFactory& operator = (const ConfigFactory&) = delete;
    ConfigFactory&& operator = (ConfigFactory&&) = delete;

    virtual IConfig* CreateConfig(const ConfigType&) override;
};

#endif //__CONFIG_FACTORY_H__
