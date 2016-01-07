#ifndef CIM_MULTI_LINE_ROUTER
#define CIM_MULTI_LINE_ROUTER

#include "server_common/server-lib/RouteTarget.h"
#include "common/core/iserver.h"
#include "ImServersInfo.h"

namespace server
{
	class ICImMLRouter :
		public core::IRouteTarget,
		public ICImServersInfoAware,
		public core::IDaemonServerAwareW
	{
	public:

		// 发往同网络的某种服务器，在同网或双线都没有服务器的情况下，默认不转发到异网
		// 如需转发到异网，将bSendDiff置true
		virtual void RouteSameNet(const std::string& strSuffix, uint32_t uri, const sox::Marshallable &obj,	bool bSendDiff = false);

		// 发往同网络的imlinkd
		// 在同网或双线都没有服务器的情况下，默认转发到imbridge
		virtual void RouteSameNetLinkd(uint32_t uUid, uint32_t uLinkdId, uint32_t uri, const sox::Marshallable &obj); 

	};
}

#endif 

