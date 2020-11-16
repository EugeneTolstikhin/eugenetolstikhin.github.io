#include "ConsoleLogger.h"
#include <iostream>

ConsoleLogger::ConsoleLogger() 
{
    //
}

ConsoleLogger::~ConsoleLogger() 
{
    //
}

void ConsoleLogger::LogMe(const std::string& file, int line, const std::string& message) const noexcept
{
    std::cout   << "File: " << file << std::endl
                << "Line: " << line << std::endl
                << "Messsage: " << message << std::endl
                << std::flush;
}
