#include "BRouteAppContext.h"
#include "common/core/form.h"
#include "BRouteWriter.h"
#include "UrlHelper.h"
#include "server_common/protocol/prouter.h"
#include "core/sox/logger.h"
#include "common/protocol/const.h"
#include "server_common/server-lib/ICMPException.h"

//wuji start
//#include <pthread.h>
//#include <time.h>
//wuji end

using namespace sox;
using namespace core;
using namespace common::router;
using namespace server::router;
using namespace protocol::imlinkd;

BEGIN_FORM_MAP(BRouteAppContext)
	ON_LINK(PRouter, &BRouteAppContext::route)
	ON_LINK(PServerIdRouter, &BRouteAppContext::routeS)//接收到的包是以PServerIdRouter承载的，包的是PRouter包，PRouter里面包的是应用包
	ON_LINK(PBrachRoute, &BRouteAppContext::branch)
	ON_LINK(PProxyRouter, &BRouteAppContext::OnRouteProxy)
    ON_VOID(PSS_PingRouter, &BRouteAppContext::OnPing)

	ON_LINK(PSS_WrapRequest, &BRouteAppContext::OnPSS_WrapRequest)
END_FORM_MAP()


BRouteAppContext::BRouteAppContext(){
	addEntry(BRouteAppContext::getFormEntries(), this, this);
}

void BRouteAppContext::OnPing()
{
    ABRouteWriter *rw = (ABRouteWriter *)getWriter();
    PSS_PingRouterRes obj;
    rw->answer(PSS_PingRouterRes::uri, obj);
}

//add by long: support proxy mode, and reliable mode router
void BRouteAppContext::OnRouteProxy(PProxyRouter *rt, IConn *conn)
{
    Request rq(rt->load.data(), rt->load.length());
    rq.setUri(rt->ruri);
    rq.setResCode(rt->resCode);
    rq.setKey(rt->from);

    ABRouteWriter *rw = (ABRouteWriter *)getWriter();
    rw->setFrom(rt->from);
    rw->SetProxyInfo(&(rt->m_objProxyInfo));
	
    //log(Debug, "[BRouteAppContext::OnRouteProxy]: from:%s", rt->from.c_str());
	//BRouteAppContext::requestDispatch(rq, conn);
	requestDispatch(rq, conn);
	rw->SetProxyInfo(NULL);
	rw->setFrom("");
}

//end long

void BRouteAppContext::branch(server::router::PBrachRoute *br, IConn *conn){
	Request request(br->load.data(), br->load.size());
	request.head();
	ABRouteWriter *rw = (ABRouteWriter *)getWriter();
	rw->setFrom("");
	//try{
	//BRouteAppContext::requestDispatch(request, conn);
	requestDispatch(request, conn);
}

void BRouteAppContext::route(PRouter *rt, IConn *conn){
	Request rq(rt->load.data(), rt->load.length());
	rq.setUri(rt->ruri);
	rq.setResCode(rt->resCode);
	rq.setKey(rt->from);
	rq.to = rt->to;

	ABRouteWriter *rw = (ABRouteWriter *)getWriter();
	rw->setFrom(rt->from);
	//try{
	//BRouteAppContext::requestDispatch(rq, conn);
	requestDispatch(rq, conn);

	rw->setFrom("");
	/*}catch(ICMPException &){
		PICMP icmp;
		icmp.from = rt->from;
		icmp.to = rt->to;
		icmp.ruri = rt->ruri;
		icmp.resCode = rt->resCode;
		icmp.ttl = rt->ttl - 1;
		icmp.type = PICMP::not_in_mine;
		icmp.load = rt->load;
		rw->icmp(icmp);
	}*/
}


// IWriter *BRouteAppContext::	requestDispatch(Request &request, IConn *conn)
// {
// 	clock_t start = clock();
// 	IWriter *pObj=MfcAppcontext::requestDispatch(request, conn);
// 	clock_t end = clock();
// 	if((request.getUri() & 0xff) !=11)
// 		log(Info, "tid:%u uri:%u/%u clock:%f" , pthread_self(), request.getUri() & 0xff, request.getUri() >> 8, (double)(end - start) /   CLOCKS_PER_SEC );
// 
// 	return pObj;
// 
// 
// }

void BRouteAppContext::routeS(server::router::PServerIdRouter *rt, IConn *conn){
	Request rq(rt->load.data(), rt->load.length());
	rq.setUri(rt->ruri);
	rq.setResCode(rt->resCode);
	rq.setKey(rt->from);
	ABRouteWriter *rw = (ABRouteWriter *)getWriter();
	rw->setFrom(rt->from);
	//try{
//	BRouteAppContext::requestDispatch(rq, conn);
	requestDispatch(rq, conn);

	rw->setFrom("");
	/*}catch(ICMPException &){
		log(Error, "route serverid icmp error!!!");
	}*/
}

uint32_t BRouteAppContext::getUid(Request &request, IConn *conn){	
	uint32_t uid;
	bool res = DomainName::toUid(request.getKey(), uid);
	if(res){
		//log(Debug, "%s to uid:%u", request.getKey().data(), uid);
		return uid;
	}else{
		log(Error, "[BRouteAppContext::getUid]: error uid:%s", request.getKey().data());
		return INVALID_UID;
	}
}

uint32_t BRouteAppContext::getSid(Request &request, IConn *conn){	
	uint32_t sid;
	bool res = DomainName::getSessionId(request.getKey(), sid);
	if(res){
		return sid;
	}else{
		log(Error, "BRouterAppContext error sid:%s", request.getKey().data());
		return INVALID_UID;
	}
}

void BRouteAppContext::getKeys(std::vector<uint32_t> &vecKey)
{
    ABRouteWriter *rw = (ABRouteWriter *) getWriter();
    CProxyInfo *pInfo;
    if ((pInfo = rw->GetProxyInfo()) != NULL)
    {
        vecKey = pInfo->m_vecKey;
    }
    else
    {
        log(Warn, "proxy key empty");
    }
}

void BRouteAppContext::OnPSS_WrapRequest(protocol::imlinkd::PSS_WrapRequest *p, IConn *conn)
{
	ABRouteWriter *rw = (ABRouteWriter *) getWriter();

	log(Info, "[BRouteAppContext::OnPSS_WrapRequest] rw->GetFrom:%s uri:0x%x RouteNumber:%u RetryCount:%u TaskId:%u", 
		rw->GetFrom().c_str(), p->m_uUri, p->m_uRouterNumber, p->m_uRetryCount, p->m_uTaskId);

	Request request(p->m_strPacket.data(), p->m_strPacket.size());
	request.setUri(p->m_uUri);
	request.setResCode(RES_SUCCESS);
	request.setKey(rw->GetFrom()); //借用底层包设置的From

	//临时记录下PSS_WrapRequest头，以便发送时使用
	rw->SetWrapHead(p);

	requestDispatch(request, conn);

	rw->SetWrapHead(NULL);
}
