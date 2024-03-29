#ifndef __GAME_INITIALISER_H__
#define __GAME_INITIALISER_H__

#include <vector>
#include <string>
#include <memory>
#include "ILoggerFactory.h"

#include <csignal>

#ifdef __linux__
#include <unistd.h>
#include <sys/wait.h>
#endif

class GameInitialiser final
{
public:
    GameInitialiser();
    ~GameInitialiser() = default;

    GameInitialiser(const GameInitialiser&) = delete;
    GameInitialiser(GameInitialiser&&) = delete;
    GameInitialiser& operator = (const GameInitialiser&) = delete;
    GameInitialiser&& operator = (GameInitialiser&&) = delete;

    std::vector<std::string> Init();
private:
    const std::string HEADER = "Test";
	const std::string ANSWER = "Accepted";
    LoggerType m_typeLogger = LoggerType::TO_FILE;

    std::unique_ptr<ILoggerFactory> m_loggerFactory;
    std::unique_ptr<ILogger> m_log;
};

#endif // __GAME_INITIALISER_H__
