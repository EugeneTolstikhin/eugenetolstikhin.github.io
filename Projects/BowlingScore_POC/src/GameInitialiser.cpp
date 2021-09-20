#include "GameInitialiser.h"
#include "LoggerFactory.h"
#include "FileLogger.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

GameInitialiser::GameInitialiser() :
    m_loggerFactory(new LoggerFactory)
    ,m_log(m_loggerFactory->CreateLogger(m_typeLogger))
{
    //
}

//TODO: Create different thread for communication with admin
std::vector<std::string> GameInitialiser::Init()
{
    m_log->LogMe(__FILE__, __LINE__, "Init");
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    std::vector<std::string> players = {"A", "B"};

    m_log->LogMe(__FILE__, __LINE__, "Open socket");
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        m_log->LogMe(__FILE__, __LINE__, "ERROR opening socket");
        return std::move(players);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));

    portno = 8888;

    serv_addr.sin_family = AF_INET;  
    serv_addr.sin_addr.s_addr = INADDR_ANY;  
    serv_addr.sin_port = htons(portno);

    m_log->LogMe(__FILE__, __LINE__, "Bind socket");
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        m_log->LogMe(__FILE__, __LINE__, "ERROR on binding");
        return std::move(players);
    }

    m_log->LogMe(__FILE__, __LINE__, "Listen to socket");
    listen(sockfd,5);

    clilen = sizeof(cli_addr);

    m_log->LogMe(__FILE__, __LINE__, "Accept new connection");
    while ((newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)) >= 0)
    {
        bzero(buffer,256);

        m_log->LogMe(__FILE__, __LINE__, "Read data");
        n = read(newsockfd, buffer, 255);
        if (n < 0)
        {
            m_log->LogMe(__FILE__, __LINE__, "ERROR reading from socket");
            return std::move(players);
        }
        else if (buffer[0] =='1')
        {
            m_log->LogMe(__FILE__, __LINE__, std::string("Buffer = ") + std::string(buffer));
            send(newsockfd, "Accepted", strlen(buffer), 0);

            //TODO: Pass this value to the main thread
            // yield, etc.

            m_log->LogMe(__FILE__, __LINE__, "Close connection 1");
            close(newsockfd);
            break;

        }

        m_log->LogMe(__FILE__, __LINE__, std::string("Buffer = ") + std::string(buffer));

        m_log->LogMe(__FILE__, __LINE__, "Close connection 2");
        close(newsockfd);
    }

    m_log->LogMe(__FILE__, __LINE__, "Close main socket");
    close(sockfd);

    return std::move(players);
}