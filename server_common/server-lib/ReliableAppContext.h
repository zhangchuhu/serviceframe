#pragma once
#include <map>
#include "common/core/ibase.h"
#include "common/core/iserver.h"
#include "core/corelib/MfcAppContext.h"
#include "common/protocol/prouter.h"
#include "server_common/protocol/prouter.h"
#include "server_common/server-lib/iclient.h"
#include "server_common/server-lib/ReliableWriter.h"
#include "server_common/server-lib/AutoTimer.h"


namespace core
{
	#define WATCHDOGLOG(level, fmt,...) \
      do{m_pReliableWriter->__RaiseAlarm(level, fmt, __VA_ARGS__);\
         log(level, fmt, __VA_ARGS__);}while(0)

	struct CWriterInfo
	{
		uint32_t m_uExpectSeqId;
		uint32_t m_uLastPingTime;
		uint64_t m_uLastRestTime;

		CWriterInfo() :
			m_uExpectSeqId(0),
			m_uLastPingTime(0),
			m_uLastRestTime(0)
		{
		}
	};

	class CReliableAppContext:
        public PHClass,
        public IAppContextAware,
        public IDaemonServerAware,
        public IDaemonClientAwareW,
        public ILinkHandlerAware,
        public IWriterAware
    {
	public:
		DECLARE_FORM_MAP
		CReliableAppContext()
		{
		    m_checkTimer.Init(this, &CReliableAppContext::watchdog);
		    m_checkTimer.Start(3600 * 1000);
		    m_mapSrcSvidCount.clear();

			m_expireTimer.Init(this, &CReliableAppContext::CheckExpiredWriter);
			//接收方先不启用过期机制
			//m_expireTimer.Start(60 * 1000);
	    }

        virtual void OnSyncRouter(server::router::PSS_SyncRouter *p, IConn *conn);

        bool DoAckRouter(server::router::PSS_SyncRouter *p, IConn *conn);

        //inherit from DaemonClientW
        virtual void onRefreshSV();
        virtual void onRecoverRefreshSV();
        virtual void onAddSV(const sdaemon::FavorStrSV & fs);
        virtual void onServerRemovedSV(const sdaemon::FavorStrSV & fs);
        virtual void onServerStatusChangeSV(const sdaemon::FavorStrSV & fs);
        void setReliableWriter(CReliableWriter *p)
        {
			m_pReliableWriter = p;
        }
        void watchdog();

		void CheckExpiredWriter();

    private:
        //std::map<uint32_t, uint32_t> m_mapSrcSeqId;
		//std::map<uint32_t, uint64_t> m_mapSrcResetTime;

		/* serverid => writer info */
		std::map<uint32_t, CWriterInfo> m_mapWriterInfo;

		CReliableWriter*             m_pReliableWriter;

		std::map<uint32_t, uint32_t> m_mapSrcSvidCount;

		CAutoTimer m_checkTimer;
		CAutoTimer m_expireTimer;
	};	
}

