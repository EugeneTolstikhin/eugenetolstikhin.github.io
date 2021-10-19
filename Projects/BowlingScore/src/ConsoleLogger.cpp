#include "ConsoleLogger.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <stdarg.h>

ConsoleLogger::ConsoleLogger() 
{
    //
}

ConsoleLogger::~ConsoleLogger() 
{
    //
}

void ConsoleLogger::LogMe(const char* file, int line, const char* message, ...) 
{
    char buffer[256];
	va_list args;
	va_start (args, message);
	vsprintf (buffer, message, args);
	va_end (args);
    
    std::cout   << " File: " << file << std::endl
                << " Line: " << line << std::endl
                << " Message: " << buffer << std::endl
                << std::flush;
}
