#include "FileLogger.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <stdarg.h>
#include <cstdio>

FileLogger::FileLogger() : log("log.txt", std::ios::app)
{
    if (!log)
    {
        std::cerr << "Error opening log file" << std::endl;
    }
}

FileLogger::~FileLogger() 
{
    log.close();
}

void FileLogger::LogMe(const char* file, int line, const char* message, ...) 
{
    char buffer[256];
	va_list args;
	va_start (args, message);
	vsprintf (buffer, message, args);
	va_end (args);

    log << " File: " << file << std::endl
        << " Line: " << line << std::endl
        << " Message: " << buffer << std::endl
        << std::fflush;
}
