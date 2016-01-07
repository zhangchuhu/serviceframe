#include "BRouteWriter.h"
#include "UrlHelper.h"
#include "server_common/protocol/prouter.h"
#include "core/sox/logger.h"

using namespace core;
using namespace common::router;
using namespace server::router;

ABRouteWriter::ABRouteWriter()
	: m_pProxyInfo(NULL), m_pWrapHead(NULL)
{
	
}

void ABRouteWriter::answer(uint32_t uri, const sox::Marshallable &obj) 
{
	if (tmpFrom.empty())
	{
		send(pp->getAnswerPacket(uri, obj));
	}
	else
	{
		send(pp->getRoutePacket(getName(), tmpFrom, uri, obj));
	}
}

void ABRouteWriter::answer(uint32_t uri, uint16_t resCode, const sox::Marshallable &obj)
{
	if(tmpFrom.empty())
	{
		send(pp->getAnswerPacket(uri, resCode, obj));
    }
	else
	{
		send(pp->getRoutePacket(getName(), tmpFrom, uri, resCode, obj));
	}
}


/*
void ABRouteWriter::answer(uint32_t uri, const sox::Marshallable &obj){
	if(tmpFrom.empty()){
		send(pp->getAnswerPacket(uri, RES_SUCCESS, obj));
	}else{
		send(pp->getRoutePacket(getName(), tmpFrom, uri, obj));
	}
}*/

void ABRouteWriter::answerErr(uint32_t uri, uint16_t resCode){
	if(tmpFrom.empty())
    {
		send(pp->getAnswerPacket(uri, resCode, sox::Voidmable()));
	}
    else
    {
		send(pp->getRoutePacket(getName(), tmpFrom, uri, resCode));
	}
	
}

void ABRouteWriter::route(const std::string &from, const std::string &to, uint32_t uri, const sox::Marshallable &m){
	send(pp->getRoutePacket(from, to, uri, m));
	/*Sender *buf = getFreeSender();
	buf->setMethod(ROUTE);
	buf->marshall(m);
	pr.resCode = RES_SUCCESS;
	pr.from = from;
	pr.to = to;
	pr.ruri = uri;
	pr.load.assign(buf->body(), buf->bodySize());

	buf->clear();
	buf->marshall(PRouter::uri, pr);
	send(buf);*/
}

void ABRouteWriter::route(const std::string &to, uint32_t uri, const sox::Marshallable &m){
	send(pp->getRoutePacket(getName(), to, uri, m));
}

void ABRouteWriter::routeErr(const std::string &to, uint32_t uri, uint16_t resCode){
	send(pp->getRoutePacket(getName(), to, uri, resCode));
}


//void ABRouteWriter::routeU(const std::vector<uint32_t> &uids, uint32_t uri, const sox::Marshallable &m){
//	send(pp->getUPacket(getName(), uids, uri, m));

	/*Sender *buf = getFreeSender();
	buf->setMethod(ROUTE);
	buf->marshall(m);

	PUidsRouter pu;
	pu.resCode = RES_SUCCESS;
	pu.from = getName();
	pu.uids = uids;
	pu.ruri = uri;
	pu.load.assign(buf->body(), buf->bodySize());

	buf->clear();
	buf->marshall(PUidsRouter::uri, pu);
	send(buf);*/
//}

void ABRouteWriter::routeU(uint32_t uid, uint32_t imlinkd, uint32_t uri, const sox::Marshallable &m){
	PUidTarget prt;
	prt.from = getName();
	prt.packLoad(m);
	prt.ttl = 2;
	prt.resCode = RES_SUCCESS;
	prt.ruri = uri;
	prt.uid = uid;

	log(Info, "route to uid: %u, imlinkd: %u", uid, imlinkd);
	routeS(imlinkd, PUidTarget::uri, prt);
}

void ABRouteWriter::routeU2(uint32_t uid, uint32_t imlinkd, uint32_t uri, const sox::Marshallable &m, uint32_t uRouter)
{
	PUidTarget2 prt;
	prt.from = getName();
	prt.packLoad(m);
	prt.ttl = 2;
	prt.resCode = RES_SUCCESS;
	prt.ruri = uri;
	prt.uid = uid;
	prt.uRouter = uRouter; //设置路数，默认为0，使用TCP

	log(Info, "routeU2 to uid: %u, imlinkd: %u router:%u ", uid, imlinkd, uRouter);
	routeS(imlinkd, PUidTarget2::uri, prt);
}


