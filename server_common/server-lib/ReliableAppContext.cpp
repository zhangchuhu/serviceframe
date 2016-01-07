#include "ReliableAppContext.h"
#include "common/core/form.h"
#include "ReliableWriter.h"
#include "UrlHelper.h"
#include "server_common/protocol/prouter.h"
#include "core/sox/logger.h"
#include "common/protocol/const.h"
#include "server_common/server-lib/ICMPException.h"


#undef __CLASS__
#define __CLASS__ "CReliableAppContext"
#include "server_common/server-lib/Logger.h"

using namespace sox;
using namespace core;
using namespace common::router;
using namespace server::router;

BEGIN_FORM_MAP(CReliableAppContext)
    ON_LINK(PSS_SyncRouter, &CReliableAppContext::OnSyncRouter)
END_FORM_MAP()


inline const char *GetReliableType(ENUM_RELIABLE_TYPE enType)
{
    switch (enType)
    {
        case ENUM_RELIABLE_TYPE_RESET:
            return "reset";
        case ENUM_RELIABLE_TYPE_DATA:
            return "data";
        case ENUM_RELIABLE_TYPE_PING:
            return "ping";
		case ENUM_RELIABLE_TYPE_DATA_MULTI_ROUTE:
			return "multi route data";
		case ENUM_RELIABLE_TYPE_SHUTDOWN:
			return "shutdown";
		default:
			break;
    }

    return "unknown";
}

void CReliableAppContext::OnSyncRouter(PSS_SyncRouter *pMsg, IConn *conn)
{
    if (DoAckRouter(pMsg, conn))
    {
        PackBuffer buffer;
        Pack pack(buffer);
        pack.push_uint32(10 + pMsg->load.length());
        pack << pMsg->ruri << pMsg->resCode;
        pack.push(pMsg->load.data(), pMsg->load.length());
        handler->onData(pack.data(), pack.size(), conn, 0);
    }

	CWriterInfo &writerInfo = m_mapWriterInfo[pMsg->m_uSrcId];
    ABRouteWriter *aw = (ABRouteWriter *) writer;
	aw->RouteAck(pMsg, writerInfo.m_uExpectSeqId, pMsg->m_uBridgeId);
}


