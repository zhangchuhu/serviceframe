#include "MultiImTransHelper.h"

using namespace server;

// 根据uid，所在imlinkd网络类型选取imtransfer，并发包
bool CMultiImTransHelper::_RouteByNetType(uint32_t uid, uint32_t netType, uint32_t uri, const sox::Marshallable &obj)
{
	uint32_t transId;
	{
		boost::mutex::scoped_lock s1(m_mutex);
		transId = CImTransHelper::GetTransferID(uid, netType);
	}

	if (transId != (uint32_t)-1)
	{
		routeS(transId, uri, obj);
		log (Debug, "[CMultiImTransHelper::_RouteByNetType]: Ok. uid(%u)-netType(%u)-transfer(%u)-uri(0x%x)", uid, netType, transId, uri);
		return true;
	}

	log (Error, "[CMultiImTransHelper::_RouteByNetType]: Faild. uid(%u)-netType(%u)-transfer(%u)-Uri(0x%x)", uid, netType, transId, uri);
	return false;
}

// 根据uid，所在imlinkd网络类型选取imtransfer，并发包
bool CMultiImTransHelper::_RouteBySrvId(uint32_t Uid, uint32_t linkdId, uint32_t Uri, const sox::Marshallable &obj)
{
	uint32_t transId;
	{
		boost::mutex::scoped_lock s1(m_mutex);
		uint32_t srvType = CImServersInfo::GetNetTypeByID(linkdId);
		transId = CImTransHelper::GetTransferID(Uid, srvType);
	}

	if (transId != (uint32_t)-1)
	{
		routeS(transId, Uri, obj);
		//log (Debug, "[CMultiImTransHelper::_RouteBySrvId]: Ok. uid(%u)-linkd(%u)-transfer(%u)-Uri(0x%x)", Uid, linkdId, transId, Uri);
		return true;
	}

	log (Error, "[CMultiImTransHelper::_RouteBySrvId]: Faild. uid(%u)-linkd(%u)-transfer(%u)-Uri(0x%x)", Uid, linkdId, transId, Uri);
	return false;
}
