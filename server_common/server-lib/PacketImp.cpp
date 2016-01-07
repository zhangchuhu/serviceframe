#include "PacketImp.h"
#include "server_common/protocol/prouter.h"
#include "core/sox/logger.h"

using namespace core;
using namespace server::router;


// add by Grj
IPacketBase *PacketImp::GetRouteToSrvsPacket(const std::string& from, const std::string& to, uint32_t uri, const sox::Marshallable& obj)
{
	PToServersRoute *pk;
	if (!rtss.empty())
	{
		pk = rtss.front();
		rtss.pop_front();
	}
	else
	{
		pk = new PToServersRoute();
	}
	pk->from = from;
	pk->to = to;
	pk->ruri = uri;
	pk->resCode = RES_SUCCESS;
	pk->packLoad(obj);
	pk->ttl = 2;
	return pk;
}

void PacketImp::gcRouteToSrvsPacket(IPacketBase *rb)
{
	delete ((PToServersRoute*) rb);//rtss.push_back((PToServersRoute*) rb);
}

IPacketBase* PacketImp::GetRoundRobinPacket(const std::string& from, const std::string& to, uint32_t uUri, const sox::Marshallable& obj)
{
	PRoundRobinRouter* pk;
	if (!m_listRoundRobinRouter.empty())
	{
		pk = m_listRoundRobinRouter.front();
		m_listRoundRobinRouter.pop_front();
	}
	else
	{
		pk = new PRoundRobinRouter();
	}
	pk->m_router.from = from;
	pk->m_router.to = to;
	pk->m_router.ruri = uUri;
	pk->m_router.packLoad(obj);
	pk->m_router.resCode = RES_SUCCESS;
	pk->m_router.ttl = 2;
	return pk;
}

void PacketImp::GcRoundRobinPacket(IPacketBase *rb)
{
	delete ((PRoundRobinRouter*) rb);//m_listRoundRobinRouter.push_back((PRoundRobinRouter*) rb);
}
// end


IPacketBase *PacketImp::getRoutePacket(const std::string &from, const std::string &to, uint32_t uri, uint16_t resCode, const sox::Marshallable &obj){
	PRouter *pk;
	if(!rrs.empty()){
		pk = rrs.front();
		rrs.pop_front();
	}else{
		pk = new PRouter();
	}
	pk->from = from;
	pk->to = to;
	pk->ruri = uri;
	pk->resCode = resCode;
	pk->packLoad(obj);
	pk->ttl = 2;
	return pk;
}


IPacketBase *PacketImp::getRoutePacket(const std::string &from, const std::string &to, uint32_t uri, const sox::Marshallable &obj){
	PRouter *pk;
	if(!rrs.empty()){
		pk = rrs.front();
		rrs.pop_front();
	}else{
		pk = new PRouter();
	}
	pk->from = from;
	pk->to = to;
	pk->ruri = uri;
	pk->resCode = RES_SUCCESS;
	pk->packLoad(obj);
	pk->ttl = 2;
	return pk;
}

IPacketBase *PacketImp::getRoutePacket(const std::string &from, const std::	string &to, uint32_t uri, uint16_t err){
	PRouter *pk;
	if(!rrs.empty()){
		pk = rrs.front();
		rrs.pop_front();
	}else{
		pk = new PRouter();
	}
	pk->from = from;
	pk->to = to;
	pk->ruri = uri;
	pk->resCode = err;
	pk->ttl = 2;
	return pk;
}

IPacketBase *PacketImp::getSPacket(const std::string &from, uint32_t s, uint32_t uri, const sox::Marshallable &obj){
	PServerIdRouter *pk;
	if(!rss.empty()){
		pk = rss.front();
		rss.pop_front();
	}else{
		pk = new PServerIdRouter();
	}
	pk->from = from;
	pk->serverId = s;
	pk->ruri = uri;
	pk->resCode = RES_SUCCESS;
	pk->packLoad(obj);
	pk->ttl = 2;
	return pk;
}

//add by kdjie, 2009.12.16
IPacketBase *PacketImp::getSPacket(const std::string &from, uint32_t s, uint32_t uri, const std::string &str)
{
	PServerIdRouter *pk;
	if (!rss.empty())
	{
		pk = rss.front();
		rss.pop_front();
	}
	else
	{
		pk = new PServerIdRouter();
	}
	pk->from = from;
	pk->serverId = s;
	pk->ruri = uri;
	pk->resCode = RES_SUCCESS;
	pk->packLoad(str);
	pk->ttl = 2;
	return pk;
}
//add end

