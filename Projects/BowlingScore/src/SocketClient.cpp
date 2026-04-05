#include "SocketClient.h"
#include "SocketBasic.h"

#include <stdexcept>
#include <ranges>
#include <span>
#include <vector>

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

namespace
{
void close_socket_handle(const int sockfd)
{
#ifdef __linux__
	close(sockfd);
#elif defined _WIN32
	closesocket(sockfd);
#endif
}

void write_all(const int sockfd, std::string_view message)
{
    auto remaining = std::span{message.data(), message.size()};
    while (!remaining.empty())
    {
        const auto written = write(sockfd, remaining.data(), remaining.size());
        if (written <= 0)
        {
            throw std::runtime_error("Cannot write to socket");
        }

        remaining = remaining.subspan(static_cast<std::size_t>(written));
    }
}
}

SocketClient::SocketClient(const struct addrinfo& addrs)
{
	const auto* current = &addrs;
    while (current != nullptr)
	{
        m_sockfd = socket(current->ai_family, current->ai_socktype, current->ai_protocol);
        if (m_sockfd <= 0)
        {
            current = current->ai_next;
            continue;
        }

        if (connect(m_sockfd, current->ai_addr, current->ai_addrlen) == 0)
        {
            return;
        }

        close_socket_handle(m_sockfd);
        m_sockfd = 0;
        current = current->ai_next;
	}

    throw std::runtime_error("Cannot connect to socket");
}

SocketClient::~SocketClient()
{
    if (m_sockfd > 0)
    {
	    close_socket_handle(m_sockfd);
    }
}

std::string SocketClient::readFromSocket(std::string_view secretKey)
{
	auto bufSize = SocketBasic::getBufferLength();
	std::vector<char> buf(bufSize, '\0');

	while (true)
	{
		std::ranges::fill(buf, '\0');
		const auto err = read(m_sockfd, buf.data(), bufSize - 1);
		if (err == 0)
		{
			throw std::runtime_error("Socket is closed for reading");
		}
        if (err < 0)
        {
            throw std::runtime_error("Cannot read from socket");
        }

        std::string response(buf.data(), static_cast<std::size_t>(err));
		if (std::string_view(response).contains(secretKey))
		{
			return response;
		}
	}
}

void SocketClient::writeToSocket(std::string_view message) const
{
    write_all(m_sockfd, message);
}

