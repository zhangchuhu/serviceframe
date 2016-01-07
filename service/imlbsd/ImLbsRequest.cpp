#include "ImLbsRequest.h"
#include "common/core/form.h"
#include "common/version.h"
#include "server_common/server-lib/id_def.h"
#include "server_common/protocol/daemon.h"
#include "server_common/protocol/plogin.h"
#include "core/sox/snox.h"
#include "server_common/server-lib/CookieHelper.h"
#include <fstream>
#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/bn.h>
#include <openssl/rsa.h>
#include "server_common/protocol/daemon.h"
#include <sstream>
#include "server_common/protocol/pimhashset.h"
#include "server_common/server-lib/DaemonClient.h"
#include "LinkdManager.h"

using namespace core;
using namespace server::imlbs;
using namespace protocol::login;
using namespace sdaemon;
using namespace server;
using namespace std;
using namespace protocol::im;
using namespace protocol::imlbs;

//当randomImLinkd的重试次数
#define RANDOMLINK_RETRY 3

BEGIN_FORM_MAP(ImLbsRequest)
	ON_LINK(PExchangeKey, &ImLbsRequest::onExchangeKey)
	ON_REQUEST(PSS_ImLinkdUpdate, &ImLbsRequest::OnLinkdUpdate)
	ON_REQUEST(PSS_ImLinkdPorts, &ImLbsRequest::OnImLinkdSameGroup) 
	ON_REQUEST(PImLinkdPorts, &ImLbsRequest::OnImlinkdSameISP) 
	ON_REQUEST(PGetImLinkdInfo, &ImLbsRequest::OnGetImLinkdInfo)
END_FORM_MAP()


void ImLbsRequest::onExchangeKey(PExchangeKey *ex, IConn *conn) {
	log(Debug, "exchanged key.");

	PExchangeKeyRes rc4key;
	std::string rc4 = rc4Helper::genRC4Key();
	rc4key.encSessionKey = rc4Helper::encRC4Key(ex->publicKey, ex->e, rc4);

	Sender ss(rc4key.uri, rc4key);
	conn->send(ss);

	conn->setEncKey((const unsigned char *)rc4.data(), DEF_SESSIONKEY_LENGTH);
}

// imlinkd 更新自己的在线人数
void ImLbsRequest::OnLinkdUpdate(protocol::imlbs::PSS_ImLinkdUpdate* obj)
{
	//m_pLinkdManager->SetImLinkdInfo(obj->m_srvId, obj->m_onlineNum);

#ifdef IMLBS_PING_DEBUG
	log (Info, "[ImLbsRequest::OnLinkdUpdate] Imlinkd(%d)-online(%d).", obj->m_srvId, obj->m_onlineNum);
#endif 
}

// 旧协议，强制登陆
void ImLbsRequest::OnGetImLinkdInfo(protocol::login::PGetImLinkdInfo *p)
{
	PGetImLinkdInfoRes res_pack;

	if (m_pLinkdManager->GetImlinkInfo(res_pack.ip, res_pack.ports))
	{
		stringstream ss;
		ss << "ip(" << res_pack.ip << ")-ports: ";
		for (vector<uint16_t>::iterator it = res_pack.ports.begin(); it != res_pack.ports.end(); ++it)
		{
			ss << *it << ", ";
		}
		log (Info, "[ImLbsRequest::OnGetImLinkdInfo]: user(%d) get imlinkd: %s", p->uid, ss.str().data());
		answer(PGetImLinkdInfoRes::uri, RES_SUCCESS, res_pack);
	}
	else
	{
		log (Error, "[ImLbsRequest::OnGetImLinkdInfo] user(%d) get imlinkd faild.", p->uid);
		answer(PGetImLinkdInfoRes::uri, RES_ENONEXIST);
	}
}

