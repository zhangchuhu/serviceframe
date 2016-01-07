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

		// ����ͬ�����ĳ�ַ���������ͬ����˫�߶�û�з�����������£�Ĭ�ϲ�ת��������
		// ����ת������������bSendDiff��true
		virtual void RouteSameNet(const std::string& strSuffix, uint32_t uri, const sox::Marshallable &obj,	bool bSendDiff = false);

		// ����ͬ�����imlinkd
		// ��ͬ����˫�߶�û�з�����������£�Ĭ��ת����imbridge
		virtual void RouteSameNetLinkd(uint32_t uUid, uint32_t uLinkdId, uint32_t uri, const sox::Marshallable &obj); 

	};
}

#endif 

