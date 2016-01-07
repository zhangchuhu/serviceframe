#ifndef DBCLIENT_DAEMON
#define DBCLIENT_DAEMON
#include "DaemonClient.h"

namespace core{

class DBClientDaemon :public DaemonClient{
	ILinkHandler *innerHandler;
public:
	DBClientDaemon();
	void setInnerHandler(ILinkHandler *h);
	
	virtual void start();
};
}
#endif

