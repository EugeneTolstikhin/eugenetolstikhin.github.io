#ifndef __SOCKET__CLIENT__H__
#define __SOCKET__CLIENT__H__

#include <sys/types.h> 
#include <string>

class SocketClient final
{
public:
	SocketClient(struct addrinfo);
	~SocketClient();
	
	std::string readFromSocket(const char*);
	void writeToSocket(const char*) const;
	
private:
	int m_sockfd = 0;
};

#endif // __SOCKET__CLIENT__H__