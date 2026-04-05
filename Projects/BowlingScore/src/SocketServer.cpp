#include "SocketServer.h"
#include "SocketBasic.h"

#include <cstring>
#include <stdexcept>
#include <ranges>
#include <csignal>
#include <fcntl.h>
#include <span>
#include <vector>
#include <string>

#ifdef __linux__
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>
#elif defined _WIN32
#include <winsock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")
#endif

#ifdef __linux__
#define GETSOCKETERRNO() (errno)
#elif defined _WIN32
#define GETSOCKETERRNO() (WSAGetLastError())
#endif

namespace
{
std::string socket_error_message(const std::string_view action)
{
    return std::string(action) + ". Error code = " + std::to_string(GETSOCKETERRNO());
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

SocketServer::SocketServer() :

	/*************************************************************/
	/* Create an AF_INET stream socket to receive incoming       */
	/* connections on                                            */
	/*************************************************************/
	m_sockfd(socket(AF_INET, SOCK_STREAM, 0))
{
    if (m_sockfd < 0)
    {
        throw std::runtime_error("Cannot open socket");
    }

	/*************************************************************/
	/* Let the socket be reusable                                */
	/*************************************************************/
	int yes = 1;
	int err = setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, (void*)&yes, sizeof(yes));
	if (err < 0)
	{
		throw std::runtime_error(socket_error_message("ERROR on setsockopt"));
	}

	/*************************************************************/
	/* Bind the socket                                           */
	/*************************************************************/
	struct sockaddr_in serv_addr;
    memset((char*)&serv_addr, '\0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(SocketBasic::getPortServer());

	err = bind(m_sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (err < 0)
    {
        throw std::runtime_error(socket_error_message("ERROR on binding"));
    }

	/*************************************************************/
	/* Set the listen backlog                                   */
	/*************************************************************/
	err = listen(m_sockfd, SocketBasic::getPoolSize());
	if (err < 0)
    {
        throw std::runtime_error(socket_error_message("ERROR on listening"));
    }
}

SocketServer::~SocketServer()
{
	closeSocket(m_sockfd);
}

void SocketServer::acceptClient()
{
	struct sockaddr_in cli_addr;
	socklen_t clilen = sizeof(cli_addr);

	do
	{
		m_clisockfd = accept(m_sockfd, (struct sockaddr*)&cli_addr, &clilen);
	} while (m_clisockfd < 0);
}

std::string SocketServer::readFromClient(std::string_view secretKey)
{
	auto bufSize = SocketBasic::getBufferLength();
	std::vector<char> buf(bufSize + 1, '\0');
	int err = read(m_clisockfd, buf.data(), bufSize);
	if (err == 0)
	{
		throw std::runtime_error("Socket is closed for reading");
	}
    if (err < 0)
    {
        throw std::runtime_error("Cannot read from client socket");
    }

    std::string payload(buf.data(), static_cast<std::size_t>(err));
    if (std::string_view(payload).contains(secretKey))
    {
		return payload;
    }

	return {};
}

void SocketServer::writeToClient(std::string_view message) const
{
    write_all(m_clisockfd, message);
}

void SocketServer::closeClient() const
{
	closeSocket(m_clisockfd);
}

void SocketServer::closeSocket(int sockfd) const
{
#ifdef __linux__
    close(sockfd);
#elif defined _WIN32
	closesocket(sockfd);
#endif
}

