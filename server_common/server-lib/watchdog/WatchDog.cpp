#include "WatchDog.h"
#include "Notifier.h"
#include "core/sox/logger.h"
#include "core/form.h"
#include "server_common/server-lib/UrlHelper.h"


using namespace core;
using namespace protocol;
using namespace protocol::im;


const char *Status2String(sdaemon::ServerStatus status)
{
    switch (status)
    {
        case sdaemon::INITIAL:
            return "INITIAL";
        case sdaemon::WAITING:
            return "WAITING";
        case sdaemon::READY:
            return "READY";
        case sdaemon::RUNNING:
            return "RUNNING";
        default:
            return "";
    }
}

#define WATCHDOG_TIMER_INTERVAL 30 * 1000

CWatchDog::CWatchDog()
{
    m_uRandom = 0;
    select_timeout(5 * 1000);
}

void CWatchDog::onServerRegistered()
{
    NotifyAll("watchdog login!");
}

void CWatchDog::onRefreshSV()
{
    std::vector<sdaemon::FavorStrSV> sv = client->getServersSV();
    for (std::vector<sdaemon::FavorStrSV>::iterator i = sv.begin(); i != sv.end(); ++i)
    {
        if (i->serverStatus != sdaemon::RUNNING || m_mapRequest.find(i->sstr) == m_mapRequest.end())
        {
            continue;
        }
        CServerData &data = m_mapServerData[i->serverId];
        ((sdaemon::FavorStrSV &) data) = *i;
        data.m_uLastRecv = time(NULL);
    }
}

void CWatchDog::onRecoverRefreshSV()
{
    NotifyAll("watchdog recover!");
    onRefreshSV();
}

void CWatchDog::onAddSV(const sdaemon::FavorStrSV & fs)
{
    if (fs.serverStatus != sdaemon::RUNNING || m_mapRequest.find(fs.sstr) == m_mapRequest.end())
    {
        return;
    }
    CServerData &data = m_mapServerData[fs.serverId];
    data.m_uLastRecv = time(NULL);
    ((sdaemon::FavorStrSV &) data) = fs;
    NotifyAll("server added: %s[%u] dip:%s dip:%s port:%u status:%s",
        fs.sstr.c_str(), fs.serverId, sox::addr_ntoa(fs.dip).c_str(), sox::addr_ntoa(fs.wip).c_str(), fs.port, Status2String(fs.serverStatus));
}

void CWatchDog::onServerRemovedSV(const sdaemon::FavorStrSV & fs)
{
    m_mapServerData.erase(fs.serverId);
    NotifyAll("server removed: %s[%u] dip:%s dip:%s port:%u",
        fs.sstr.c_str(), fs.serverId, sox::addr_ntoa(fs.dip).c_str(), sox::addr_ntoa(fs.wip).c_str(), fs.port);
}

void CWatchDog::onServerStatusChangeSV(const sdaemon::FavorStrSV & fs)
{
    NotifyAll("server changed status: %s[%u] dip:%s dip:%s port:%u status:%s",
        fs.sstr.c_str(), fs.serverId, sox::addr_ntoa(fs.dip).c_str(), sox::addr_ntoa(fs.wip).c_str(), fs.port, Status2String(fs.serverStatus));
}

void CWatchDog::handle(int nEvent)
{
	if (nEvent != sox::SEL_TIMEOUT)
	{
		return;
    }

    
    time_t now = time(NULL);
    std::map<std::string, IRequestWrapper *>::iterator iter;
    bool bRand;
    m_uRandom % 2 ? bRand = true : bRand = false;
    m_uRandom++;

    for (std::map<uint32_t, CServerData>::iterator i = m_mapServerData.begin(); i != m_mapServerData.end(); ++i)
    {
        if ((iter = m_mapRequest.find(i->second.sstr)) == m_mapRequest.end())
        {
            continue;
        }

        int ip;
        i->second.dip ? ip = i->second.dip : i->second.wip;

        if (bRand)
        {
            //test wrapper
            log(Debug, "[CWatchDog::OnTimer]: %s[%u] %s:%u wrapper",
                i->second.sstr.c_str(), i->second.serverId, sox::addr_ntoa(ip).c_str(), i->second.port);
            IRequestWrapper *fWrapper = iter->second;
            (*fWrapper)(i->second.serverId);
            writer->flush();
        }
        else
        {
            //check timeout
            if (now - i->second.m_uLastRecv > 61)
            {
                NotifyAll("%s[%u] %s:%u may not work! no response for %u sec",
                    i->second.sstr.c_str(), i->second.serverId, sox::addr_ntoa(ip).c_str(), i->second.port, now - i->second.m_uLastRecv);
            }
            else
            {
                //log(Debug, "%s[%u] %s:%u last response for %u sec",
                //   i->second.sstr.c_str(), i->second.serverId, sox::addr_ntoa(ip).c_str(), i->second.port, now - i->second.m_uLastRecv);
            }
        }
    }

    select_timeout(WATCHDOG_TIMER_INTERVAL);
}

IWriter *CWatchDog::requestDispatch(Request &request, IConn *conn)
{
    int ip;
    uint32_t uSvrID;
    std::string strFrom = request.getKey();
    std::map<uint32_t, CServerData>::iterator i;
    std::map<uint32_t, IResponseWrapper *>::iterator iter;


    if (DomainName::getSid(strFrom, uSvrID) && (i = m_mapServerData.find(uSvrID)) != m_mapServerData.end())
    {
        i->second.dip ? ip = i->second.dip : i->second.wip;
        log(Info, "[CWatchDog::requestDispatch]: response from: %s[%u] %s:%u, uri: %u/%u",
            i->second.sstr.c_str(), uSvrID, sox::addr_ntoa(ip).c_str(), i->second.port, request.getUri() & 0xff, request.getUri() >> 8);
        i->second.m_uLastRecv = time(NULL);


        if ((iter = m_mapResponse.find(request.getUri())) != m_mapResponse.end())
        {
            IResponseWrapper *fWrap = iter->second;
            (*fWrap)(request.getPackData());
            return writer;
        }
    }
    
    return BRouteAppContext::requestDispatch(request, conn);
}

