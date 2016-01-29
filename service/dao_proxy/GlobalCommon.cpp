#include "GlobalCommon.h"

#include <iostream>
#include "server_common/helper/unixdaemon.h"
#include "core/corelib/WrapServerStart.h"
#include "server_common/server-lib3/IServerStategy.h"
#include "server_common/sq_client/any_cast.h"


using namespace core;
namespace server
{
namespace dao_proxy
{

id_map::IDMap<ANYPTRTYPE >*		g_pFSMContainer=NULL;
core::IMainThreadQueue*			g_pMainThreadQueue=NULL;
server::thrift::YYThriftHybirdHandler* g_pHybirdLinkHandler=NULL;
server::thrift::YYThriftHybirdHandler* g_pHybirdLinkHandlerForDAO=NULL;

x_timer::TimerContainer*			g_pTimerContainer=NULL;

PxConf* gl_myconf=NULL;
PxGlobalConf* gl_globalconf=NULL;

IConnManager* gl_service_connection_manager=NULL;

RouteStrategy* gl_route_strategy=NULL;
DaoStatManager* gl_daostatmanager=NULL;
ThroughputStat* gl_throughput_statstic=NULL;
TrafficStat* gl_traffic_statstic=NULL;

DaoConnectionManager* gl_daoconnectionmanager=NULL;
ThriftPxMsgHandler* gl_thriftproxymsghandler=NULL;

KTMHashRing* gl_hash_ring=NULL;

MsgHook* gl_msghook=NULL;

uint64_t gl_processpid=0;

uint64_t get_current_time_US()
{
	struct timeval tv;
	gettimeofday(&tv,NULL);
	uint64_t current_time_us = (uint64_t)((uint64_t)tv.tv_sec*1000000ull + tv.tv_usec);
	return current_time_us;
}
uint64_t get_current_time_S()
{
	return g_pTimerContainer->get_current_time();
}



}
}
