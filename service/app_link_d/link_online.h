#ifndef __LINK_ONLINE_H__
#define __LINK_ONLINE_H__
#include "common/core/ibase.h"
#include "common/core/iserver.h"
#include "common/core/ilink.h"
#include "server_common/server-lib/RouteTarget.h"

#include "app_protocol/app_protocol.h"
#include "iuid_dispatcher.h"
namespace server {
	namespace applinkd {

		struct ImLinkUser
		{
			ImLinkUser()  {}

			time_t time;
			uint32_t cid;                   //连接的ID, 由core::MultiConnManagerVecImp管理的std::vector<IConn *> 集合的索引
			uint32_t serialId;              //IConn的序列号
			protocol::APP_IMSTATUS status;    //登陆状态
		};
		class LinkOnline: public core::PHClass,
			public core::IRouteTarget,
			public core::IConnManagerAware,
			public IUidDispatcher,
			public core::IServerAware
		{
		public:

			DECLARE_FORM_MAP

			void onLoginIM(protocol::PLoginImLinkd *p, core::IConn *conn);

			virtual int dispatchByUid(uint32_t uid, core::Sender &s, bool bTcp);

			virtual int dispatchByUid(uint32_t uid, uint32_t uri, sox::Marshallable &obj, bool bTcp) ;

			virtual uint32_t getUidByCid(uint32_t cid);

		private:
			
			void _add_into_logineds(uint32_t uid, uint32_t serialid, uint32_t cid, protocol::APP_IMSTATUS status);

		private:
			typedef std::map<uint32_t, ImLinkUser> users_t;
			users_t logineds;
		};
	}
}
#endif