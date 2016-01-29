#ifndef _TESTDAOR_REQUEST_H_
#define _TESTDAOR_REQUEST_H_

#include "common/core/ibase.h"
#include "server_common/server-lib/RouteTarget.h"
#include "server_common/server-lib3/interface.h"
#include "server_common/helper/TimerHandler.h"
#include "server_common/protocol/prouter.h"
#include "server_common/protocol/pdbnode.h"
#include "server_common/server-lib3/MTLinkHandler.h"
#include "server_common/server-lib3/WorkerQueue.h"

#include "server_common/seda/stage/stage_base.h"
#include <string>

using namespace core;

extern IMainThreadQueue * _mainQueue;
extern IConnManager * _connect_managerr;
extern uint32_t gl_sleep_us;
extern seda::linux_mutex gl_mtask_lock;
extern std::vector<ITask* > gl_mtask_q;


namespace server{
namespace testdaodb{
	using namespace core;


class TestDaoRequest
	: public core::PHClass
	, public core::InstanceTarget
	, public core::IInstanceFactory
{
public:
	uint32_t m_tran_time_us;
	uint32_t m_proc_time_us;
	MTLinkHandler* m_plink_hand;
	MTWorkerQueue* m_mtworkq;

public:
	TestDaoRequest();
	~TestDaoRequest();
	
	virtual PHClass* getInstance(Request& request, IConn* conn, InstanceTarget** out);

	DECLARE_FORM_MAP

	void dao_normal_request(::server::router::PProxyDaoRequest* req);
	void ping_request(::server::router::PProxyDaoPing* req);
	void stateRequest(server::db_node::DBStateReq * req);
	void adminRequest(server::db_node::DBAdminReq * req);

public:
	bool OnTimerCheckMalfunction();
	TimerHandler<TestDaoRequest, &TestDaoRequest::OnTimerCheckMalfunction> m_timer;

};
}
}	

#endif //_TESTDAOR_REQUEST_H_

