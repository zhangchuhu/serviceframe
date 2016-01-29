#ifndef _DAOP_THE_GLOBALCOMMON_H_
#define _DAOP_THE_GLOBALCOMMON_H_

#include "server_common/server-lib3/interface.h"
#include "server_common/server-lib3/ITask.h"
#include "server_common/seda/common/id_map.h"
#include "x_timer.hpp"
#include <string>
#include <map>
#include <vector>

#include "../thrift-bridge/YYThriftHybirdHandler.h"

#include "PxConf.h"
#include "PxGlobalConf.h"
#include "dao_connection_manager.h"
#include "dao_manager.h"
#include "PxThriftMsgHandler.h"
#include "ThroughputStat.h"
#include "RouteStrategy.h"
#include "KTMHashRing.h"
#include "TrafficStat.h"
#include "MsgHook.h"

namespace server
{
namespace dao_proxy
{
	typedef void* ANYPTRTYPE;
	extern id_map::IDMap<ANYPTRTYPE >*		g_pFSMContainer;
	extern core::IMainThreadQueue*			g_pMainThreadQueue;
	extern server::thrift::YYThriftHybirdHandler* g_pHybirdLinkHandler;

	extern server::thrift::YYThriftHybirdHandler* g_pHybirdLinkHandlerForDAO;
	
	extern x_timer::TimerContainer*			g_pTimerContainer;

	extern PxConf* gl_myconf;
	extern PxGlobalConf* gl_globalconf;

	extern IConnManager* gl_service_connection_manager;

	extern RouteStrategy* gl_route_strategy;
	extern DaoStatManager* gl_daostatmanager;
	extern DaoConnectionManager* gl_daoconnectionmanager;
	extern ThroughputStat* gl_throughput_statstic;
	extern TrafficStat* gl_traffic_statstic;

	extern ThriftPxMsgHandler* gl_thriftproxymsghandler;

	extern KTMHashRing* gl_hash_ring;
	extern MsgHook* gl_msghook;

	extern uint64_t gl_processpid;

	uint64_t get_current_time_US();
	uint64_t get_current_time_S();

}
}

#endif	// _DAOP_THE_GLOBALCOMMON_H_
