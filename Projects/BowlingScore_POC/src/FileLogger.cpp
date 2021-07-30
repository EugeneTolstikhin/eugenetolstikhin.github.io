#include "FileLogger.h"
#include <iostream>

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

void FileLogger::LogMe(const std::string& file, int line, const std::string& message) 
{
    log << "File: " << file << std::endl
        << "Line: " << line << std::endl
        << "Message: " << message << std::endl
        << std::fflush;
}
