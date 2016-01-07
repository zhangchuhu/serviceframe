#include "server_common/helper/main_inc.h"
#include "core/sox/logger.h"

#include "ImLbsRequest.h"
#include "server_common/helper/unixdaemon.h"
#include "core/corelib/MultiConnManagerVecImp.h"
#include "UdpPing.h"
#include "../imlib/WatchDog.h"
#include "P2PLbs.h"

namespace{
	#define IMLBSD_TIMEOUT (2 * 60 * 1000)

	class LbsConnCreator: public IServerConnCreator{
	public:
		LbsConnCreator(){}
		virtual IConn *creatConnect(SOCKET so, uint32_t ip, int port, ILinkHandler *ih, ILinkEvent *ie, CreateCallback *cb){
			InnerConn *conn = new InnerConn(so, (u_long)ip, port, ih, ie);
			conn->setHandler(ih);
			conn->setLinkEvent(ie);
			conn->setTimeout(IMLBSD_TIMEOUT);
			if(cb)
				cb->onConnCreate(conn);
			conn->select(0, sox::SEL_READ);
			return conn;
		}
	};
}

using namespace core;
using namespace std;
using namespace server;
using namespace server::imlbs;

static CLinkdManager* sP = NULL;
void SigFunc(int sig)
{
	// 打印imlinkd在线信息
	sP->Trace();
}

int main(int sz, char *args[]) 
{
	SET_LOG_LEVEL_CONTROL_SINGLE_THREAD
	init_daemon dm(sz, args);
	WrapServerStart::init();

	CONFIG_RDAEMON_SERVER_INIT("imlbsd", BRouteAppContext, 
		MultiConnManagerImp,
		InnerConnCreator, 
		InnerConnCreator, 
		BackLinkHandler, 
		RouteWriter,
		TinyXmlServerConfigImp)

	if (signal(SIGUSR2, SigFunc) == SIG_ERR)
	{
		return -1;
	}

	LbsConnCreator lbsdConnCreator;
	BackLinkHandler outerHandler;
	MultiConnManagerVecImp lbsdConManager;
	lbsdConManager.setClientConnCreator(NULL);
	lbsdConManager.setServerConnCreator(&lbsdConnCreator);

	outerHandler.setAppContext(&__appContext);
	outerHandler.setPackLimit(1000);

	SimplePrxServer outerServer;
	outerServer.refreshPorts(__serverConfig.getFrontPorts());
	outerServer.setConnManager(&lbsdConManager);
	outerServer.setLinkHandler(&outerHandler);
	outerServer.setLinkEvent(&lbsdConManager);


	CLinkdManager tmpLinkdManager;
	ImLbsRequest lr;
	__appContext.addEntry(ImLbsRequest::getFormEntries(), &lr, &lr);
	tmpLinkdManager.setWriter(&__writer);

	lr.setDaemonClient(&__clientDaemon);
	lr.setServer(&__server);
	lr.setServer1(&outerServer);
	__server.watch(&lr);
	lr.setServerIdDispatcher(&__clientDaemon);
	
	lr.SetLinkdManager(&tmpLinkdManager);
	tmpLinkdManager.SetImLbsRequest(&lr);

	sP = &tmpLinkdManager;

	CONFIG_WATCHDOG_INIT
	// UDP ping
	CUdpPingServerConnCreator udpServerCreator;
	CUdpPingConnManagerImp udpPingManager;
	udpPingManager.setServerConnCreator(&udpServerCreator);

	CLbsUdpPingHandler udpHandler;
	udpHandler.setPackLimit(1024);
	udpHandler.SetLinkdManager(&tmpLinkdManager);

	CUdpPingServer udpServer;
	vector<uint16_t> vecFrontPorts = __serverConfig.getFrontPorts();
	udpServer.Start(*vecFrontPorts.rbegin(), &udpHandler, &udpPingManager, &udpPingManager);
	log (Info, "[main] create udp ping port ok. port(%d)", *vecFrontPorts.rbegin());


	P2PLbs p2plbs;
	p2plbs.setDaemonClient(&__clientDaemon);
	p2plbs.setServer(&__server);
	__appContext.addEntry(P2PLbs::getFormEntries(), &p2plbs, &p2plbs);
	DAEMON_SERVER_START

	outerServer.startSV();
//    IConn *xxx = __connManager.createClientConn("121.9.221.157", 443, &__handler, &__connManager);
//    xxx->sendBin("12345678", 4, -1);
	WrapServerStart::run();
}
