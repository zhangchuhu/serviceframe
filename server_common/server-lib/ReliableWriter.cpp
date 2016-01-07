#include "ReliableWriter.h"
#include "server_common/server-lib/id_def.h"
#include "server_common/protocol/WatchDog.h"
#include "server_common/protocol/daemon.h"
#include <stdio.h>
using namespace core;
using namespace server;
using namespace server::router;
using namespace sdaemon;

#undef __CLASS__
#define __CLASS__ "CReliableWriter"
#include "Logger.h"


BEGIN_FORM_MAP(CReliableWriter)
    ON_REQUEST(PSS_AckRouter, &CReliableWriter::OnAckRouter)
END_FORM_MAP()


static CReliableWriter *s_pWriterInstance = NULL;

void CReliableWriter::SetInstance(CReliableWriter *p)
{
    s_pWriterInstance = p;
}
CReliableWriter *CReliableWriter::GetInstance()
{
    return s_pWriterInstance;
}


CReliableWriter::CReliableWriter()
{
    m_nTimeout = 60000;
    select_timeout(60000);
}

void CReliableWriter::handle(int nEvent)
{
    uint32_t uSeqidEnd = 0;
    uint32_t uSeqidBegin = 0;
	uint64_t ullBlockTime = 0;
    CReliableSendQueue *pSendQueue = NULL;
	uint32_t uNow = time(NULL);

    if (nEvent == sox::SEL_TIMEOUT)
    {
        for (MapSendQueueType::iterator iter = m_mapSendQueue.begin(); iter != m_mapSendQueue.end(); )
        {
            pSendQueue = (CReliableSendQueue *)&(iter->second);
            int32_t iTimeCost = uNow - pSendQueue->GetLastAckTime();
			if (iTimeCost > 7200)
			{
				FUNLOG(Warn, "dst:%u expired", iter->first);
				m_mapSendQueue.erase(iter++);
				continue;
			}

            pSendQueue->Try(sDispatcher);
			//keep alive!
			pSendQueue->Ping(sDispatcher);

            /*Send out alarm if data queque is still not empty.*/
            uSeqidEnd = 0;
            uSeqidBegin = 0;
            ullBlockTime = 0;
            pSendQueue->GetBlockSeqId(uSeqidBegin, uSeqidEnd, ullBlockTime);
            if(((uSeqidEnd - uSeqidBegin > 15) && (ullBlockTime > 60))
           		|| (ullBlockTime > 300))
            {
				sdaemon::FavorStrSV objFavorSV = client->getServerSV(iter->first);
				if (NONE_SERVER_ID == objFavorSV.serverId)
				{
					FUNLOG(Warn, "the dst sid may be removed, wait 2 hours to delete the queue", NULL);
					++iter;
					continue;
				}

				uint32_t uDstSid = iter->first;
				uint32_t uBridgeIp = 0;
				uint16_t uBridgePort = 0;

				((core::DaemonClient*)client)->getBridgeByDstSid(uDstSid, uBridgeIp, uBridgePort);

				//compute ack bridge info
				uint32_t uAckBridgeId = hashTable.GetServer(iter->first + server->getGroupId());
				sdaemon::FavorStrSV objAckBridgeFS = client->getServerSV(uAckBridgeId);
				if (NONE_SERVER_ID == objAckBridgeFS.serverId)
				{
					++iter;
					continue;
				}

				uint32_t uAckBridgeIp = objAckBridgeFS.dip;
				uint16_t uAckBridgePort = objAckBridgeFS.port;

				//set log error
				bool bError = false;
				if ((ullBlockTime > 600) || ((uSeqidEnd - uSeqidBegin + 1) > 100))
				{
					bError = true;
				}

				ALMLOG(bError ? Error : Info, "[CReliableWriter::%s]Dest(sid-%u/%s,dip-%s,wip-%s,bridge-%s:%d, ackbridge-%s:%d):%u reliable msg(seqid(%u-%u)) blocked in queue %llu seconds!\n",
					__FUNCTION__, iter->first, objFavorSV.sstr.c_str(), sox::addr_ntoa(objFavorSV.dip).c_str(), sox::addr_ntoa(objFavorSV.wip).c_str(),
					sox::addr_ntoa(uBridgeIp).c_str(), uBridgePort, 
					sox::addr_ntoa(uAckBridgeIp).c_str(), uAckBridgePort, 
					(uSeqidEnd-uSeqidBegin+1), uSeqidBegin, uSeqidEnd, ullBlockTime);
            }

			++iter;
        }
        select_timeout(m_nTimeout);
    }
}

void CReliableWriter::OnRouterEmit(const PSS_SyncRouter *pMsg)
{
    MapSendQueueType::iterator iter;
    CReliableSendQueue *pQueue;

    if (pMsg->m_uDstId == (uint32_t) -1)
    {
        return;
    }
    if ((iter = m_mapSendQueue.find(pMsg->m_uDstId)) == m_mapSendQueue.end())
    {
        pQueue = &(m_mapSendQueue[pMsg->m_uDstId]);
        pQueue->SetSourceId(server->getServerId());
        pQueue->SetDestinationId(pMsg->m_uDstId);
        pQueue->SetSizeLimit(100000);
        pQueue->Reset(sDispatcher);
    }
    else
    {
        pQueue = &(iter->second);
    }

    pQueue->Push(pMsg);//放入队列
    pQueue->Flush(sDispatcher);
}

