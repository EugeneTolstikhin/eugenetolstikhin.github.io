#ifndef __FILE_LOGGER_H__
#define __FILE_LOGGER_H__

#include "ILogger.h"
#include <fstream>

class FileLogger : public ILogger
{
public:
    FileLogger();
    virtual ~FileLogger();

    virtual void LogMe(const std::string&, int, const std::string&);

private:
    std::ofstream log;
};

#endif //__FILE_LOGGER_H__