bool CReliableAppContext::DoAckRouter(PSS_SyncRouter *pMsg, IConn *conn)
{
	//initialize CWriteInfo in constructor
	CWriterInfo &writerInfo = m_mapWriterInfo[pMsg->m_uSrcId];

	//update last ping time when received packet from writer
	writerInfo.m_uLastPingTime = time(NULL);

	//send ack packet anyway
	//ABRouteWriter *aw = (ABRouteWriter *) writer;
	//aw->RouteAck(pMsg);


	//handle reset packet
    if (pMsg->m_enType == ENUM_RELIABLE_TYPE_RESET)
    {
		std::map<uint32_t, uint64_t>::iterator it;
		if (pMsg->m_ullSendTime != 0)
		{
			if (writerInfo.m_uLastRestTime < pMsg->m_ullSendTime)
			{
				writerInfo.m_uExpectSeqId = pMsg->m_uSeqId;
				writerInfo.m_uLastRestTime = pMsg->m_ullSendTime;

				FUNLOG(Warn, "from:%u to:%u seqid:%u type:%s time:%llu valid timestamp, reseted",
					pMsg->m_uSrcId, pMsg->m_uDstId, pMsg->m_uSeqId, GetReliableType(pMsg->m_enType), pMsg->m_ullSendTime);
			}
			else
			{
				FUNLOG(Warn, "from:%u to:%u seqid:%u type:%s time:%llu invalid timestamp, ignore",
					pMsg->m_uSrcId, pMsg->m_uDstId, pMsg->m_uSeqId, GetReliableType(pMsg->m_enType), pMsg->m_ullSendTime);
			}
		}
		return false;
    }

	//handle ping packet
	if (pMsg->m_enType == ENUM_RELIABLE_TYPE_PING)
	{
		FUNLOG(Debug, "from:%u to:%u seqid:%u type:%s time:%llu",
			pMsg->m_uSrcId, pMsg->m_uDstId, pMsg->m_uSeqId, GetReliableType(pMsg->m_enType), pMsg->m_ullSendTime);
		return false;
	}

	//handle data packet
	if (pMsg->m_enType == ENUM_RELIABLE_TYPE_DATA)
	{
		//我未reset，不接收任何数据报文
		//对方一定是持有老的svid向我发送报文
		if (0 == writerInfo.m_uLastRestTime)
		{
			FUNLOG(Debug, "from:%u to:%u seqid:%u type:%s time:%llu is not reset",
				pMsg->m_uSrcId, pMsg->m_uDstId, pMsg->m_uSeqId, GetReliableType(pMsg->m_enType), pMsg->m_ullSendTime);
			pMsg->m_enType = ENUM_RELIABLE_TYPE_SHUTDOWN;
			return false;
		}

    	if (pMsg->m_uSeqId == writerInfo.m_uExpectSeqId + 1)
    	{
			FUNLOG(Debug, "from:%u to:%u seqid:%u type:%s time:%llu seqid is sequent",
				pMsg->m_uSrcId, pMsg->m_uDstId, pMsg->m_uSeqId, GetReliableType(pMsg->m_enType), pMsg->m_ullSendTime);

        	writerInfo.m_uExpectSeqId = pMsg->m_uSeqId;
        	return true;
    	}
    	else if (pMsg->m_uSeqId > writerInfo.m_uExpectSeqId + 1)
    	{
			FUNLOG(Warn, "from:%u to:%u seqid:%u type:%s time:%llu seqid is not sequent, expect seqid is %u",
				pMsg->m_uSrcId, pMsg->m_uDstId, pMsg->m_uSeqId, GetReliableType(pMsg->m_enType), pMsg->m_ullSendTime, writerInfo.m_uExpectSeqId);

 			//1个连接上每出现100个乱序sync报文就告警
        	std::map<uint32_t, uint32_t>::iterator itmap = m_mapSrcSvidCount.find(pMsg->m_uSrcId);
        	if (itmap != m_mapSrcSvidCount.end())
        	{
				(itmap->second)++;
				if (100 < itmap->second)
				{
					WATCHDOGLOG(Warn, "seqid is not sequent! from:%u, seqid:%u", pMsg->m_uSrcId, pMsg->m_uSeqId);
					m_mapSrcSvidCount.erase(pMsg->m_uSrcId);
				}
        	}
        	else
        	{
				m_mapSrcSvidCount[pMsg->m_uSrcId] = 1;
        	}
			return false;
		}
    	else
    	{
			FUNLOG(Warn, "from:%u to:%u seqid:%u type:%s time:%llu seqid is too old, expect seqid is %u",
				pMsg->m_uSrcId, pMsg->m_uDstId, pMsg->m_uSeqId, GetReliableType(pMsg->m_enType), pMsg->m_ullSendTime, writerInfo.m_uExpectSeqId);
			return false;
    	}
	}

	//handle data packet
	if (pMsg->m_enType == ENUM_RELIABLE_TYPE_DATA_MULTI_ROUTE)
	{
		//我未reset，不接收任何数据报文
		//对方一定是持有老的svid向我发送报文
		if (0 == writerInfo.m_uLastRestTime)
		{
			FUNLOG(Debug, "from:%u to:%u seqid:%u type:%s time:%llu is not reset",
				pMsg->m_uSrcId, pMsg->m_uDstId, pMsg->m_uSeqId, GetReliableType(pMsg->m_enType), pMsg->m_ullSendTime);
			pMsg->m_enType = ENUM_RELIABLE_TYPE_SHUTDOWN;
			return false;
		}

		if (pMsg->m_uSeqId == writerInfo.m_uExpectSeqId + 1)
		{
			FUNLOG(Debug, "from:%u to:%u seqid:%u type:%s time:%llu seqid is sequent",
				pMsg->m_uSrcId, pMsg->m_uDstId, pMsg->m_uSeqId, GetReliableType(pMsg->m_enType), pMsg->m_ullSendTime);

			writerInfo.m_uExpectSeqId = pMsg->m_uSeqId;
			return true;
		}
		else if (pMsg->m_uSeqId > writerInfo.m_uExpectSeqId + 1)
		{
			FUNLOG(Warn, "from:%u to:%u seqid:%u type:%s time:%llu seqid is not sequent, expect seqid is %u",
				pMsg->m_uSrcId, pMsg->m_uDstId, pMsg->m_uSeqId, GetReliableType(pMsg->m_enType), pMsg->m_ullSendTime, writerInfo.m_uExpectSeqId);

 			//1个连接上每出现100个乱序sync报文就告警
			std::map<uint32_t, uint32_t>::iterator itmap = m_mapSrcSvidCount.find(pMsg->m_uSrcId);
			if (itmap != m_mapSrcSvidCount.end())
			{
				(itmap->second)++;
				if (100 < itmap->second)
				{
					WATCHDOGLOG(Warn, "seqid is not sequent! from:%u, seqid:%u", pMsg->m_uSrcId, pMsg->m_uSeqId);
					m_mapSrcSvidCount.erase(pMsg->m_uSrcId);
				}
			}
			else
			{
				m_mapSrcSvidCount[pMsg->m_uSrcId] = 1;
			}
			return false;
		}
		else
		{
			FUNLOG(Warn, "from:%u to:%u seqid:%u type:%s time:%llu seqid is duplicated or invalid, expect seqid is %u",
				pMsg->m_uSrcId, pMsg->m_uDstId, pMsg->m_uSeqId, GetReliableType(pMsg->m_enType), pMsg->m_ullSendTime, writerInfo.m_uExpectSeqId);
			return false;
		}
	}

    return false;
}

void CReliableAppContext::onRefreshSV()
{
}

void CReliableAppContext::onRecoverRefreshSV()
{
}

void CReliableAppContext::onAddSV(const sdaemon::FavorStrSV & fs)
{
}

void CReliableAppContext::onServerRemovedSV(const sdaemon::FavorStrSV & fs)
{
	/* do nothing here!
    std::map<uint32_t, uint32_t>::iterator iter;
    if ((iter = m_mapSrcSeqId.find(fs.serverId)) != m_mapSrcSeqId.end())
    {
        m_mapSrcSeqId.erase(iter);
    }*/
}

void CReliableAppContext::onServerStatusChangeSV(const sdaemon::FavorStrSV & fs)
{
}

void CReliableAppContext::watchdog()
{
	m_mapSrcSvidCount.clear();
}

void CReliableAppContext::CheckExpiredWriter()
{
	uint32_t uNow = time(NULL);
	int32_t iTimeCost = 0;

	for (std::map<uint32_t, CWriterInfo>::iterator it = m_mapWriterInfo.begin(); it != m_mapWriterInfo.end(); )
	{
		iTimeCost = uNow - it->second.m_uLastPingTime;
		if (iTimeCost > 3600 * 2)
		{
			FUNLOG(Warn, "src:%u expired", it->first);
			m_mapWriterInfo.erase(it++);
		}
		else
		{
			++it;
		}
	}
}


