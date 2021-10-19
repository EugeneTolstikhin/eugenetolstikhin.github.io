#include "GameInitialiser.h"
#include "LoggerFactory.h"
#include "FileLogger.h"
#include "SocketServer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <vector>
#include <thread>

GameInitialiser::GameInitialiser() :
    m_loggerFactory(new LoggerFactory)
    ,m_log(m_loggerFactory->CreateLogger(m_typeLogger))
{
	//
}

//TODO: Create different thread for communication with admin
std::vector<std::string> GameInitialiser::Init()
{
	std::string buf;
	std::vector<std::string> players;
	std::unique_ptr<SocketServer> server(new SocketServer);
	while(true)
	{
		server->acceptClient();
		buf = server->readFromClient(HEADER.c_str());

		if (buf.empty()) continue;

		std::istringstream iss(buf);
        std::string str;
        bool headerReceived = false;

        while (iss >> str)
        {
            if (str == HEADER)
            {
                headerReceived = true;
                server->writeToClient(ANSWER.c_str());
                continue;
            }

            if (headerReceived)
            {
                players.push_back(str);
            }
        }

		server->closeClient();
	}

    return std::move(players);
}