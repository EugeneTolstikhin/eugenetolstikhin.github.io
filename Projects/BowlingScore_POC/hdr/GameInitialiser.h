#ifndef __GAME_INITIALISER_H__
#define __GAME_INITIALISER_H__

#include <vector>
#include <string>
#include <memory>
#include "ILoggerFactory.h"

class GameInitialiser
{
public:
    GameInitialiser();
    ~GameInitialiser() = default;

    std::vector<std::string> Init();
private:
    LoggerType m_typeLogger = LoggerType::TO_FILE;

    std::unique_ptr<ILoggerFactory> m_loggerFactory;
    std::unique_ptr<ILogger> m_log;
};

#endif // __GAME_INITIALISER_H__
