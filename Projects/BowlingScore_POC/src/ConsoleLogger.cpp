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

void ConsoleLogger::LogMe(const std::string& file, int line, const std::string& message) 
{
    std::cout   << "File: " << file << std::endl
                << "Line: " << line << std::endl
                << "Message: " << message << std::endl
                << std::flush;
}
