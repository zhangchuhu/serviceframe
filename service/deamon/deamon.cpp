#include "WrapServerStart.h"
#include "server_common/server-lib/ConfigDaemonServer.h"

int main(int sz, char *args[]) 
{
	WrapServerStart::init();
	ConfigDaemonServer __server;
	 __server.startSV();
	WrapServerStart::run();
	return 0;
}
