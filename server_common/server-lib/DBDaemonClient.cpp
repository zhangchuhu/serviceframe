#include "DBDaemonClient.h"

using namespace core;
using namespace sdaemon;


DBClientDaemon::DBClientDaemon(){
	innerHandler = NULL;
}

void DBClientDaemon::start(){
	realStart(innerHandler);
}

void DBClientDaemon::setInnerHandler(ILinkHandler *h){
	innerHandler = h;
}
