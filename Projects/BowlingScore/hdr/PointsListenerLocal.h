#ifndef __POINTS_LISTENER_H__
#define __POINTS_LISTENER_H__

#include "IPointsListener.h"
#include "SocketClient.h"
#include "ILoggerFactory.h"

#ifdef __linux__
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#elif defined _WIN32
#include <winsock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")
#endif

#include <sys/types.h> 
#include <memory>
#include <charconv>

class PointsListenerLocal : public IPointsListener
{
public:
    PointsListenerLocal();
    virtual ~PointsListenerLocal();

    PointsListenerLocal(const PointsListenerLocal&) = delete;
    PointsListenerLocal(PointsListenerLocal&&) = delete;
    PointsListenerLocal& operator = (const PointsListenerLocal&) = delete;
    PointsListenerLocal&& operator = (PointsListenerLocal&&) = delete;

    virtual unsigned short Receive() override;

private:
    LoggerType m_typeLogger = LoggerType::TO_FILE;
	std::unique_ptr<ILoggerFactory> m_loggerFactory;
    std::unique_ptr<ILogger> m_log;

    const int PROTOCOL = 0;
    const std::string CLIENT_TO_SERVER_MESSAGE = "Give me the points!";
	const std::string READY_MESSAGE = "Wait for command!";
	const std::string ANSWER_SECRET_KEY = "I give you: ";

    using AddrInfoPtr = std::unique_ptr<struct addrinfo, decltype(&freeaddrinfo)>;

    static unsigned short parseScore(std::string_view);

	AddrInfoPtr m_addrs{nullptr, freeaddrinfo};
};

#endif //__POINTS_LISTENER_H__

