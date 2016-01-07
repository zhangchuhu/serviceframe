#include "ReliableSendQueue.h"

using namespace core;
using namespace server::router;


#undef __CLASS__
#define __CLASS__ "CReliableSendQueue"
#include "Logger.h"


CReliableSendQueue::CReliableSendQueue()
{
    m_uSrcId = 0;
    m_uDstId = 0;
    m_uCurrSeqId = 0;
    m_nQueueSize = 0;
    m_bIsReset = false;
    m_bIsUrgentMode = false;
	m_uLastAckTime = time(NULL);
    m_iterNotSent = m_msgQueue.begin();
}

CReliableSendQueue::~CReliableSendQueue()
{
}

void CReliableSendQueue::Try(IServerIdDispatcher *sDispatcher)
{
    if (m_nQueueSize > 0)
    {
        FUNLOG(Debug, "src:%u dst:%u queue not empty: %u, retry now",  m_uSrcId, m_uDstId, m_nQueueSize);
        PSS_SyncRouter &objMsg = m_msgQueue.front();
        sDispatcher->dispatchByServerIdSV(m_uDstId, PSS_SyncRouter::uri, objMsg);
    }
}

void CReliableSendQueue::Ping(IServerIdDispatcher *sDispatcher)
{
	PSS_SyncRouter objMsg;
	objMsg.m_uSeqId = 0;	//useless, just set zero
	objMsg.m_uSrcId = m_uSrcId;
	objMsg.m_uDstId = m_uDstId;
	objMsg.m_enType = ENUM_RELIABLE_TYPE_PING;	//ping packet

	sDispatcher->dispatchByServerIdSV(m_uDstId, PSS_SyncRouter::uri, objMsg);
}

//while send to a server firstly, the sendqueue is reseted,
//no data will be sent until the reset ack arrive.
void CReliableSendQueue::Reset(IServerIdDispatcher *sDispatcher)
{
	uint64_t ullResetTime = 0;
	//identify reset ack not arrive
    SetReset(false);

    PSS_SyncRouter objMsg(ENUM_RELIABLE_TYPE_RESET);
    objMsg.m_uSrcId = m_uSrcId;
    objMsg.m_uDstId = m_uDstId;
    objMsg.m_uSeqId = 0;

	if(!getNowTime(ullResetTime))
	{
		log(Warn, "[CReliableSendQueue::%s]Failed to get reset timestamp!!!\n", __FUNCTION__);
	}
	objMsg.m_ullSendTime = ullResetTime;
	
    FUNLOG(Debug, "src:%u dst:%u seqid:%u time:%llu type:reset", m_uSrcId, m_uDstId, objMsg.m_uSeqId, objMsg.m_ullSendTime);
    sDispatcher->dispatchByServerIdSV(m_uDstId, PSS_SyncRouter::uri, objMsg);
    
    //insert into front, all packets in queue become not sent, except this reset
    m_iterNotSent = m_msgQueue.insert(m_msgQueue.begin(), objMsg);
    m_nQueueSize++;
    ++m_iterNotSent;
}

//push sync packet into sendqueue
void CReliableSendQueue::Push(const PSS_SyncRouter *pMsg)
{
	uint64_t ullSendTime = 0;
	
    PSS_SyncRouter objMsg(*pMsg);
    objMsg.m_uSeqId = ++m_uCurrSeqId;
    objMsg.m_uSrcId = m_uSrcId;
    objMsg.m_uDstId = m_uDstId;

	if(!getNowTime(ullSendTime))
	{
		log(Warn, "[CReliableSendQueue::%s]Failed to get data timestamp!!!\n", __FUNCTION__);
	}
	objMsg.m_ullSendTime = ullSendTime;

	//sendqueue is too large, enter urgent mode, send packet one by one
    if (m_nQueueSize > 10000)
    {
        SetUrgentMode(true);
        FUNLOG(Warn, "queue size:%u, enter urgent mode!", m_nQueueSize);
    }

	//sendqueue size exceed limitation, discard eldest packet
    if (m_nQueueSize >= m_nMaxQueueSize)
    {
        FUNLOG(Error, "queue too big:%u, drop front message!", m_nQueueSize);
        m_msgQueue.pop_front();
        m_nQueueSize--;
    }

    //insert sync into the tail of sendqueue
    SendQueueType::iterator iter = m_msgQueue.insert(m_msgQueue.end(), objMsg);
    m_nQueueSize++;
    FUNLOG(Debug, "src:%u dst:%u seqid:%u time:%llu", m_uSrcId, m_uDstId, objMsg.m_uSeqId, objMsg.m_ullSendTime);

    //all packets sent, waiting for ack; this new packet become not sent
    if (m_iterNotSent == m_msgQueue.end())
    {
        m_iterNotSent = iter;
    }
}

