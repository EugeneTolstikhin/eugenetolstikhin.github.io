#ifndef __SOCKET__SERVER__H__
#define __SOCKET__SERVER__H__

#include <sys/types.h> 
#include <string>

class SocketServer final
{
public:
	SocketServer();
	~SocketServer();

	void acceptClient();
	std::string readFromClient(const char*);
	void writeToClient(const char*) const;
	void closeClient() const;
	
private:
	int m_sockfd = 0;
	int m_clisockfd = 0;
};

#endif // __SOCKET__SERVER__H__