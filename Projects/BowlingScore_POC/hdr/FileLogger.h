#ifndef __FILE_LOGGER_H__
#define __FILE_LOGGER_H__

#include "ILogger.h"
#include <fstream>

class FileLogger : public ILogger
{
public:
    FileLogger();
    virtual ~FileLogger();

    FileLogger(const FileLogger&) = delete;
    FileLogger(FileLogger&&) = delete;
    FileLogger& operator = (const FileLogger&) = delete;
    FileLogger&& operator = (FileLogger&&) = delete;

    virtual void LogMe(const std::string&, int, const std::string&, ...) override;

private:
    std::ofstream log;
};

#endif //__FILE_LOGGER_H__