// 同组imlinkd 上报自己的IP信息
void ImLbsRequest::OnImLinkdSameGroup(PSS_ImLinkdPorts *pq)
{
	if (pq->m_uIp != sox::aton_addr("119.145.146.219"))
	{
		return;
	}

	log(Info, "[ImLbsRequest::OnImLinkdSameGroup]: serverid(%d)-onlieNum(%d)", pq->m_uLinkdId, pq->m_uOnline);
	for (std::vector<uint16_t>::iterator it = pq->m_vecPorts.begin(); it != pq->m_vecPorts.end(); ++it)
	{
		log(Info, "[ImLbsRequest::OnImLinkdSameGroup]: %d", *it);
	}

	m_pLinkdManager->SetImLinkdInfoByGroup(pq->m_uLinkdId, pq->m_uOnline, pq->m_uIp, pq->m_vecPorts);
}

// 同ISP的imlinkd 上报自己的IP信息
void ImLbsRequest::OnImlinkdSameISP(sdaemon::PImLinkdPorts* pObj)
{
	return;

	log(Info, "[ImLbsRequest::OnImlinkdSameISP]: linkd info from same ISP: serverid(%d)", pObj->serverId);
	for (std::vector<uint16_t>::iterator it = pObj->ports.begin(); it != pObj->ports.end(); ++it)
	{
		log(Info, "[ImLbsRequest::OnImlinkdSameISP]: %d", *it);
	}

	FavorStrSV fs = client->getServerSV(pObj->serverId);
	if (fs.serverId == NONE_SERVER_ID)
	{
		log (Error, "[ImLbsRequest::OnImlinkdSameISP] server %d not exist.", fs.serverId);
		return;
	}
	else
	{
		string strIp;
		if (fs.dip != 0)
		{
			strIp = sox::addr_ntoa(fs.dip);
		}
		else if (fs.wip != 0)
		{
			strIp = sox::addr_ntoa(fs.wip);
		}
		else
		{
			return;
		}
		
		m_pLinkdManager->SetImLinkdInfoByISP(pObj->serverId, strIp, pObj->ports);
	}
}


uint32_t ImLbsRequest::GetGroupId()
{
	return server->getGroupId();
}


void ImLbsRequest::onServerRemovedSV(const sdaemon::FavorStrSV & fs)
{
	if (fs.sstr == SUFFIX_IMLINKD)
	{
		if (fs.groupId == server->getGroupId())
		{
			// 同组的imlinkd
			m_pLinkdManager->DelImLinkdByGroup(fs.serverId);
		}
		
		ISPType ftype = fs.dip == 0 ? CNC : CTL;
		if (server->getNetType() == ftype)
		{
			// 同ISP的imlinkd
			m_pLinkdManager->DelImLinkdByISP(fs.serverId);
		}
		
		string strIp;
		if (fs.dip != 0)
		{
			strIp = sox::addr_ntoa(fs.dip);
		}
		else if (fs.wip != 0)
		{
			strIp = sox::addr_ntoa(fs.wip);
		}
		log(Info, "[ImLbsRequest::onServerRemovedSV]: remove imlinkd %s ok.", strIp.data());

		if (m_pLinkdManager->m_mapLinkdInfo.empty())
		{
			log (Warn, "!!! no imlinkd in this imlbs: %s !!!", server->getIp().data());
			RouteLog(Warn, "!!! no imlinkd in this imlbs: %s !!!", server->getIp().data());
		}
	}
}

void ImLbsRequest::onAddSV(const sdaemon::FavorStrSV & fs)
{
	/*
	if(fs.sstr == SUFFIX_IMHASHSETINFO && fs.serverStatus == sdaemon::RUNNING)
	{
		RouteToHashSet();
	}
	*/
}

void ImLbsRequest::onRefreshSV()
{
	/*
	std::vector<sdaemon::FavorStrSV> ff = client->getServersSV();
	for(std::vector<sdaemon::FavorStrSV>::iterator it = ff.begin(); it != ff.end(); ++it)
	{
		onAddSV(*it);
	}
	*/
}


void ImLbsRequest::onRecoverRefreshSV()
{
	//m_imlinkIdHelper.clearServer();
	//onRefreshSV();
}

void ImLbsRequest::onServerStatusChangeSV(const sdaemon::FavorStrSV & fs)
{
	onAddSV(fs);
}