/*
void ABRouteWriter::routeU(const std::map<uint32_t, uint32_t> &uid_imlinkd, uint32_t uri, const sox::Marshallable &m){
	for(map<uint32_t, uint32_t>::iterator it = uid_imlinkd.begin(); it != uid_imlinkd.end(); ++it){
		PUidTarget prt;
		prt.from = getName();
		prt.packLoad(m);
		prt.ttl = 2;
		prt.resCode = RES_SUCCESS;
		prt.ruri = uri;
		prt.uid = (uint32_t)(*it).first;

		log(Info, "route to uid: %u, imlinkd: %u", (uint32_t)(*it).first, (uint32_t)(*it).second);
		routeS((uint32_t)(*it).second, PUidTarget::uri, prt);
	}	
}*/

std::string ABRouteWriter::getName(){
	if(server==0)
	{
		return std::string("");
	}
	std::string tem;
	server->getFullName(tem);
	return tem;
}

void ABRouteWriter::routeS(uint32_t s, uint32_t u, const sox::Marshallable &o){
	routeS(getName(), s, u, o);
	/*Sender *buf = getFreeSender();

	buf->setMethod(ROUTE);
	buf->marshall(o);
	ps.resCode = RES_SUCCESS;
	ps.from = getName();
	ps.serverId = s;
	ps.ruri = u;
	ps.load.assign(buf->body(), buf->bodySize());
	
	buf->clear();
	buf->marshall(PServerIdRouter::uri, ps);
	send(buf);*/
}

void ABRouteWriter::routeS(const std::string &from, uint32_t s, uint32_t u, const sox::Marshallable &o){
    send(pp->getSPacket(from, s, u, o));

	/*Sender *buf = getFreeSender();
	buf->setMethod(ROUTE);
	buf->marshall(o);
	ps.resCode = RES_SUCCESS;
	ps.from = from;
	ps.serverId = s;
	ps.ruri = u;
	ps.load.assign(buf->body(), buf->bodySize());

	buf->clear();
	buf->marshall(PServerIdRouter::uri, ps);
	send(buf);*/
}

//add by kdjie, 2009.12.15
//用于发送序列化后的二进制数据

void ABRouteWriter::routeS(const std::string &from, uint32_t s, uint32_t u, const std::string &str)
{
	send(pp->getSPacket(from, s, u, str));
}
//add end

void ABRouteWriter::setFrom(const std::string &f){
	tmpFrom = f;
}

// add by Grj
void ABRouteWriter::RouteToServers(const std::string& to, uint32_t uri, const sox::Marshallable& obj)
{
	send(pp->GetRouteToSrvsPacket(getName(), to, uri, obj));
}

void ABRouteWriter::RouteRoundRobin(const std::string& to, uint32_t uri, const sox::Marshallable& obj)
{
	CProxyInfo *pInfo;

	if ((pInfo = GetProxyInfo()) != NULL)
	{
		PProxyRouter router;
		router.m_objProxyInfo = (*pInfo);
		router.m_objProxyInfo.m_vecProxyId.push_back(server->getServerId());
		//base
		router.from = tmpFrom;
		router.m_uServerId = -1;
		router.ruri= uri;
		router.resCode = RES_SUCCESS;
		router.packLoad(obj);

		send(pp->GetRoundRobinPacket(getName(), to, PProxyRouter::uri, router));
	}
	else
	{
		send(pp->GetRoundRobinPacket(getName(), to, uri, obj));
	}
	
}

void ABRouteWriter::RouteRoundRobin(const std::string& from, const std::string& to, uint32_t uri, const sox::Marshallable& obj)
{
	CProxyInfo *pInfo;

	if ((pInfo = GetProxyInfo()) != NULL)
	{
		PProxyRouter router;
		router.m_objProxyInfo = (*pInfo);
		router.m_objProxyInfo.m_vecProxyId.push_back(server->getServerId());
		//base
		router.from = tmpFrom;
		router.m_uServerId = -1;
		router.ruri= uri;
		router.resCode = RES_SUCCESS;
		router.packLoad(obj);

		send(pp->GetRoundRobinPacket(from, to, PProxyRouter::uri, router));
	}
	else
	{
		send(pp->GetRoundRobinPacket(from, to, uri, obj));
	}

}

