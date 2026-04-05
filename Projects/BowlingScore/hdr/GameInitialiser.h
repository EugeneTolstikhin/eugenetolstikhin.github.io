#ifndef __GAME_INITIALISER_H__
#define __GAME_INITIALISER_H__

#include <expected>
#include <future>
#include <thread>
#include <vector>
#include <string>
#include <string_view>
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
    using PlayerList = std::vector<std::string>;

    static std::expected<PlayerList, std::string> parsePlayers(std::string_view);

    static constexpr std::string_view HEADER = "Test";
	static constexpr std::string_view ANSWER = "Accepted";
    LoggerType m_typeLogger = LoggerType::TO_FILE;

    std::unique_ptr<ILoggerFactory> m_loggerFactory;
    std::unique_ptr<ILogger> m_log;
};

#endif // __GAME_INITIALISER_H__

