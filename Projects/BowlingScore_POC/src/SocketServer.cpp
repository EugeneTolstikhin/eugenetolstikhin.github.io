#include "SocketServer.h"
#include "SocketBasic.h"

#include <stdexcept>
#include <cstring>

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

SocketServer::SocketServer() :

	/*************************************************************/
	/* Create an AF_INET stream socket to receive incoming      */
	/* connections on                                            */
	/*************************************************************/
	m_sockfd(socket(AF_INET, SOCK_STREAM, 0))
{
    if (m_sockfd < 0)
    {
        throw std::runtime_error("Cannot open socket");
    }

	int err = 0;

	/*************************************************************/
	/* Allow socket descriptor to be reuseable                   */
	/*************************************************************/
	err = setsockopt(m_sockfd, SOL_SOCKET,  SO_REUSEADDR,
					(char *)&on, sizeof(on));
	if (err < 0)
	{
		throw std::runtime_error("setsockopt() failed");
	}

	/*************************************************************/
	/* Set socket to be nonblocking. All of the sockets for      */
	/* the incoming connections will also be nonblocking since   */
	/* they will inherit that state from the listening socket.   */
	/*************************************************************/
	err = ioctl(listen_sd, FIONBIO, (char *)&on);
	if (err < 0)
	{
		throw std::runtime_error("ioctl() failed");
	}

	struct sockaddr_in serv_addr;
    memset((char*)&serv_addr, '\0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(SocketBasic::getPortServer());

	/*************************************************************/
	/* Bind the socket                                           */
	/*************************************************************/
	err = bind(m_sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (err < 0)
    {
        throw std::runtime_error("ERROR on binding");
    }

	/*************************************************************/
	/* Set the listen back log                                   */
	/*************************************************************/
	err = listen(m_sockfd, SocketBasic::getPoolSize());
	if (err < 0)
    {
        throw std::runtime_error("SERVER ERROR on listening");
    }
}

SocketServer::~SocketServer()
{
#ifdef __linux__
    close(m_sockfd);
#elif defined _WIN32
	closesocket(m_sockfd);
#endif
}

void SocketServer::acceptClient()
{
	struct sockaddr_in cli_addr;
	struct timeval timeout;
	socklen_t clilen = sizeof(cli_addr);

	/*************************************************************/
	/* Initialize the timeval struct to 3 minutes.  If no        */
	/* activity after 3 minutes this program will end.           */
	/*************************************************************/
	timeout.tv_sec  = 3 * 60;
	timeout.tv_usec = 0;

	do
	{
		m_clisockfd = accept(m_sockfd, (struct sockaddr*)&cli_addr, &clilen);
	} while (m_clisockfd < 0);
}

std::string SocketServer::readFromClient(const char* secretKey)
{
	bool ready = false;
	auto bufSize = SocketBasic::getBufferLength();
	std::string res;

	char* buf = new char[bufSize];
	while(!ready)
	{
		memset(buf, '\0', bufSize);
		int err = read(m_clisockfd, buf, bufSize - 1);
		res = buf;
		if (err == 0)
		{
			delete[] buf;
			throw std::runtime_error("Socket is closed for reading");
		}
		else if (res.find(secretKey) != std::string::npos)
		{
			ready = true;
		}
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
#ifdef __linux__
    close(m_clisockfd);
#elif defined _WIN32
	closesocket(m_clisockfd);
#endif
}
