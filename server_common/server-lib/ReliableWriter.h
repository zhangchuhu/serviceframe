#pragma once
#include <map>
#include <stdarg.h>
#include "ReliableSendQueue.h"
#include "common/core/form.h"
#include "common/core/ibase.h"
#include "common/core/ilink.h"
#include "common/core/iserver.h"
#include "core/sox/snox.h"
#include "server_common/server-lib/BRouteWriter.h"
#include "server_common/protocol/prouter.h"
#include "server_common/server-lib/iclient.h"
#include "server_common/server-lib/DaemonClient.h"
#include "server_common/server-lib/ConsistentFnvHash.h"

namespace core
{

    #define ALMLOG(level, fmt,...) \
      do{CReliableWriter::__RaiseAlarm(level, fmt, __VA_ARGS__);\
         log(level, fmt, __VA_ARGS__);}while(0)
         
    typedef std::map<uint32_t, CReliableSendQueue> MapSendQueueType;

    class CReliableWriter:
        public PHClass,
        public sox::Handler,
        public IDaemonClientAwareW,
        public IDaemonServerAware,
        public IServerIdDispatcherAware,
        public IWriterAware
    {
    public:
        DECLARE_FORM_MAP

        static void SetInstance(CReliableWriter *p);
        static CReliableWriter *GetInstance();

        CReliableWriter();

        //for timer keep alive
        virtual void handle(int nEvent);

        //when PSS_SyncRouter call emit()
        virtual void OnRouterEmit(const server::router::PSS_SyncRouter *pMsg);

        //reliable route response
        virtual void OnAckRouter(server::router::PSS_AckRouter *pMsg);

        //inherit from DaemonClientW
        virtual void onRefreshSV();

        virtual void onRecoverRefreshSV();

        virtual void onAddSV(const sdaemon::FavorStrSV & fs);

        virtual void onServerRemovedSV(const sdaemon::FavorStrSV & fs);

        virtual void onServerStatusChangeSV(const sdaemon::FavorStrSV & fs);

        void __RaiseAlarm(int uLevel, const char *szFmt, ...);

        bool GetBlockById(uint32_t uDstId);

    private:
        int m_nTimeout;
        std::map<uint32_t, CReliableSendQueue> m_mapSendQueue;//keyÊÇ·þÎñÆ÷ID
        UserConsistentFnvHash  	hashTable;
    };
}
