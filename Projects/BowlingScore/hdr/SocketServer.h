#ifndef __SOCKET__SERVER__H__
#define __SOCKET__SERVER__H__

#include <sys/types.h> 
#include <string>
#include <string_view>

class SocketServer final
{
public:
	SocketServer();
	~SocketServer();

	void acceptClient();
	std::string readFromClient(std::string_view);
	void writeToClient(std::string_view) const;
	void closeClient() const;
	
private:
	void closeSocket(int) const;
	
	int m_sockfd = 0;
	int m_clisockfd = 0;
};

#endif // __SOCKET__SERVER__H__

