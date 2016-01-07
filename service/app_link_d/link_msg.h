#ifndef __LINK_MSG_H__
#define __LINK_MSG_H__
#include "common/core/ibase.h"
#include "common/core/iserver.h"
#include "server_common/server-lib/RouteTarget.h"

#include "app_protocol/app_protocol.h"
#include "link_online.h"

namespace server {
	namespace applinkd {
		class LinkOnline;
		class LinkMsg : public core::PHClass,
			public core::IRouteTarget{

		public:
			LinkMsg();
			DECLARE_FORM_MAP
			void OnChatMsgFromUser(uint32_t uUid, protocol::PCS_ChatMsg *pMsg);
			void OnChatMsgFromServer(protocol::PCS_ChatMsg *pMsg);
		};
	}
}
#endif