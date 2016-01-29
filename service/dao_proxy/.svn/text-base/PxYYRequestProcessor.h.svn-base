#ifndef _PROXYYYREQUEST_PROCESSOR_H_20111031_
#define _PROXYYYREQUEST_PROCESSOR_H_20111031_
#include "common/core/ibase.h"
#include "common/core/form.h"

#include "server_common/server-lib/RouteTarget.h"
#include "server_common/server-lib3/interface.h"
#include "server_common/helper/TimerHandler.h"
#include <string>
#include "server_common/protocol/prouter.h"
#include "server_common/protocol/pdbnode.h"

namespace server{
namespace dao_proxy{
	using namespace core;

class PxYYRequestProcessor
	: public core::PHClass
	, public core::InstanceTarget
	, public core::IInstanceFactory
{
public:
	PxYYRequestProcessor();
	~PxYYRequestProcessor();

	virtual PHClass* getInstance(Request& request, IConn* conn, InstanceTarget** out);

	DECLARE_FORM_MAP

	void daoheadresponse(::server::router::PProxyDaoResponse* resp);
	void pingresponse(::server::router::PProxyDaoPingResp* resp);

	void stateRequest(server::db_node::DBStateReq * req);

	void adminRequest(server::db_node::DBAdminReq * req);
};
}
}	

#endif /*_PROXYYYREQUEST_PROCESSOR_H_20111031_*/

