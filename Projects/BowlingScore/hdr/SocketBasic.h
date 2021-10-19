#ifndef __SOCKET__BASIC__H__
#define __SOCKET__BASIC__H__

#include <string>
#include "IConfigFactory.h"

class SocketBasic final
{
public:
	static void init(const char* configPath, const ConfigType& typeConfig);
	static const char* getHost();
	static const char* getPortClient();
	static uint16_t getPortServer();
	static int getPoolSize();
	static size_t getBufferLength();
private:
	SocketBasic() = default;
	~SocketBasic() = default;
	
	static uint16_t m_portServer;
	static size_t m_bufferLength;
	static int m_poolSize;
	static std::string m_portClient;
	static std::string m_host;
};

#endif // __SOCKET__BASIC__H__
