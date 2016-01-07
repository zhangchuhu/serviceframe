#pragma once
#include "common/core/ibase.h"
#include "server_common/server-lib/iclient.h"
#include "server_common/server-lib/RouteTarget.h"
#include "server_common/config/IServerConfig.h"
#include "common/core/iserver.h"
#include "common/res_code.h"
#include "core/sox/logger.h"
#include "common/protocol/PImLbs.h"

#include <iterator>
#include <vector>

namespace server
{
namespace imlbs
{
		class P2PLbs:
            public core::PHClass, 
            public core::IRouteTarget, 
            public core::IServerAware,
		    public core::IDaemonServerAwareW,
			public core::IServerIdDispatcherAware,
			public core::IDaemonClientAwareW
        {
            std::map<uint32_t, protocol::imlbs::PSS_BridgeRegister> m_mapP2PBridgeInfo;
            std::vector<protocol::imlbs::PSS_BridgeRegister> m_vecP2PBridgeInfo;

            void __ResortP2PBridge();
		public:

			DECLARE_FORM_MAP

            void OnGetBridge(protocol::imlbs::PCS_GetP2PBridge *pMsg, core::IConn *pConn);

			void OnBridgeRegister(const std::string &strKey, protocol::imlbs::PSS_BridgeRegister *pMsg);
			
			virtual void onRefreshSV() {};
			virtual void onRecoverRefreshSV() {};
			virtual void onAddSV(const sdaemon::FavorStrSV & fs) {};
			virtual void onServerRemovedSV(const sdaemon::FavorStrSV & fs);
			virtual void onServerStatusChangeSV(const sdaemon::FavorStrSV & fs) {};
			virtual void onServerRegistered() {};
		};
	}
}

