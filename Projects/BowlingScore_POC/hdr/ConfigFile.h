#ifndef __CONFIG__FILE__H__
#define __CONFIG__FILE__H__

#include "IConfig.h"
#include <fstream>
#include <map>
#include <string>

class ConfigFile : public IConfig
{
public:
	ConfigFile();
	ConfigFile(const char* filename);
    virtual ~ConfigFile();

    ConfigFile(const ConfigFile&) = delete;
    ConfigFile(ConfigFile&&) = delete;
    ConfigFile& operator = (const ConfigFile&) = delete;
    ConfigFile&& operator = (ConfigFile&&) = delete;

	virtual void initPath(const char* path) override;
	virtual size_t getUIntValue(const char* paramName) const override;
	virtual std::string getStringValue(const char* paramName) const override;
	
private:
	std::ifstream file;
	std::map<std::string, std::string> fileContent;
};

#endif // __CONFIG__FILE__H__