std::string ABRouteWriter::GetFrom()
{
	return tmpFrom;
}
// end

// add by long
void ABRouteWriter::SetProxyInfo(CProxyInfo *p)
{
	m_pProxyInfo = p;
}


CProxyInfo *ABRouteWriter::GetProxyInfo()
{
	return m_pProxyInfo;
}

void ABRouteWriter::RouteWithProxyInfo(uint32_t uServerId, uint32_t uUri, const sox::Marshallable &objMsg)
{
    CProxyInfo *pInfo;

    if ((pInfo = GetProxyInfo()) != NULL)
    {
        PProxyRouter *router = (PProxyRouter *) pp->GetProxyPacket();
        router->from = tmpFrom;
        router->m_uServerId = uServerId;
        router->ruri = uUri;
        router->resCode = RES_SUCCESS;
        router->packLoad(objMsg);
        router->m_objProxyInfo = (*pInfo);
        //add my self
		uint32_t uServerId = server->getServerId();
		router->m_objProxyInfo.m_vecProxyId.push_back(uServerId);

		log(Debug, "[ABRouteWriter::RouteWithProxyInfo]: proxy route to %u", uServerId);
        send(router);
    }
    else
    {
		log(Debug, "[ABRouteWriter::RouteWithProxyInfo]: proxyinfo not found, use routeS");
        routeS(uServerId, uUri, objMsg);
    }
}

void ABRouteWriter::RouteWithProxyInfo(const std::string &strTo, uint32_t uUri, const sox::Marshallable &objMsg)
{
	CProxyInfo *pInfo;
	if ((pInfo = GetProxyInfo()) != NULL)
	{
        PProxyRouter router;
        //add my self
        router.m_objProxyInfo = (*pInfo);
		router.m_objProxyInfo.m_vecProxyId.push_back(server->getServerId());
        //base
        router.from = tmpFrom;
        router.m_uServerId = -1;
		router.ruri= uUri;
		router.resCode = RES_SUCCESS;
		router.packLoad(objMsg);

		log(Debug, "[ABRouteWriter::ProxyToRandom]: proxy route to %s", strTo.c_str());
		route(strTo, PProxyRouter::uri, router);
	}
	else
	{
		log(Debug, "[ABRouteWriter::ProxyToRandom]: proxyinfo not found, use route");
		route(strTo, uUri, objMsg);
	}
}

void ABRouteWriter::AnswerWithProxyInfo(uint32_t uUri, const sox::Marshallable &objMsg)
{
    AnswerWithProxyInfo(uUri, RES_SUCCESS, objMsg);
}

void ABRouteWriter::AnswerWithProxyInfo(uint32_t uUri, uint16_t uResCode, const sox::Marshallable &objMsg)
{
    CProxyInfo *pInfo;
    if ((pInfo = GetProxyInfo()) != NULL && pInfo->m_vecProxyId.size() > 0)
	{
        uint32_t uServerId = pInfo->m_vecProxyId.back();
        PProxyRouter *router = (PProxyRouter *) pp->GetProxyPacket();
        router->from = tmpFrom;
        router->ruri = uUri;
		router->resCode = uResCode;
        router->packLoad(objMsg);
        router->m_uServerId = uServerId;
        router->m_objProxyInfo = *pInfo;
		//remove my self
		router->m_objProxyInfo.m_vecProxyId.pop_back();

		//log(Debug, "[ABRouteWriter::AnswerWithProxyInfo]: proxyinfo found, anwser to server:%u", router->m_uServerId);
		send(router);
    }
	else
	{	
		//log(Debug, "[ABRouteWriter::AnswerWithProxyInfo]: proxyinfo empty, use answer");
		answer(uUri, objMsg);
	}
}

void ABRouteWriter::RouteSync(uint32_t uSvrId, uint32_t uUri, const sox::Marshallable& objMsg)
{
    send(pp->GetSyncPacket(uSvrId, uUri, objMsg));
}


