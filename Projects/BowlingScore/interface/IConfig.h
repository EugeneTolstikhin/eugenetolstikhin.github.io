#ifndef __ICONFIG_H__
#define __ICONFIG_H__

#include <string>

class IConfig
{
public:
    IConfig() = default;
    virtual ~IConfig() = default;

    IConfig(const IConfig&) = delete;
    IConfig(IConfig&&) = delete;
    IConfig& operator = (const IConfig&) = delete;
    IConfig&& operator = (IConfig&&) = delete;

	virtual void initPath(const char* path) = 0;
	virtual size_t getUIntValue(const char* paramName) const = 0;
	virtual std::string getStringValue(const char* paramName) const = 0;
};

#endif // __ICONFIG_H__