#ifndef IM_LBSREQUEST_H_
#define IM_LBSREQUEST_H_

#include "common/core/ibase.h"
#include "server_common/server-lib/iclient.h"
#include "server_common/server-lib/RouteTarget.h"
#include "server_common/config/IServerConfig.h"
#include "common/core/iserver.h"

#include "common/protocol/plogin.h"
#include "common/res_code.h"
#include "core/sox/logger.h"
#include "server_common/server-lib/RC4_Helper.h"
#include "imlib/ImLinkIDHelper.h"
#include "common/protocol/PImLbs.h"
#include "server_common/protocol/daemon.h"
//#include "LinkdManager.h"

#include <iterator>
#include <vector>

namespace server {
	namespace imlbs {

		class CLinkdManager;
		class ImLbsRequest : public core::PHClass, 
						public core::IRouteTarget, 
						public core::IServerAware,
						public core::IDaemonServerAwareW,
						public core::IServerIdDispatcherAware,
						public core::IDaemonClientAwareW{

			std::vector<uint32_t> imlinkd;	
			typedef std::map<uint32_t, std::vector<uint16_t> > srv_ports_t;
			srv_ports_t iml_ports; 

			CLinkdManager* m_pLinkdManager;

		public:

			DECLARE_FORM_MAP

			void onExchangeKey(protocol::login::PExchangeKey *ex, core::IConn *conn);

			// 旧登陆协议获取imlinkd信息
			void OnGetImLinkdInfo(protocol::login::PGetImLinkdInfo*);
			
			// 同组的imlinkd向自己注册
			void OnImLinkdSameGroup(protocol::imlbs::PSS_ImLinkdPorts *pq);

			// 同ISP的imlinkd向自己注册
			void OnImlinkdSameISP(sdaemon::PImLinkdPorts* pObj);

			// imlinkd 更新在线人数
			void OnLinkdUpdate(protocol::imlbs::PSS_ImLinkdUpdate* obj);

			void SetLinkdManager(CLinkdManager* p)
			{
				m_pLinkdManager = p;
			}

			uint32_t GetGroupId();
			

			virtual void onRefreshSV();
			virtual void onRecoverRefreshSV();
			virtual void onAddSV(const sdaemon::FavorStrSV & fs);
			virtual void onServerRemovedSV(const sdaemon::FavorStrSV & fs);
			virtual void onServerStatusChangeSV(const sdaemon::FavorStrSV & fs);

			virtual void onServerRegistered(){}
		};
	}
}

#endif /*LBSREQUEST_H_*/
