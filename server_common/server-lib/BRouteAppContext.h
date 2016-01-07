#ifndef BACK_ROUTE_APPCONTEXT_H
#define BACK_ROUTE_APPCONTEXT_H
#include <map>
#include "common/core/ibase.h"
#include "core/corelib/MfcAppContext.h"
#include "common/protocol/prouter.h"
#include "server_common/protocol/prouter.h"
#include "server_common/server-lib/iclient.h"

#include "server_common/protocol/PWrapRequest.h"

namespace core
{
	class BRouteAppContext:
        public PHClass, 
        public MfcAppcontext
    {
	public:
		DECLARE_FORM_MAP

		BRouteAppContext();
		void route(server::router::PRouter *rt, IConn *conn);
		void routeS(server::router::PServerIdRouter *rt, IConn *conn);
		void branch(server::router::PBrachRoute *br, IConn *conn);

//wuji start
//		IWriter *requestDispatch(Request &request, IConn *conn);
//wuji end

        //add by long
        void OnRouteProxy(server::router::PProxyRouter *pr, IConn *conn);
        void OnPing();

		//add by kdjie
		void OnPSS_WrapRequest(protocol::imlinkd::PSS_WrapRequest *p, IConn *conn);

	protected:
		virtual uint32_t getUid(Request &request, IConn *conn);
		virtual uint32_t getSid(Request &request, IConn *conn);
        virtual void getKeys(std::vector<uint32_t> &vecKey);
	};	
}


#endif
