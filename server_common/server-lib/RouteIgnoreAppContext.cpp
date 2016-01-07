#include "RouteIgnoreAppContext.h"
#include "common/core/form.h"
#include "BRouteWriter.h"
#include "UrlHelper.h"
#include "server_common/protocol/prouter.h"
#include "core/sox/logger.h"
#include "common/protocol/const.h"

using namespace sox;
using namespace core;
using namespace common::router;
using namespace server::router;

BEGIN_FORM_MAP(RouteIgnoreAppContext)
	ON_LINK(PRouter, &RouteIgnoreAppContext::route)
	ON_LINK(PServerIdRouter, &RouteIgnoreAppContext::routeS)
END_FORM_MAP()


RouteIgnoreAppContext::RouteIgnoreAppContext(){
	addEntry(RouteIgnoreAppContext::getFormEntries(), this, this);
}


void RouteIgnoreAppContext::route(PRouter *rt, IConn *conn){
	Request rq(rt->load.data(), rt->load.length());
	rq.setUri(rt->ruri);
	rq.setResCode(rt->resCode);
	rq.setKey(rt->from);
	rq.to = rt->to;
	requestDispatch(rq, conn);	
}

void RouteIgnoreAppContext::routeS(server::router::PServerIdRouter *rt, IConn *conn){
	Request rq(rt->load.data(), rt->load.length());
	rq.setUri(rt->ruri);
	rq.setResCode(rt->resCode);
	rq.setKey(rt->from);
	requestDispatch(rq, conn);
}

