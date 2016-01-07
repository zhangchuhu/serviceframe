#ifndef ROUTEIGNORE_APPCONTEXT_H
#define ROUTEIGNORE_APPCONTEXT_H
#include "common/core/ibase.h"
#include "core/corelib/MfcAppContext.h"
#include "server_common/protocol/prouter.h"

namespace core{
	class RouteIgnoreAppContext: public MfcAppcontext, public PHClass{
	public:
		DECLARE_FORM_MAP

		RouteIgnoreAppContext();
		void route(server::router::PRouter *rt, IConn *conn);
		void routeS(server::router::PServerIdRouter *rt, IConn *conn);
	};	
}


#endif

