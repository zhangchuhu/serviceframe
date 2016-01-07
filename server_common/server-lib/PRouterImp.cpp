#include "server_common/protocol/prouter.h"
#include "DomainName.h"
#include "common/core/iserver.h"
#include "IPacket.h"
#include "id_def.h"
#include "common/core/sender.h"
#include "UrlHelper.h"
#include "core/sox/logger.h"
#include "ReliableWriter.h"

using namespace server::router;
using namespace core;

int PAnswer::emit(core::IConn *conn, core::IServerIdDispatcher* sd){
	if(conn){
		conn->send(ss);
	}else{
		log(Warn, "Answer but conn null uri:%u", ss.getUri());
	}
	return 0;
}

void PAnswer::gc(IPacketGc *pg){
	ss.clear();
	pg->gcAnswerPacket(this);
}

int PRouter::emit(core::IConn *conn, core::IServerIdDispatcher* sd){
	uint32_t serverId;
	if(DomainName::getSid(to, serverId)){
//		sd->dispatchByServerId(serverId, uri, *this);
		sd->dispatchByServerIdSV(serverId, uri, *this);
	}else{
		sd->dispatchToServerRandom(to, uri, *this);
	}
	return 0;
}

void PRouter::gc(IPacketGc *pg){
	pg->gcRoutePacket(this);
}

// add by Grj
int PToServersRoute::emit(core::IConn *conn, core::IServerIdDispatcher* sd)
{
	PRouter obj;
	obj.from = this->from;
	obj.load = this->load;
	obj.ttl = this->ttl;
	obj.resCode = this->resCode;
	obj.ruri = this->ruri;
	obj.to = this->to;	
	
	sd->dispatchToServers(to, PRouter::uri, obj);
	return 0;
}

void PToServersRoute::gc(IPacketGc *pg)
{
	pg->gcRouteToSrvsPacket(this);
}

int PRoundRobinRouter::emit(core::IConn *conn, core::IServerIdDispatcher* sd)
{
	sd->DispatchToServerRoundRobin(m_router.to, m_router.uri, m_router);
	return 0;
}

void PRoundRobinRouter::gc(core::IPacketGc *pg)
{
	pg->GcRoundRobinPacket(this);
}
// end


int PServerIdRouter::emit(core::IConn *conn, core::IServerIdDispatcher* sd){
//	sd->dispatchByServerId(serverId, uri, *this);
	sd->dispatchByServerIdSV(serverId, uri, *this);
	return 0;
}

void PServerIdRouter::gc(IPacketGc *pg){
	pg->gcSPacket(this);
}

int PUidsRouter::emit(core::IConn *conn, core::IServerIdDispatcher* sd){
	sd->dispatchToServerRandom(SUFFIX_ROUTE, uri, *this);
	return 0;
}

void PUidsRouter::gc(IPacketGc *pg){
	pg->gcUPacket(this);
}


int PSS_SyncRouter::emit(core::IConn *conn, core::IServerIdDispatcher *sd)
{
    CReliableWriter *p = CReliableWriter::GetInstance();
    if (p != NULL)
        p->OnRouterEmit(this);
    return 0;
}

void PSS_SyncRouter::gc(core::IPacketGc *pg)
{
    pg->GcSyncPacket(this);
}

int PSS_AckRouter::emit(core::IConn *conn, core::IServerIdDispatcher *sd)
{
    sd->dispatchByServerIdSV(m_uDstId, uri, *this);
    return 0;
}

void PSS_AckRouter::gc(core::IPacketGc *pg)
{
    pg->GcAckPacket(this);
}

int PProxyRouter::emit(core::IConn *conn, core::IServerIdDispatcher *sd)
{
    sd->dispatchByServerIdSV(m_uServerId, uri, *this);
    return 0;
}

void PProxyRouter::gc(core::IPacketGc *pg)
{
    pg->GcProxyPacket(this);
}

