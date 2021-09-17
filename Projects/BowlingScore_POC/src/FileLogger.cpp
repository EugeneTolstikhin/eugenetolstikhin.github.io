#include "FileLogger.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <stdarg.h>

FileLogger::FileLogger() : log("log.txt", std::ios::app)
{
    if (!log)
    {
        std::cout << "Error opening log file" << std::endl;
    }
}

FileLogger::~FileLogger() 
{
    log.close();
}

void FileLogger::LogMe(const std::string& file, int line, const std::string& message, ...) 
{
    va_list args;
    std::istringstream iss(message);
    std::string str;
    std::vector<std::string> tokens;
    int count = 0;

    while (iss >> str)
    {
        if (str.find("%") != -1)
        {
            if (str.length() > 1)
            {
                ++count;
            }
            else
            {
                throw std::runtime_error("Invalid token found");
            }
        }

        tokens.push_back(std::move(str));
    }

    std::string s;
    va_start(args, count);
    for (auto& val: tokens)
    {
        if (val.find("%") != std::string::npos)
        {
            auto sub = str.substr(1);
            if (sub.find("s") != std::string::npos)
            {
                s += va_arg(args, const char *);
            }
            else if (sub.find("d") != std::string::npos)
            {
                s += va_arg(args, int);
            }
            else if (sub.find("f") != std::string::npos)
            {
                s += va_arg(args, double);
            }
            else if (sub.find("%") != std::string::npos)
            {
                s += "%";
            }
            /* And so on */
        }
        else
        {
            s += val;
        }

        s += " ";
    }

    va_end(args);

    log << " File: " << file << std::endl
        << " Line: " << line << std::endl
        << " Message: " << s << std::endl
        << std::fflush;
}
