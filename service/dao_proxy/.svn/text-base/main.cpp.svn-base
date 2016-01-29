#include "server_common/helper/unixdaemon.h"
#include "server_common/config/LoaderContainer.h"
#include "server_common/server-lib3/IServerStategy.h"
#include "server_back/thrift-bridge/YYThriftHybirdHandler.h"
#include "server_back/thrift-bridge/ProxyProcessorImp.h"

#include "core/corelib/WrapServerStart.h"

#include "server_common/server-lib3/interface.h"
#include "GlobalCommon.h"
#include "PxYYRequestProcessor.h"
#include <unistd.h>
#include "ConsoleServer.h"

using namespace core;
using namespace server::thrift;
using namespace server::dao_proxy;
using namespace server::cache;
// timer
class CoreLoopTimerTask: public ITask
{
public:
	virtual void run()
	{
		g_pTimerContainer->poll();
		g_pMainThreadQueue->push_to_mini_timer_loop(new CoreLoopTimerTask);
	}
};



int main(int sz, char *args[]) {
    init_daemon dm(sz, args);
    WrapServerStart::init(); // config need init handle

	gl_processpid=getpid();

	gl_myconf=new PxConf();
	gl_globalconf=new PxGlobalConf();
	gl_hash_ring=new KTMHashRing();
	if(gl_myconf->init("../bin/dbd_conf.xml")<0)
	{
		cerr <<"myconf init error" <<endl;
		return -1;
	}
	if(gl_globalconf->init("../bin/daoproxy_globalconf.xml")<0)
	{
		cerr <<"global conf init error" <<endl;
		return -1;
	}
	if(gl_hash_ring->init(gl_globalconf->get_hash_ring())<0)
	{
		cerr <<"gl_hash_ring init error" <<endl;
		return -1;
	}

	///init timer
	g_pTimerContainer = new x_timer::TimerContainer(0x7FFFFFFF);
	g_pTimerContainer->init();

	////////////////
	gl_route_strategy=new RouteStrategy();
	if(gl_route_strategy->init_load_conf()<0)
	{
		cerr <<"gl_route_strategy init_load_conf error" <<endl;
		return -1;
	}
	gl_msghook=new MsgHook();

	gl_throughput_statstic=new ThroughputStat();
	if(gl_throughput_statstic->init()<0)
	{
		cerr <<"gl_throughput_statstic init error" <<endl;
		return -1;
	}
	gl_traffic_statstic=new TrafficStat();
	if(gl_traffic_statstic->init()<0)
	{
		cerr <<"gl_traffic_statstic init error" <<endl;
		return -1;
	}
	g_pFSMContainer=new id_map::IDMap<ANYPTRTYPE >();

	gl_daostatmanager=new DaoStatManager();
	if(gl_daostatmanager->init_from_conf()<0)
	{
		cerr <<"gl_daostatmanager init error" <<endl;
		return -1;
	}

	YYThriftHybirdHandler *pxyHandler = new YYThriftHybirdHandler();
	pxyHandler->set_queue_limit(10000);
	g_pHybirdLinkHandler=pxyHandler;

	YYThriftHybirdHandler *pxyHandlerFordao = new YYThriftHybirdHandler();
	pxyHandlerFordao->set_queue_limit(30000);
	g_pHybirdLinkHandlerForDAO=pxyHandlerFordao;

    ServerStategy ss;
    ss.type = DAEMONSERVER;
    ss.threadStategy = SINGLETHREAD;
    ss.groupId = gl_myconf->groupid;
    ss.port = gl_myconf->port;
    ss.name = gl_myconf->server_name;
	ss.queue_pack_size=5000;
	ss.thread_num=10;
	ss.handler=pxyHandler;

	IServerFacade *sf = initEviroment(ss);

	gl_service_connection_manager=sf->getConnManager();

	gl_daoconnectionmanager=new DaoConnectionManager();
	if(gl_daoconnectionmanager->init(sf->getSendConnFactory(),pxyHandlerFordao)<0)
	{
		cerr <<"DaoConnectionManager init error" <<endl;
		return -1;
	}

	if(gl_daoconnectionmanager->rc_calltest()<0)
	{
		cerr <<"gl_daoconnectionmanager init rc_calltest error" <<endl;
		return -1;
	}
	if(gl_daostatmanager->rc_calltest()<0)
	{
		cerr <<"gl_daostatmanager init rc_calltest error" <<endl;
		return -1;
	}
	gl_daostatmanager->stat_poller.start_timer(10);
	gl_daoconnectionmanager->connect_checker.start_timer(5);

	gl_thriftproxymsghandler=new ThriftPxMsgHandler();

	pxyHandler->setAppContext(sf->getAppContext());
	pxyHandler->setProxyProcessor(gl_thriftproxymsghandler);
	///
	pxyHandlerFordao->setAppContext(sf->getAppContext());
	pxyHandlerFordao->setProxyProcessor(gl_thriftproxymsghandler);
	///
	PxYYRequestProcessor __DaoResponseProcessor;
	sf->getAppContext()->addEntry(PxYYRequestProcessor::getFormEntries(), &__DaoResponseProcessor);

	///start x_timer
	g_pMainThreadQueue=sf->getMainThreadWriteQueue();
	g_pMainThreadQueue->push_to_mini_timer_loop(new CoreLoopTimerTask);



	ConsoleServer console;
	IServer* console_srv=sf->getServer(7878, SINGLETHREAD, 1000, 1,&console);
	console_srv->setLinkHandler(&console);
	console.setDaemonServer(sf->getDaemonServer());


	sf->startAllServer();
	sf->loop();

	return 0;
}

