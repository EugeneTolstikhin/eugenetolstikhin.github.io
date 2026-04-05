#include "PointsListenerLocal.h"

#include "LoggerFactory.h"
#include "FileLogger.h"

#include "ICommon.h"

#include "SocketBasic.h"
#include "SocketClient.h"

#include <charconv>
#include <stdexcept>
#include <cstring>
#include <string_view>


PointsListenerLocal::PointsListenerLocal() :
    m_loggerFactory(std::make_unique<LoggerFactory>())
    ,m_log(m_loggerFactory->CreateLogger(m_typeLogger))
{
	struct addrinfo hints;
    memset(&hints, 0, (sizeof hints));
	hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    struct addrinfo* rawAddrs = nullptr;
	int err = getaddrinfo(SocketBasic::getHost(), SocketBasic::getPortClient(), &hints, &rawAddrs);
	if (err != 0)
    {
        throw std::runtime_error("Cannot get address information");
    }
    m_addrs.reset(rawAddrs);

	if (m_addrs == nullptr)
	{
		throw std::runtime_error("Address info data is empty");
	}
}
    
PointsListenerLocal::~PointsListenerLocal() = default;

unsigned short PointsListenerLocal::Receive()
{
	SocketClient client(*m_addrs);
	client.readFromSocket(READY_MESSAGE);
	client.writeToSocket(CLIENT_TO_SERVER_MESSAGE);
    return parseScore(client.readFromSocket(ANSWER_SECRET_KEY));
}

unsigned short PointsListenerLocal::parseScore(const std::string_view response)
{
    const auto spacePos = response.rfind(' ');
    if (spacePos == std::string_view::npos || spacePos + 1 >= response.size())
    {
        throw std::runtime_error("Cannot parse points from listener response");
    }

    unsigned short score = 0;
    auto numericPart = response.substr(spacePos + 1);
    while (!numericPart.empty())
    {
        const auto trailing = numericPart.back();
        if (trailing != '\0' && trailing != '\n' && trailing != '\r' && trailing != ' ' && trailing != '\t')
        {
            break;
        }

        numericPart.remove_suffix(1);
    }

    const auto [ptr, ec] = std::from_chars(numericPart.data(), numericPart.data() + numericPart.size(), score);
    if (ec != std::errc{} || ptr != numericPart.data() + numericPart.size() || score > MAX_POINTS)
    {
        throw std::out_of_range("Amount of points is out of range");
    }

    return score;
}