void ABRouteWriter::RouteAck(PSS_SyncRouter *p, uint32_t uExpectId, uint32_t uImbridgeId)
{
    PSS_AckRouter *pMsg = (PSS_AckRouter *) pp->GetAckPacket();
    pMsg->m_uSeqId = p->m_uSeqId;
    pMsg->m_uDstId = p->m_uSrcId;
    pMsg->m_uSrcId = p->m_uDstId;
    pMsg->m_enType = p->m_enType;
    pMsg->m_uExpectId = uExpectId;
    pMsg->m_uBridgeId = uImbridgeId;
    send(pMsg);
}


void ABRouteWriter::AnswerProxyReliable(uint32_t uUri, uint16_t uResCode, const sox::Marshallable& objMsg)
{
    CProxyInfo *pInfo;
    if ((pInfo = GetProxyInfo()) != NULL && pInfo->m_vecProxyId.size() > 0)
    {
        PProxyRouter router;
        router.m_objProxyInfo = *pInfo;
        router.from = tmpFrom;
        router.ruri = uUri;
        router.resCode = uResCode;
        router.packLoad(objMsg);
        router.m_uServerId = router.m_objProxyInfo.m_vecProxyId.back();
        router.m_objProxyInfo.m_vecProxyId.pop_back();

        log(Debug, "[ABRouteWriter::AnswerProxyReliable]: proxyinfo found, anwser to server:%u", router.m_uServerId);
        RouteSync(router.m_uServerId, PProxyRouter::uri, router);
    }
    else
    {
        log(Debug, "[ABRouteWriter::AnswerProxyReliable]: proxyinfo empty, use answer");
        if (tmpFrom.empty())
        {
            log(Debug, "[ABRouteWriter::AnswerProxyReliable]: from empty!, can't answer");
        }
        else
        {
            uint32_t uServerId;
            if (DomainName::getSid(tmpFrom, uServerId))
            {
                RouteSync(uServerId, uUri, objMsg);
            }
        }
    }
}

void ABRouteWriter::AnswerProxyReliable(uint32_t uUri, const sox::Marshallable& objMsg)
{
    AnswerProxyReliable(uUri, RES_SUCCESS, objMsg);
}

// end

// add by lbc
void ABRouteWriter::RouteMultiSync(uint32_t uSvrId, uint32_t uUri, const sox::Marshallable& objMsg)
{
	send(pp->GetMultiSyncPacket(uSvrId, uUri, objMsg));
}
// end

const std::string & ABRouteWriter::getFrom() const
{
	return tmpFrom;
}

protocol::imlinkd::PSS_WrapRequest *ABRouteWriter::GetWrapHead()
{
	return m_pWrapHead;
}
void ABRouteWriter::SetWrapHead(protocol::imlinkd::PSS_WrapRequest *pWrapHead)
{
	m_pWrapHead = pWrapHead;
}

