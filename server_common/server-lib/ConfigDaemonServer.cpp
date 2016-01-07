#include "ConfigDaemonServer.h"
#include "core/sox/logger.h"
using namespace core;
using namespace server::config;

void ConfigDaemonServer::onConfigRefresh(){
	setName(getServerConfig()->getName());
	refreshPort(getServerConfig()->getPort());
}

