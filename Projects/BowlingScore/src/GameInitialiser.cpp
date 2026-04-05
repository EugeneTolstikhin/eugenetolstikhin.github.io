#include "GameInitialiser.h"
#include "LoggerFactory.h"
#include "FileLogger.h"
#include "SocketServer.h"

#include <cctype>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <exception>
#include <ranges>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <vector>

GameInitialiser::GameInitialiser() :
    m_loggerFactory(std::make_unique<LoggerFactory>())
    ,m_log(m_loggerFactory->CreateLogger(m_typeLogger))
{
	//
}

//TODO: Create different thread for communication with admin
std::vector<std::string> GameInitialiser::Init()
{
    std::promise<PlayerList> playersPromise;
    auto playersFuture = playersPromise.get_future();

	std::jthread acceptWorker([promise = std::move(playersPromise)](std::stop_token) mutable
    {
        try
        {
            SocketServer server;
	        while (true)
	        {
		        server.acceptClient();
		        const auto payload = server.readFromClient(HEADER);

		        if (payload.empty())
                {
		            server.closeClient();
                    continue;
                }

                server.writeToClient(ANSWER);
                auto parsedPlayers = parsePlayers(payload);
		        server.closeClient();

                if (!parsedPlayers.has_value() || parsedPlayers->empty())
                {
                    continue;
                }

                promise.set_value(std::move(*parsedPlayers));
                return;
	        }
        }
        catch (...)
        {
            promise.set_exception(std::current_exception());
        }
    });

    return playersFuture.get();
}

std::expected<GameInitialiser::PlayerList, std::string> GameInitialiser::parsePlayers(const std::string_view payload)
{
    PlayerList tokens;
    for (auto&& part : payload | std::views::split(' '))
    {
        std::string token;
        for (const auto ch : part)
        {
            if (!std::isspace(static_cast<unsigned char>(ch)))
            {
                token.push_back(static_cast<char>(ch));
            }
        }

        if (!token.empty())
        {
            tokens.push_back(std::move(token));
        }
    }

    if (tokens.empty() || tokens.front() != HEADER)
    {
        return std::unexpected("Missing admin handshake header");
    }

    tokens.erase(tokens.begin());
    return tokens;
}