//使用Wrap头发送方式
void ABRouteWriter::RouteWithWrapHead(uint32_t uServerId, uint32_t uUri, const sox::Marshallable& objMsg)
{
	if (m_pWrapHead)
	{
		log(Info, "[ABRouteWriter::RouteWithWrapHead] WrapHead found, route:%u retry:%u taskid:%u", m_pWrapHead->m_uRouterNumber, m_pWrapHead->m_uRetryCount, m_pWrapHead->m_uTaskId);

		protocol::imlinkd::PSS_WrapRequest objWrap;
		objWrap.m_uUri = uUri;
		sox::PacketToString(objMsg, objWrap.m_strPacket);
		
		objWrap.SetWrapOption(m_pWrapHead->m_uRetryCount, m_pWrapHead->m_uRouterNumber, m_pWrapHead->m_uTaskId);

		RouteWithProxyInfo(uServerId, objWrap.uri, objWrap);
	}
	else
	{
		log(Info, "[ABRouteWriter::RouteWithWrapHead] WrapHead not found, use RouteWithProxyInfo");
		RouteWithProxyInfo(uServerId, uUri, objMsg);
	}
}
void ABRouteWriter::RouteWithWrapHead(const std::string &strTo, uint32_t uUri, const sox::Marshallable& objMsg, bool bRoundRobin)
{
	if (m_pWrapHead)
	{
		log(Info, "[ABRouteWriter::RouteWithWrapHead] WrapHead found, route:%u retry:%u taskid:%u", m_pWrapHead->m_uRouterNumber, m_pWrapHead->m_uRetryCount, m_pWrapHead->m_uTaskId);

		protocol::imlinkd::PSS_WrapRequest objWrap;
		objWrap.m_uUri = uUri;
		sox::PacketToString(objMsg, objWrap.m_strPacket);

		objWrap.SetWrapOption(m_pWrapHead->m_uRetryCount, m_pWrapHead->m_uRouterNumber, m_pWrapHead->m_uTaskId);

		if (bRoundRobin)
			RouteRoundRobin(strTo, objWrap.uri, objWrap);
		else
			RouteWithProxyInfo(strTo, objWrap.uri, objWrap);
	}
	else
	{
		log(Info, "[ABRouteWriter::RouteWithWrapHead] WrapHead not found, use RouteWithProxyInfo/RouteRoundRobin");

		if (bRoundRobin)
			RouteRoundRobin(strTo, uUri, objMsg);
		else
			RouteWithProxyInfo(strTo, uUri, objMsg);
	}
}
void ABRouteWriter::AnswerWithWrapHead(uint32_t uUri, const sox::Marshallable& objMsg)
{
	if (m_pWrapHead)
	{
		log(Info, "[ABRouteWriter::AnswerWithWrapHead] WrapHead found, route:%u retry:%u taskid:%u", m_pWrapHead->m_uRouterNumber, m_pWrapHead->m_uRetryCount, m_pWrapHead->m_uTaskId);

		protocol::imlinkd::PSS_WrapRequest objWrap;
		objWrap.m_uUri = uUri;
		sox::PacketToString(objMsg, objWrap.m_strPacket);

		objWrap.SetWrapOption(m_pWrapHead->m_uRetryCount, m_pWrapHead->m_uRouterNumber, m_pWrapHead->m_uTaskId);

		AnswerWithProxyInfo(objWrap.uri, objWrap);
	}
	else
	{
		log(Info, "[ABRouteWriter::AnswerWithWrapHead] WrapHead not found, use AnswerWithProxyInfo");
		AnswerWithProxyInfo(uUri, objMsg);
	}
}
void ABRouteWriter::RouteWithWrapHeadTrans(uint32_t uServerId, uint32_t uUri, const sox::Marshallable& objMsg) //透明方式
{
	if (m_pWrapHead)
	{
		log(Info, "[ABRouteWriter::RouteWithWrapHeadTrans] WrapHead found, route:%u retry:%u taskid:%u", m_pWrapHead->m_uRouterNumber, m_pWrapHead->m_uRetryCount, m_pWrapHead->m_uTaskId);

		protocol::imlinkd::PSS_WrapRequest objWrap;
		objWrap.m_uUri = uUri;
		sox::PacketToString(objMsg, objWrap.m_strPacket);

		objWrap.SetWrapOption(m_pWrapHead->m_uRetryCount, m_pWrapHead->m_uRouterNumber, m_pWrapHead->m_uTaskId);

		//RouteWithProxyInfo(uServerId, objWrap.uri, objWrap);

		CProxyInfo *pInfo;

		if ((pInfo = GetProxyInfo()) != NULL)
		{
			PProxyRouter *router = (PProxyRouter *) pp->GetProxyPacket();
			router->from = tmpFrom;
			router->m_uServerId = uServerId;
			router->ruri = objWrap.uri;
			router->resCode = RES_SUCCESS;
			router->packLoad(objWrap);
			router->m_objProxyInfo = (*pInfo);
			//add my self
			//uint32_t uServerId = server->getServerId();
			//router->m_objProxyInfo.m_vecProxyId.push_back(uServerId);

			log(Debug, "[ABRouteWriter::RouteWithWrapHeadTrans]: proxy route from %s to sid %u", tmpFrom.c_str(), uServerId);
			send(router);
		}
		else
		{
			log(Debug, "[ABRouteWriter::RouteWithWrapHeadTrans]: proxyinfo not found, use routeS from %s to sid %u", tmpFrom.c_str(), uServerId);
			routeS(tmpFrom, uServerId, objWrap.uri, objWrap);
		}
	}
	else
	{
		log(Info, "[ABRouteWriter::RouteWithWrapHeadTrans] WrapHead not found, use RouteWithProxyInfo");
		//RouteWithProxyInfo(uServerId, uUri, objMsg);
		CProxyInfo *pInfo;

		if ((pInfo = GetProxyInfo()) != NULL)
		{
			PProxyRouter *router = (PProxyRouter *) pp->GetProxyPacket();
			router->from = tmpFrom;
			router->m_uServerId = uServerId;
			router->ruri = uUri;
			router->resCode = RES_SUCCESS;
			router->packLoad(objMsg);
			router->m_objProxyInfo = (*pInfo);
			//add my self
			//uint32_t uServerId = server->getServerId();
			//router->m_objProxyInfo.m_vecProxyId.push_back(uServerId);

			log(Debug, "[ABRouteWriter::RouteWithWrapHeadTrans]: proxy route from %s to sid %u", tmpFrom.c_str(), uServerId);
			send(router);
		}
		else
		{
			log(Debug, "[ABRouteWriter::RouteWithWrapHeadTrans]: proxyinfo not found, use routeS from %s to sid %u", tmpFrom.c_str(), uServerId);
			routeS(tmpFrom, uServerId, uUri, objMsg); //use src url
		}
	}
}
void ABRouteWriter::RouteWithWrapHeadTrans(const std::string &strTo, uint32_t uUri, const sox::Marshallable& objMsg) //透明方式
{
	if (m_pWrapHead)
	{
		log(Info, "[ABRouteWriter::RouteWithWrapHeadTrans] WrapHead found, route:%u retry:%u taskid:%u", m_pWrapHead->m_uRouterNumber, m_pWrapHead->m_uRetryCount, m_pWrapHead->m_uTaskId);

		protocol::imlinkd::PSS_WrapRequest objWrap;
		objWrap.m_uUri = uUri;
		sox::PacketToString(objMsg, objWrap.m_strPacket);

		objWrap.SetWrapOption(m_pWrapHead->m_uRetryCount, m_pWrapHead->m_uRouterNumber, m_pWrapHead->m_uTaskId);

		//RouteWithProxyInfo(strTo, objWrap.uri, objWrap);

		CProxyInfo *pInfo;
		if ((pInfo = GetProxyInfo()) != NULL)
		{
			PProxyRouter router;
			//add my self
			router.m_objProxyInfo = (*pInfo);
			//router.m_objProxyInfo.m_vecProxyId.push_back(server->getServerId());
			//base
			router.from = tmpFrom;
			router.m_uServerId = -1;
			router.ruri= objWrap.uri;
			router.resCode = RES_SUCCESS;
			router.packLoad(objWrap);

			log(Debug, "[ABRouteWriter::RouteWithWrapHeadTrans]: proxy route from %s to %s", tmpFrom.c_str(), strTo.c_str());
			route(tmpFrom, strTo, PProxyRouter::uri, router);
		}
		else
		{
			log(Debug, "[ABRouteWriter::RouteWithWrapHeadTrans]: proxyinfo not found, use route from %s to %s", tmpFrom.c_str(), strTo.c_str());
			route(tmpFrom, strTo, uUri, objMsg); //use src url
		}
	}
	else
	{
		log(Info, "[ABRouteWriter::RouteWithWrapHeadTrans] WrapHead not found, use RouteWithProxyInfo");
		//RouteWithProxyInfo(strTo, uUri, objMsg);

		CProxyInfo *pInfo;
		if ((pInfo = GetProxyInfo()) != NULL)
		{
			PProxyRouter router;
			//add my self
			router.m_objProxyInfo = (*pInfo);
			//router.m_objProxyInfo.m_vecProxyId.push_back(server->getServerId());
			//base
			router.from = tmpFrom;
			router.m_uServerId = -1;
			router.ruri= uUri;
			router.resCode = RES_SUCCESS;
			router.packLoad(objMsg);

			log(Debug, "[ABRouteWriter::RouteWithWrapHeadTrans]: proxy route from %s to %s", tmpFrom.c_str(), strTo.c_str());
			route(tmpFrom, strTo, PProxyRouter::uri, router);
		}
		else
		{
			log(Debug, "[ABRouteWriter::RouteWithWrapHeadTrans]: proxyinfo not found, use route from %s to %s", tmpFrom.c_str(), strTo.c_str());
			route(tmpFrom, strTo, uUri, objMsg); //use src url
		}
	}
}