IPacketBase *PacketImp::getUPacket(const std::string from, const std::vector<uint32_t> &uids, uint32_t uri, const sox::Marshallable &obj){
	PUidsRouter *pk;
	if(!rus.empty()){
		pk = rus.front();
		rus.pop_front();
	}else{
		pk = new PUidsRouter();
	}
	pk->from = from;
	pk->uids = uids;
	pk->ruri = uri;
	pk->resCode = RES_SUCCESS;
	pk->packLoad(obj);
	pk->ttl = 2;
	return pk;
}


IPacketBase *PacketImp::getAnswerPacket(uint32_t uri, const sox::Marshallable &obj){
	PAnswer *pa;
	if(!ras.empty()){
		pa = ras.front();
		ras.pop_front();
	}else{
		pa = new PAnswer;
	}
	pa->ss.setResCode(RES_SUCCESS);
	pa->ss.marshall(uri, obj);
	return pa;
}

IPacketBase *PacketImp::getAnswerPacket(uint32_t uri, uint16_t resCode, const sox::Marshallable &obj){
	PAnswer *pa;
	if(!ras.empty()){
		pa = ras.front();
		ras.pop_front();
	}else{
		pa = new PAnswer;
	}
	pa->ss.setResCode(resCode);
	pa->ss.marshall(uri, obj);
	return pa;
}

IPacketBase *PacketImp::GetSyncPacket(uint32_t uSvrId, uint32_t uUri, const sox::Marshallable &objMsg)
{
    PSS_SyncRouter *p;
    if (!m_listSyncRouter.empty())
    {
        p = m_listSyncRouter.front();
        m_listSyncRouter.pop_front();
    }
    else
    {
        p = new PSS_SyncRouter;
    }

    p->ruri = uUri;
    p->packLoad(objMsg);
    p->m_uDstId = uSvrId;
    return p;
}

IPacketBase *PacketImp::GetMultiSyncPacket(uint32_t uSvrId, uint32_t uUri, const sox::Marshallable &objMsg)
{
	PSS_SyncRouter *p;
	p = new PSS_SyncRouter(ENUM_RELIABLE_TYPE_DATA_MULTI_ROUTE);

	p->ruri = uUri;
	p->packLoad(objMsg);
	p->m_uDstId = uSvrId;
	return p;
}

IPacketBase *PacketImp::GetAckPacket()
{
    PSS_AckRouter *p;
    if (!m_listAckRouter.empty())
    {
        p = m_listAckRouter.front();
        m_listAckRouter.pop_front();
    }
    else
    {
        p = new PSS_AckRouter();
    }
    return p;
}

void PacketImp::gcPackets(std::vector<IPacketBase *> &gcs){
	for(std::vector<IPacketBase *>::iterator it = gcs.begin(); it != gcs.end(); ++it){
		(*it)->gc(this);
	}
	gcs.clear();
}


void PacketImp::gcRoutePacket(IPacketBase *rb){
	delete ((PRouter *)rb);//rrs.push_back((PRouter *)rb);
}

void PacketImp::gcSPacket(IPacketBase *rb){
	delete ((PServerIdRouter *)rb);//rss.push_back((PServerIdRouter *)rb);
}

void PacketImp::gcUPacket(IPacketBase *rb){
	delete ((PUidsRouter *)rb);//rus.push_back((PUidsRouter *)rb);
}

void PacketImp::gcAnswerPacket(IPacketBase *rb){
	delete ((PAnswer *)rb);//ras.push_back((PAnswer *)rb);
}

void PacketImp::GcSyncPacket(IPacketBase *p)
{
    delete ((PSS_SyncRouter *) p);//m_listSyncRouter.push_back((PSS_SyncRouter *) p);
}

void PacketImp::GcAckPacket(IPacketBase *p)
{
   delete ((PSS_AckRouter *) p);//m_listAckRouter.push_back((PSS_AckRouter *) p);
}

IPacketBase *PacketImp::GetProxyPacket()
{
    PProxyRouter *p;
    if (!m_listProxyRouter.empty())
    {
        p = m_listProxyRouter.front();
        m_listProxyRouter.pop_front();
    }
    else
    {
        p = new PProxyRouter();
    }
    p->m_objProxyInfo.m_vecProxyId.clear();
    p->m_objProxyInfo.m_vecKey.clear();
    return p;
}

void PacketImp::GcProxyPacket(IPacketBase *p)
{
    delete ((PProxyRouter *) p);//m_listProxyRouter.push_back((PProxyRouter *) p);
}
