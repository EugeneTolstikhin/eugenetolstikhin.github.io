#include "PointsListenerLocal.h"
#include "LoggerFactory.h"
#include "FileLogger.h"

#include <stdexcept>
#include <cstring>


PointsListenerLocal::PointsListenerLocal() :
    m_loggerFactory(new LoggerFactory)
    ,m_log(m_loggerFactory->CreateLogger(m_typeLogger))
{
    //
}
    
PointsListenerLocal::~PointsListenerLocal()
{
    close(sockfd);
}

unsigned short PointsListenerLocal::Receive()
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        return -1;
    }

    server = gethostbyname(HOSTNAME.c_str());
    if (server == NULL)
    {
        return -2;
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
        (char *)&serv_addr.sin_addr.s_addr,
        server->h_length);
    serv_addr.sin_port = htons(PORT);
    
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        return -3;
    }

    if (write(sockfd, CLIENT_TO_SERVER_MESSAGE.c_str(), CLIENT_TO_SERVER_MESSAGE.length()) < 0)
    {
        return -4;
    }

    bzero(buf, BUFFER_SIZE);
    auto err = read(sockfd, buf, BUFFER_SIZE - 1);
    if (err < 0)
    {
        return -5;
    }

    int numBuf = atoi(buf);

    if (numBuf < 0 || numBuf > 10)
    {
        return -6;
    }

    close(sockfd);

    return static_cast<unsigned short>(numBuf);
}
