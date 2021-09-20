#ifndef __POINTS_LISTENER_H__
#define __POINTS_LISTENER_H__

#include "IPointsListener.h"
#include "ILoggerFactory.h"

#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <memory>

class PointsListenerLocal : public IPointsListener
{
public:
    PointsListenerLocal();
    virtual ~PointsListenerLocal();

    virtual void Connect() override;
    virtual unsigned short Receive() override;
    virtual void Shutdown() override;

private:
    static constexpr int BUFFER_SIZE = 256;

    std::unique_ptr<ILoggerFactory> m_loggerFactory;
    std::unique_ptr<ILogger> m_log;

    LoggerType m_typeLogger = LoggerType::TO_FILE;

    const int BACKLOG_SIZE = 5;
    const unsigned short PORT = 8080;
    const std::string HOSTNAME = "127.0.0.1";
    const int PROTOCOL = 0;
    const std::string CLIENT_TO_SERVER_MESSAGE = "Wait for points";

    char buf[BUFFER_SIZE];
    struct sockaddr_in serv_addr;
    int sockfd;
    struct hostent *server;
};

#endif //__POINTS_LISTENER_H__
