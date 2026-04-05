#ifndef __SOCKET__CLIENT__H__
#define __SOCKET__CLIENT__H__

#include <sys/types.h> 
#include <string>
#include <string_view>

class SocketClient final
{
public:
	SocketClient(const struct addrinfo&);
	~SocketClient();
	
	std::string readFromSocket(std::string_view);
	void writeToSocket(std::string_view) const;
	
private:
	int m_sockfd = 0;
};

#endif // __SOCKET__CLIENT__H__

