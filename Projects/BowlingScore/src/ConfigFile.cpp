#include "ConfigFile.h"
#include <charconv>
#include <cctype>
#include <expected>
#include <algorithm>
#include <string_view>

namespace
{
std::string trim_copy(std::string_view value)
{
    auto begin = value.begin();
    auto end = value.end();

    while (begin != end && std::isspace(static_cast<unsigned char>(*begin)))
    {
        ++begin;
    }

    while (begin != end && std::isspace(static_cast<unsigned char>(*(end - 1))))
    {
        --end;
    }

    return std::string(begin, end);
}

using ConfigEntry = std::pair<std::string, std::string>;

std::expected<ConfigEntry, std::string> parse_config_line(std::string_view line)
{
    const auto pos = line.find(':');
    if (pos == std::string_view::npos)
    {
        return std::unexpected("Missing ':' delimiter in config line");
    }

    auto key = trim_copy(line.substr(0, pos));
    auto value = trim_copy(line.substr(pos + 1));

    if (key.empty())
    {
        return std::unexpected("Config key cannot be empty");
    }

    return ConfigEntry{std::move(key), std::move(value)};
}
}

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
	size_t res = 0;
    const auto& value = fileContent.at(paramName);
    const auto [ptr, ec] = std::from_chars(value.data(), value.data() + value.size(), res);
    if (ec != std::errc{} || ptr != value.data() + value.size())
    {
        throw std::runtime_error("Invalid unsigned integer value in config");
    }

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
		std::string line;
		while (getline(file, line))
		{
            const auto parsedLine = parse_config_line(line);
            if (!parsedLine.has_value())
            {
                continue;
            }

            auto [key, value] = *parsedLine;
            fileContent.insert_or_assign(std::move(key), std::move(value));
		}
	}
	else
	{
		throw std::runtime_error("Cannot open file");
	}
}

