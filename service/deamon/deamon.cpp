#include "server_common/helper/imdb_inc.h"

int main(int sz, char *args[]) 
{
	WrapServerStart::init();
	DAEMON_SERVER_START
		WrapServerStart::run();
	return 0;
}