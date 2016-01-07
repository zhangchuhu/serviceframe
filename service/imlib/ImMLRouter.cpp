#include "ImMLRouter.h"
#include "ImTransHelper.h"
#include "server_common/protocol/PImBridge.h"

using namespace std;
using namespace server;
using namespace core;
using namespace server::imbridge;

// 发往同网络的某种服务器，在同网或双线都没有服务器的情况下，默认不转发到异网
// 如需转发到异网，将bSendDiff置true
void ICImMLRouter::RouteSameNet(const std::string& strSuffix, uint32_t uri, const sox::Marshallable &obj, bool bSendDiff /* = false */)
{
	uint32_t netType = server->getNetType();
	uint32_t uSrvId = m_srvInfo->RandomServerIDByNameTypeEx(strSuffix, netType);
	if (uSrvId != (uint32_t)-1)
	{
		// 发往 同网络或双线 服务器
		routeS(uSrvId, uri, obj);
		log (Info, "[ICImMLRouter::RouteSameNet] route to server ok. toSrv(%s)-uri(0x%x)-netType(%d)-toSrvID(%u)", 
			strSuffix.data(), uri, (int32_t)netType, uSrvId);
	}
	else if (bSendDiff)
	{
		// 未找到同网的服务器， 转发到异网的
		route(strSuffix, uri, obj);
		log (Info, "[ICImMLRouter::RouteSameNet] route to different net ok. toSrv(%s)-uri(0x%x)-netType(%d)",
			strSuffix.data(), uri, (int32_t)netType);
	}
	else
	{
		log (Info, "[ICImMLRouter::RouteSameNet] not found server in same net. toSrv(%s)-uri(0x%x)-netType(%d)",
			strSuffix.data(), uri, (int32_t)netType);
	}
}


// 发往同网络的imlinkd
// 在同网或双线都没有服务器的情况下，默认由imbridge转发
void ICImMLRouter::RouteSameNetLinkd(uint32_t uUid, uint32_t uLinkdId, uint32_t uri, const sox::Marshallable &obj)
{
	uint32_t netType = m_srvInfo->GetNetTypeByID(uLinkdId);
	uint32_t myType = server->getNetType();

	if (myType == CNC|CTL || myType == netType)
	{
		routeU(uUid, uLinkdId, uri, obj);
		log (Info, "[ICImMLRouter::RouteSameNetLinkd] same net type. uid(%u)-linkd(%u)-uri(%u)-netType(%d)-myType(%d)",
			uUid, uLinkdId, uri, (int32_t)netType, (int32_t)myType);
	}
	else
	{
		PSS_ForwardBridge resObj(uri, uUid, uLinkdId, CImTransHelper::Pack2String(obj));
		route(SUFFIX_IMBRIDGE, PSS_ForwardBridge::uri, resObj);
		log (Info, "[ICImMLRouter::RouteSameNetLinkd] route to imbridge. uid(%u)-linkd(%u)-uri(%u)-netType(%d)-myType(%d)",
			uUid, uLinkdId, uri, (int32_t)netType, (int32_t)myType);
	}
}
