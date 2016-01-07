#include "P2PLbs.h"
#include <sstream>
#include "server_common/server-lib/UrlHelper.h"
#include "common/core/form.h"
#include "server_common/server-lib/id_def.h"
#include "core/sox/snox.h"

#undef __CLASS__
#define __CLASS__ "P2PLbs"
#include "server_common/server-lib/Logger.h"

using namespace core;
using namespace server::imlbs;
using namespace protocol::imlbs;


BEGIN_FORM_MAP(P2PLbs)
	ON_LINK(PCS_GetP2PBridge, &P2PLbs::OnGetBridge)
	ON_KEYREQUEST(PSS_BridgeRegister, &P2PLbs::OnBridgeRegister)
END_FORM_MAP()


void P2PLbs::onServerRemovedSV(const sdaemon::FavorStrSV & fs)
{
    if (fs.sstr.compare(SUFFIX_P2PBRIDGE) == 0)
    {
        m_mapP2PBridgeInfo.erase(fs.serverId);
        FUNLOG(Info, "p2pbridge[%u] removed! wip:%s dip:%s", fs.serverId, sox::addr_ntoa(fs.wip).c_str(), sox::addr_ntoa(fs.dip).c_str());
        __ResortP2PBridge();
    }
}

void P2PLbs::__ResortP2PBridge()
{
	m_vecP2PBridgeInfo.clear();
	for (std::map<uint32_t, PSS_BridgeRegister>::iterator i = m_mapP2PBridgeInfo.begin(); i != m_mapP2PBridgeInfo.end(); ++i)
	{
		m_vecP2PBridgeInfo.push_back(i->second);
	}
}

void P2PLbs::OnBridgeRegister(const std::string &strFrom, PSS_BridgeRegister *pMsg)
{
    uint32_t uSvrID;
    if (!DomainName::getSid(strFrom, uSvrID))
    {
        return;
    }

    sdaemon::FavorStrSV fs = client->getServerSV(uSvrID);
    if (fs.serverId == NONE_SERVER_ID)
    {
        FUNLOG(Error, "bug? non-existing serverid:%u", uSvrID);
        return;
    }
    if (fs.sstr.compare(SUFFIX_P2PBRIDGE) != 0)
    {
        FUNLOG(Info, "server name != %s", fs.sstr.c_str());
        return;
    }

    m_mapP2PBridgeInfo[fs.serverId] = *pMsg;
    __ResortP2PBridge();
	FUNLOG(Info, "server:%u dip:%s wip:%s", uSvrID, pMsg->m_strCtlIP.data(), pMsg->m_strCncIP.data());
}


void P2PLbs::OnGetBridge(PCS_GetP2PBridge *pMsg, IConn *pConn)
{
    if (m_vecP2PBridgeInfo.size() == 0)
    {
        FUNLOG(Error, "no p2pbridge avaliable!", NULL);
        return;
    }

    std::vector<uint32_t>::size_type nIdx = rand() % m_vecP2PBridgeInfo.size();
	PSS_BridgeRegister &info = m_vecP2PBridgeInfo[nIdx];

    PCS_GetP2PBridgeRes objRes;
    objRes.m_strGUID = pMsg->m_strGUID;
    objRes.m_strDIP = info.m_strCtlIP;
    objRes.m_strWIP = info.m_strCncIP;
    objRes.m_vecPort = info.m_vecPort;
    
    answer(objRes.uri, RES_SUCCESS, objRes);
    FUNLOG(Info, "random p2pbridge: dip:%s wip:%s", objRes.m_strDIP.data(), objRes.m_strWIP.data());
}