//flush sendqueue
void CReliableSendQueue::Flush(IServerIdDispatcher *sDispatcher)
{
    //no data will be sent until the reset ack arrive.
    if (!IsReset() || m_iterNotSent == m_msgQueue.end())
    {
        return;
    }
    
    //sendqueue is too large, enter urgent mode, send packet one by one
    if (m_nQueueSize > 10000)
    {
        FUNLOG(Error, "queue too big:%u, enter urgent mode!", m_nQueueSize);
        SetUrgentMode(true);
    }
    
    //in urgent mode, send only one packet per time
    if (IsUrgentMode())
    {
        FUNLOG(Debug, "src:%u dst:%u seqid:%u type:data mode:urgent!", m_uSrcId, m_uDstId, m_iterNotSent->m_uSeqId);
        sDispatcher->dispatchByServerIdSV(m_uDstId, PSS_SyncRouter::uri, *m_iterNotSent);
        ++m_iterNotSent;
    }
    //in normal mode, send max 100 packets
    else
    {
        for (int i = 0; m_iterNotSent != m_msgQueue.end() && i < 100; ++m_iterNotSent, i++)
        {
            FUNLOG(Debug, "src:%u dst:%u seqid:%u type:data", m_uSrcId, m_uDstId, m_iterNotSent->m_uSeqId);
            sDispatcher->dispatchByServerIdSV(m_uDstId, PSS_SyncRouter::uri, *m_iterNotSent);
        }
    }
}

//erase sendqueue
void CReliableSendQueue::Erase(const PSS_AckRouter *pAck)
{
	if(ENUM_RELIABLE_TYPE_DATA_MULTI_ROUTE == pAck->m_enType)
	{
		FUNLOG(Debug, "src:%u dst:%u seqid:%u expectid:%u type:multi route ack-%d", m_uSrcId, m_uDstId, pAck->m_uSeqId, pAck->m_uExpectId, pAck->m_enType);	
	}
	else
	{
		FUNLOG(Debug, "src:%u dst:%u seqid:%u expectid:%u type:ack-%d", m_uSrcId, m_uDstId, pAck->m_uSeqId, pAck->m_uExpectId, pAck->m_enType);
	}

	//ping报文的ack不用处理
	if (ENUM_RELIABLE_TYPE_PING == pAck->m_enType)
	{
		return;
	}

	//对方已经reset，要求shutdown连接
	if (ENUM_RELIABLE_TYPE_SHUTDOWN == pAck->m_enType)
	{
		//设置reset标志，阻止发送该队列的报文
		SetReset(false);
		return;
	}

    if (m_nQueueSize == 0)
    {
        return;
    }

	//get the first sync of sendqueue
	std::list<PSS_SyncRouter>::iterator it = m_msgQueue.begin();

	//new mode except reset ack
	if ((pAck->m_uExpectId != 0) && IsReset())
	{
		bool bErased = false;
		if (it != m_msgQueue.end())
		{
			PSS_SyncRouter &sync = *it;
			if (sync.m_uSeqId < pAck->m_uSeqId)
			{
				//sync seq < ack seq, out of order, enter urgent mode
				FUNLOG(Warn, "src:%u dst:%u seqid is not sequent, enter urgent mode!", m_uSrcId, m_uDstId);
				m_iterNotSent = m_msgQueue.begin();
				SetUrgentMode(true);
			}
		}

		//check expectid of ack
		for (it = m_msgQueue.begin(); it != m_msgQueue.end(); )
		{
	    	PSS_SyncRouter &sync = *it;

	    	//sync seq <= ack expected seq, the sync has been received and the ack is on the way,erase it
			if (sync.m_uSeqId <= pAck->m_uExpectId)
			{
				FUNLOG(Info, "src:%u dst:%u seqid:%u is erased by expectid:%u", m_uSrcId, m_uDstId, sync.m_uSeqId, pAck->m_uExpectId);

				m_msgQueue.erase(it++);
				m_nQueueSize--;
				bErased = true;
			}
			else
			{
				break;
			}
		}

		//check expectid may erase some sync, so need to reset m_iterNotSent and urgentmode
		if (IsUrgentMode() && m_nQueueSize == 0)
		{
			FUNLOG(Info, "src:%u dst:%u queue empty, enter normal mode", m_uSrcId, m_uDstId);
			SetUrgentMode(false);
			m_iterNotSent = m_msgQueue.begin();
		}
		else if (IsUrgentMode() && bErased)
		{
			m_iterNotSent = m_msgQueue.begin();
		}
	}
	//old mode or reset ack
	else
	{
		if (it != m_msgQueue.end())
		{
	    	PSS_SyncRouter &sync = *it;
		    if (sync.m_uSeqId > pAck->m_uSeqId)
		    {
		    	//sync seq > ack seq, the ack is the duplicated ack of old sync which has been erased, ignore it
		    }
		    else if (sync.m_uSeqId < pAck->m_uSeqId)
		    {
		    	//sync seq < ack seq, out of order, enter urgent mode
		        FUNLOG(Warn, "src:%u dst:%u seqid is not sequent, enter urgent mode!", m_uSrcId, m_uDstId);
		        m_iterNotSent = m_msgQueue.begin();
		        SetUrgentMode(true);
		    }
			else if (sync.m_uSeqId == pAck->m_uSeqId && sync.m_enType == pAck->m_enType)
			{
		    	//sync seq == ack seq, and sync type == ack type, sync match ack,the sync has been received successfully,erase it
				m_msgQueue.erase(it);
				m_nQueueSize--;

				//reset ack, reset successfully
			    if (pAck->m_enType == ENUM_RELIABLE_TYPE_RESET && !IsReset())
			    {
			        FUNLOG(Info, "src:%u dst:%u seqid reseted to %u", m_uSrcId, m_uDstId, pAck->m_uSeqId);
			        SetReset(true);
			    }
		    
				//if sendqueue is empty, terminate urgent mode
			    if (IsUrgentMode() && m_nQueueSize == 0)
			    {
			        FUNLOG(Info, "src:%u dst:%u queue empty, enter normal mode", m_uSrcId, m_uDstId);
			        SetUrgentMode(false);
			    }
			}
			else
			{
				//sync seq == ack seq, and sync type != ack type,the ack is the duplicated reset ack,ignore it
			}
		}
	}
}

