#include "ImDaemonClient.h"
#include "id_def.h"
#include "server_common/protocol/PImBridge.h"
#include "server_common/protocol/prouter.h"

using namespace core;
using namespace server::imbridge;
using namespace server::router;


bool CImDaemonClient::dispatchByServerIdSV(uint32_t serverId, uint32_t uri, sox::Marshallable &obj)
{
	//log(Debug, "[CImDaemonClient::dispatchByServerIdSV] serverid %d, uri(0X%04x)", serverId, uri);

    bool bUseBridge = false;
    uint32_t uBridgeSid = 0;
    
    //don't use imbridge if dispatch to daemon_d
    if (serverId == 0)
    {
        return DaemonClient::dispatchByServerIdSV(serverId, uri, obj);
    }
    
    sdaemon::FavorStrSV fa = getServerSV(serverId);
    if (!server || fa.serverId == NONE_SERVER_ID)
    {
        return false;
    }

	//dispatch to imbridge himself, dispatch directly
	if (fa.sstr.compare(SUFFIX_IMBRIDGE) == 0)
	{
		bUseBridge = false;
	}
	else
	{
		sox::scope_lock sllk(m_MutexRoute);
		//find route to the group of this server
	    uint32_t uGid = fa.groupId;
	    MapGid2Route_T::iterator it = m_mapGid2Route.find(uGid);
	    if (it != m_mapGid2Route.end())
	    {
			//indirect route
	        if (it->second != 0)
	        {
	            bUseBridge = true;
	            uBridgeSid = it->second;
	        }
	    }
	    else
	    {
		    if (fa.sstr.compare(SUFFIX_IMBRIDGE) == 0)
		    {
		        bUseBridge = false;
		    }
		    //cross net, ask imbridge to forward
		    if (server->getNetType() == CTL && fa.dip == 0)
		    {
		        bUseBridge = true;
		    }
		    else if (server->getNetType() == CNC && fa.wip == 0)
		    {
		        bUseBridge = true;
		    }
	    }
    }

    if (bUseBridge)
    {
		std::vector<uint32_t> vecBridgeSids;
		if(PSS_SyncRouter::uri == uri)
		{
			PSS_SyncRouter &p = static_cast<PSS_SyncRouter &>(obj);		
			if(ENUM_RELIABLE_TYPE_DATA_MULTI_ROUTE == p.m_enType)
			{
				server::IServerSelector *pServerSelector = GetServerSelector();
				if(pServerSelector)
				{
					pServerSelector->HashMultiServer(server->getServerId() + serverId, 3, vecBridgeSids, SUFFIX_IMBRIDGE);
					if(!vecBridgeSids.empty())
					{
						if(!vecBridgeSids.empty())
						{
							std::ostringstream ostr;
							for(std::vector<uint32_t>::iterator it = vecBridgeSids.begin(); it != vecBridgeSids.end(); ++it)
							{
								ostr << *it <<" ";

								p.m_uBridgeId = *it;

								PSS_ForwardToServerEx objMsg;
								objMsg.m_uSvrId = serverId;
								objMsg.m_uUri = uri;
								sox::PacketToString(obj, objMsg.m_strCtx);
							
								PServerIdRouter pr;
								pr.from = getName();
								pr.packLoad(objMsg);
								pr.ruri = objMsg.uri;
								pr.resCode = RES_SUCCESS;
								pr.ttl = 2;
								DaemonClient::dispatchByServerIdSV(*it, pr.uri, pr);
							}
							
							log(Debug, "[CImDaemonClient::dispatchByServerIdSV] use multi bridges, uri(0X%04x), serverids(%s)", uri, ostr.str().c_str());
							
							return true;
						}			
					}			
				}
			}
		}
		else if(PSS_AckRouter::uri == uri)
		{
			PSS_AckRouter &p = static_cast<PSS_AckRouter &>(obj);	
			if(ENUM_RELIABLE_TYPE_DATA_MULTI_ROUTE == p.m_enType)
			{
				uBridgeSid = p.m_uBridgeId;
				//log(Debug, "[CImDaemonClient::dispatchByServerIdSV] reliable ack with serverid(%d)", uBridgeSid);
			}
		}
	
		PSS_ForwardToServerEx objMsg;
		objMsg.m_uSvrId = serverId;
		objMsg.m_uUri = uri;
		sox::PacketToString(obj, objMsg.m_strCtx);
		if (uBridgeSid != 0)
		{
			//log(Debug, "server:%s/%u cross net, route to imbridge:%d", fa.sstr.c_str(), serverId, uBridgeSid);

			PServerIdRouter pr;
			pr.from = getName();
			pr.packLoad(objMsg);
			pr.ruri = objMsg.uri;
			pr.resCode = RES_SUCCESS;
			pr.ttl = 2;
			return DaemonClient::dispatchByServerIdSV(uBridgeSid, pr.uri, pr);
		}
		else
		{
			DaemonClient::dispatchToServerRandom(SUFFIX_IMBRIDGE, PSS_ForwardToServerEx::uri, objMsg);
			//log(Debug, "server:%s/%u cross net, route to imbridge", fa.sstr.c_str(), serverId);
			return true;
		}
    }
    else
    {
        return DaemonClient::dispatchByServerIdSV(serverId, uri, obj);
    }
}
