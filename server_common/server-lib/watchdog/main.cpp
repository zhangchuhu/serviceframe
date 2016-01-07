#include "server_common/helper/main_inc.h"
#include "server_common/server-lib/id_def.h"
#include "WatchDog.h"
#include "ImTest.h"

using namespace server;
using namespace server::config;

int main(int sz, char *args[]) {
	init_daemon dm(sz, args);
	WrapServerStart::init();
    
    CONFIG_RDAEMON_SERVER_INIT("watchdog",
            CWatchDog,
            MultiConnManagerImp,
            InnerConnCreator,
            InnerConnCreator,
            BackLinkHandler,
            RouteWriter,
            TinyXmlServerConfigImp);

    //init watchdog
	CWatchDog watchdog;
    watchdog.setDaemonClient(&__clientDaemon);
	watchdog.setServer(&__server);
    watchdog.setWriter(&__writer);

    //server unit test
    CImTest im;
    im.setServer(&__server);
    
    //setup service wrapper
    watchdog.ADD_SERVICE_WRAPPER(SUFFIX_IMDB, im, &CImTest::OnTestList);

    //setup uri response wrapper if needed
    //watchdog.ADD_RESPONSE_WRAPPER(PSS_FillBuddyListRes, im, &CImTest::OnTestListRes);

	DAEMON_SERVER_START
	WrapServerStart::run();
}
