#include "PointsListenerLocal.h"

#include "LoggerFactory.h"
#include "FileLogger.h"

#include "ICommon.h"

#include "SocketBasic.h"
#include "SocketClient.h"

#include <stdexcept>
#include <cstring>


PointsListenerLocal::PointsListenerLocal() :
    m_loggerFactory(std::make_unique<LoggerFactory>())
    ,m_log(m_loggerFactory->CreateLogger(m_typeLogger))
{
	struct addrinfo hints;
    memset(&hints, 0, (sizeof hints));
	hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

	int err = getaddrinfo(SocketBasic::getHost(), SocketBasic::getPortClient(), &hints, &m_addrs);
	if (err != 0)
    {
        throw std::runtime_error("Cannot get address information");
    }
	else if (m_addrs == nullptr)
	{
		throw std::runtime_error("Address info data is empty");
	}
}
    
PointsListenerLocal::~PointsListenerLocal()
{
    if (m_addrs) free(m_addrs);
}

unsigned short PointsListenerLocal::Receive()
{
	std::string buf;
	
	{
		std::unique_ptr<SocketClient> client(std::make_unique<SocketClient>(*m_addrs));
		client->readFromSocket(READY_MESSAGE.c_str());
		client->writeToSocket(CLIENT_TO_SERVER_MESSAGE.c_str());
		buf = client->readFromSocket(ANSWER_SECRET_KEY.c_str());
	}
	
	size_t spacePos = buf.rfind(" ");
	long numBuf = strtol(buf.substr(spacePos + 1).c_str(), nullptr, 10);

	if (numBuf < 0 || numBuf > MAX_POINTS)
	{
		throw std::out_of_range("Amount of points is out of range");
	}

    return static_cast<unsigned short>(numBuf);
}
