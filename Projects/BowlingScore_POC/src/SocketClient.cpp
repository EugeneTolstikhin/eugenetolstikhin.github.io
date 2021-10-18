#include "SocketClient.h"
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

SocketClient::SocketClient(struct addrinfo addrs) :
	m_sockfd(socket(addrs.ai_family, addrs.ai_socktype, addrs.ai_protocol))
{
	while (m_sockfd <= 0)
	{
		if (addrs.ai_next != nullptr)
        {
			addrs = *addrs.ai_next;
			m_sockfd = socket(addrs.ai_family, addrs.ai_socktype, addrs.ai_protocol);
        }
        else
        {
        	throw std::runtime_error("ERROR no proper socket found");
        }
	}

	int err = connect(m_sockfd, addrs.ai_addr, addrs.ai_addrlen);
    if (err != 0)
    {
        throw std::runtime_error("Cannot connect to socket");
    }
}

SocketClient::~SocketClient()
{
#ifdef __linux__
	close(m_sockfd);
#elif defined _WIN32
	closesocket(m_sockfd);
#endif
}

std::string SocketClient::readFromSocket(const char* secretKey)
{
	bool ready = false;
	auto bufSize = SocketBasic::getBufferLength();
	std::string res;

	char* buf = new char[bufSize];
	while(!ready)
	{
		memset(buf, '\0', bufSize);
		int err = read(m_sockfd, buf, bufSize - 1);
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

void SocketClient::writeToSocket(const char* message) const
{
	int err = write(m_sockfd, message, strlen(message));
    if (err <= 0)
    {
        throw std::runtime_error("Cannot write to socket");
    }
}
