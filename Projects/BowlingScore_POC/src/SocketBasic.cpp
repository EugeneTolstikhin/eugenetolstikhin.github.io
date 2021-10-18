#include "SocketBasic.h"
#include "ConfigFactory.h"
#include "ConfigFile.h"

#include <memory>

size_t SocketBasic::m_bufferLength = 0;
uint16_t SocketBasic::m_portServer = 0;
int SocketBasic::m_poolSize = 0;
std::string SocketBasic::m_portClient("");
std::string SocketBasic::m_host("");

const char* SocketBasic::getHost()
{
    return m_host.c_str();
}

const char* SocketBasic::getPortClient()
{
    return m_portClient.c_str();
}

uint16_t SocketBasic::getPortServer()
{
	return m_portServer;
}

size_t SocketBasic::getBufferLength()
{
    return m_bufferLength;
}

int SocketBasic::getPoolSize()
{
    return m_poolSize;
}

void SocketBasic::init(const char* configPath, const ConfigType& typeConfig)
{
	std::unique_ptr<IConfigFactory> m_configFactory(new ConfigFactory);
	std::unique_ptr<IConfig> m_config(m_configFactory->CreateConfig(typeConfig));
	
	m_config->initPath(configPath);

	// Parse config data
	m_bufferLength = m_config->getUIntValue("BUFFER_LENGTH");
	m_portClient = m_config->getStringValue("PORT_CLIENT");
	m_portServer = m_config->getUIntValue("PORT_SERVER");
	m_host = m_config->getStringValue("HOST");
	m_poolSize = m_config->getUIntValue("POOL_SIZE");
}
