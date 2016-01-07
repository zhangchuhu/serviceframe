#pragma once
#include <list>
#include "common/core/ibase.h"
#include "common/core/ilink.h"
#include "server_common/protocol/prouter.h"
#include "server_common/server-lib/BRouteWriter.h"

namespace core
{
    typedef std::list<server::router::PSS_SyncRouter> SendQueueType;

    class CReliableSendQueue
    {
    private:
        uint32_t m_uSrcId;
        uint32_t m_uDstId;
        uint32_t m_uCurrSeqId;
        bool m_bIsReset;
        bool m_bIsUrgentMode;
		uint32_t m_uLastAckTime;

        SendQueueType m_msgQueue;
        SendQueueType::size_type m_nQueueSize;
        SendQueueType::size_type m_nMaxQueueSize;
        SendQueueType::iterator m_iterNotSent;


    public:
        CReliableSendQueue();

        ~CReliableSendQueue();

        inline void SetSourceId(uint32_t uSrcId)
        {
            m_uSrcId = uSrcId;
        }

        inline void SetDestinationId(uint32_t uDstId)
        {
            m_uDstId = uDstId;
        }

		inline uint32_t GetDestinationId()
		{
			return m_uDstId;
		}

        inline void SetSizeLimit(SendQueueType::size_type nMax)
        {
            m_nMaxQueueSize = nMax;
        }

        inline bool IsReset()
        {
                return m_bIsReset;
        }

        inline void SetReset(bool bYes)
        {
                m_bIsReset = bYes;
        }

        inline bool IsUrgentMode()
        {
                return m_bIsUrgentMode;
        }

        inline void SetUrgentMode(bool bYes)
        {
                m_bIsUrgentMode = bYes;
        }

		inline uint32_t GetLastAckTime()
		{
			return m_uLastAckTime;
		}

		inline void SetLastAckTime(uint32_t AckTime)
		{
			m_uLastAckTime = AckTime;
		}

        void Try(IServerIdDispatcher *);

		void Ping(IServerIdDispatcher *sDispatcher);

        void Reset(IServerIdDispatcher *);

        void Push(const server::router::PSS_SyncRouter *);
        
        void Flush(IServerIdDispatcher *);
        
        void Erase(const server::router::PSS_AckRouter *);

        bool getNowTime(uint64_t &ullNowtime);
        void GetBlockSeqId(uint32_t &uSeqIdBegin, uint32_t &uSeqIdEnd, uint64_t &ullMaxBlockTime);
    };
};
