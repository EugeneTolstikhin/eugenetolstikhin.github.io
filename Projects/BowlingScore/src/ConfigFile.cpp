#include "ConfigFile.h"
#include <sstream>
#include <algorithm>

ConfigFile::ConfigFile()
{
	//
}

ConfigFile::~ConfigFile()
{
	if (file.is_open())
	{
		file.close();
	}
}

size_t ConfigFile::getUIntValue(const char* paramName) const
{
	size_t res;
	std::istringstream ( fileContent.at(paramName) ) >> res;
	return res;
}

std::string ConfigFile::getStringValue(const char* paramName) const
{
	return fileContent.at(paramName);
}

void ConfigFile::initPath(const char* path)
{
	file.open(path);
	if (file.is_open())
	{
		auto ltrim = [](std::string &s)
		{
			s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
				return !std::isspace(ch);
			}));
		};

		auto rtrim = [](std::string &s)
		{
			s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
				return !std::isspace(ch);
			}).base(), s.end());
		};

		std::string line;
		while ( getline (file, line) )
		{
			auto pos = line.find(":");
			if (pos != std::string::npos)
			{
				std::string param = line.substr(0, pos);
				ltrim(param);
				rtrim(param);

				std::string value = line.substr(pos + 1);
				ltrim(value);
				rtrim(value);

				auto res = fileContent.insert ( std::pair<std::string, std::string>(param, value) );
				if (res.second == false)
				{
					//
				}
			}
		}
	}
	else
	{
		throw std::runtime_error("Cannot open file");
	}
}