/**---------------------------------------------------------------------------
* Function	   : GetBlockSeqId
*
* Description  : This routine to get the blocked data sequence ids.
*
* @param	   : uSeqIdBegin - the first blocked sequence id.
*				uSeqIdEnd - the last blocked sequence id.
*				ullMaxBlockTime - the max blocked time.
*				
* @return	   : N/A
*---------------------------------------------------------------------------*/
void CReliableSendQueue::GetBlockSeqId(uint32_t &uSeqIdBegin, uint32_t &uSeqIdEnd, uint64_t &ullMaxBlockTime)
{
	uint64_t ullReqSendTime = 0;
	uint64_t ullNowTime = 0;

    /*1 - Check the queue size.*/
	if(0 == m_nQueueSize)
	{
		return;
	}

	/*2 - Get the all msg blocked.*/
	uSeqIdBegin = m_msgQueue.front().m_uSeqId;
	uSeqIdEnd = m_msgQueue.back().m_uSeqId;

	ullReqSendTime = m_msgQueue.front().m_ullSendTime;
	if(!getNowTime(ullNowTime))
	{
		FUNLOG(Warn, "Failed to get current timestamp!!!\n", NULL);
	}
	ullMaxBlockTime = ullNowTime - ullReqSendTime;
	
	#if 0
    /*2 - Get the first and last data squence id.*/
	SendQueueType::iterator itListQMsg = m_msgQueue.begin();
	for(; itListQMsg != m_msgQueue.end(); ++itListQMsg)
	{
		if(ENUM_RELIABLE_TYPE_DATA == itListQMsg->m_enType)
		{
			uSeqIdBegin = itListQMsg->m_uSeqId;
			uSeqIdEnd = m_msgQueue.back().m_uSeqId;
            break;
		}
	}
	#endif	
}

//Justin fang - TOOD:BUSYBOX R-TIIME
/**---------------------------------------------------------------------------
* Function	   : getNowTime
*
* Description  : The routine to get the current timestamp.
*
* @param	   : ullNowtime - the current timestamp got.
*				
* @return	   : true - success, false - faild.
*---------------------------------------------------------------------------*/
bool CReliableSendQueue::getNowTime(uint64_t &ullNowtime)
{
	bool bRes = false;
	struct timeval stTimeStamp;

	/*1 - Try max 3 times to get time of day.*/
	for(uint32_t i = 0; i < 3; i++)
	{
		if(0 == gettimeofday(&stTimeStamp, NULL))
		{
			bRes = true;
			break;
		}
		
		FUNLOG(Warn, "Failed to gettimeofday,retry(%u)!\n", (i+1));
	}

	if(bRes)
	{
		/*2 - Success to get the time stamp.*/
		ullNowtime = stTimeStamp.tv_sec;
	}
	else
	{
		FUNLOG(Warn, "Failed to get current timestamp!!!\n", NULL);
	}

	return bRes;
}

