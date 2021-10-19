#include "SocketServer.h"
#include "SocketBasic.h"

#include <stdexcept>
#include <cstring>
#include <csignal>
#include <cstdio>
#include <fcntl.h>

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
		char error[100];
		sprintf(error, "ERROR on setsockopt. Error code = %d", GETSOCKETERRNO());
		throw std::runtime_error(error);
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
		char error[100];
		sprintf(error, "ERROR on binding. Error code = %d", GETSOCKETERRNO());
        throw std::runtime_error(error);
    }

	/*************************************************************/
	/* Set the listen backlog                                   */
	/*************************************************************/
	err = listen(m_sockfd, SocketBasic::getPoolSize());
	if (err < 0)
    {
		char error[100];
		sprintf(error, "ERROR on listening. Error code = %d", GETSOCKETERRNO());
        throw std::runtime_error(error);
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

std::string SocketServer::readFromClient(const char* secretKey)
{
	std::string res;
	auto bufSize = SocketBasic::getBufferLength();
	char* buf = new char[bufSize + 1];
	memset(buf, '\0', bufSize + 1);
	int err = read(m_clisockfd, buf, bufSize);
	if (err == 0)
	{
		delete[] buf;
		throw std::runtime_error("Socket is closed for reading");
	}
	else if (strstr(buf, secretKey) != nullptr)
	{
		res = buf;
	}
	
	delete[] buf;

	return res;
}

void SocketServer::writeToClient(const char* message) const
{
	int err = write(m_clisockfd, message, strlen(message));
    if (err <= 0)
    {
        throw std::runtime_error("Cannot write to socket");
    }
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