void CReliableWriter::OnAckRouter(PSS_AckRouter *pMsg)
{
    MapSendQueueType::iterator iter;

    if ((iter = m_mapSendQueue.find(pMsg->m_uSrcId)) == m_mapSendQueue.end())
    {
        return;
    }

	iter->second.SetLastAckTime(time(NULL));
    iter->second.Erase(pMsg);
    iter->second.Flush(sDispatcher);
}


void CReliableWriter::onServerRemovedSV(const sdaemon::FavorStrSV & fs)
{
	//do nothing
    //FUNLOG(Warn, "remove queue for server:%u", fs.serverId);
    //m_mapSendQueue.erase(fs.serverId);

	//update imbridge hash环
    if (fs.sstr == SUFFIX_IMBRIDGE)
    {
	    hashTable.RemoveServer(fs.serverId);
    }
}


void CReliableWriter::onAddSV(const sdaemon::FavorStrSV &fs)
{
    /*
    if (fs.serverId == server->getServerId() && fs.serverStatus == sdaemon::RUNNING)
    {
        FUNLOG(Warn, "add myself! reset all peer", NULL);
        for (MapSendQueueType::iterator iter = m_mapSendQueue.begin(); iter != m_mapSendQueue.end(); iter++)
        {
            iter->second.Reset(sDispatcher);
        }
    }*/
	//update imbridge hash环
    if (fs.sstr == SUFFIX_IMBRIDGE && fs.serverStatus == sdaemon::RUNNING)
    {
	    hashTable.AddServer(fs.serverId);
    }
}

void CReliableWriter::onRefreshSV()
{
	UserConsistentFnvHash tmphashTable;
	hashTable = tmphashTable;
    std::vector<sdaemon::FavorStrSV> servers = client->getServersSV();
    for (std::vector<sdaemon::FavorStrSV>::iterator iter = servers.begin(); iter != servers.end(); iter++)
    {
        CReliableWriter::onAddSV(*iter);
    }
}

void CReliableWriter::onRecoverRefreshSV()
{
    CReliableWriter::onRefreshSV();
}

void CReliableWriter::onServerStatusChangeSV(const sdaemon::FavorStrSV & fs)
{
    CReliableWriter::onAddSV(fs);
}

/**---------------------------------------------------------------------------
* Function	   : __RaiseAlarm
*
* Description  : This routine to send out an alarm to watchdog client.
*
* @param	   : uLevel - alarm level.
*				szFmt - variable argument
*				
* @return	   : N/A
*---------------------------------------------------------------------------*/
void CReliableWriter::__RaiseAlarm(int uLevel, const char *szFmt, ...)
{
    std::stringstream ssFrom;
    
	/*1 - Format the va log.*/
	char buf[1024];
	va_list va;
	va_start(va, szFmt);
	size_t n = vsnprintf(buf, sizeof(buf) - 1, szFmt, va);
	va_end(va);

	if (n > sizeof(buf))
	{
		return;
	}

	/*2 - Format the orignal log msg.*/
	server::PSS_ServerLog obj;
	obj.m_uLevel = uLevel;
	obj.m_strMsg.assign(buf, n);


	/*3 - Format the sent log msg.*/
	PRouter objPRouter;
    ssFrom<<(server->getName())<<"/"<<(server->getServerId());
	objPRouter.from = ssFrom.str();
	objPRouter.to = SUFFIX_WATCHDOG;
	objPRouter.ruri = PSS_ServerLog::uri;
	objPRouter.resCode = RES_SUCCESS;
	objPRouter.packLoad(obj);
	objPRouter.ttl = 2;

	/*4 - Send out the alarm to watchdog.*/
	sDispatcher->dispatchToServerRandom(SUFFIX_WATCHDOG, objPRouter.uri, objPRouter);
	
}

bool CReliableWriter::GetBlockById(uint32_t uDstId)
{
    uint32_t uSeqidEnd = 0;
    uint32_t uSeqidBegin = 0;
	uint64_t ullBlockTime = 0;
    CReliableSendQueue *pSendQueue = NULL;
	uint32_t uNow = time(NULL);

	MapSendQueueType::iterator iter = m_mapSendQueue.find(uDstId);
	if (iter != m_mapSendQueue.end())
	{
		pSendQueue = (CReliableSendQueue *)&(iter->second);
		int32_t iTimeCost = uNow - pSendQueue->GetLastAckTime();
		if (iTimeCost > 7200)
		{
			return false;
		}
	
		/*Send out alarm if data que is still not empty.*/
		uSeqidEnd = 0;
		uSeqidBegin = 0;
		ullBlockTime = 0;
		
		sdaemon::FavorStrSV objFavorSV = client->getServerSV(iter->first);
		if (NONE_SERVER_ID == objFavorSV.serverId)
		{
			return false;
		}

		pSendQueue->GetBlockSeqId(uSeqidBegin, uSeqidEnd, ullBlockTime);
		if(((uSeqidEnd - uSeqidBegin > 100) && (ullBlockTime > 60))
			|| (ullBlockTime > 600))
		{
			return true;
		}
		else
		{

			return false;
		}
	}

	return false;
}


