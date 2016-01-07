#ifndef __APP_IMCHAT_DB_H_
#define __APP_IMCHAT_DB_H_
#include "common/core/ibase.h"
#include "common/core/iserver.h"
#include "server_common/server-lib/RouteTarget.h"
#include "app_protocol/app_protocol.h"
namespace app{
	namespace imchat{
		class ImChatDb : public core::PHClass,
			public core::IRouteTarget,
			public core::IServerIdDispatcherAware,
			public core::IServerAware
		{
		public:
			DECLARE_FORM_MAP
			void OnMsgArrival(uint32_t uid, protocol::PCS_ChatMsg* pObjMessage);
		};
	}
}
#endif