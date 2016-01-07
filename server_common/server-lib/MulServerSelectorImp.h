#pragma once 

#include "ServerSelectorImp.h"

#include <boost/thread/mutex.hpp>

namespace server
{
	class CRServerSelectorImp : public CServerSelectorImp
	{
		boost::mutex m_mutexSelector;

	public:
		CRServerSelectorImp() {};

		//--------------------------------------------------------------------------------------------------------------------------
		//本节为注册指定类型的服务器

		virtual void RegisterServerSuffix(const std::string &strSuffix, EHashServerValueType emValueType = ENUM_HASH_SERVERID, EHashFuntcionType emFuncType = ENUM_FUNC_MD5);

		//--------------------------------------------------------------------------------------------------------------------------
		//本节为关于服务器调整的相关虚接口

		virtual void onRefreshSV();
		virtual void onAddSV(const sdaemon::FavorStrSV & fs);
		virtual void onServerRemovedSV(const sdaemon::FavorStrSV & fs);
		virtual void onRecoverRefreshSV();
		virtual void onServerStatusChangeSV(const sdaemon::FavorStrSV & fs);

		//--------------------------------------------------------------------------------------------------------------------------
		//本节为随机选择一个需要的服务器

		virtual uint32_t RandomServer(const std::string &strSuffix);
		virtual uint32_t RandomServerByNet(const std::string &strSuffix, uint32_t uNetType = NET_TYPE_ANY);
		virtual uint32_t RandomServerByGroup(const std::string &strSuffix, uint32_t uGroupId);
		virtual uint32_t RandomServerByNetGroup(const std::string &strSuffix, uint32_t uNetType, uint32_t uGroupId);

		//--------------------------------------------------------------------------------------------------------------------------
		//本李为哈希选择一个需要的服务器

		virtual uint32_t HashServer(uint32_t uKey, const std::string &strSuffix);
		bool HashMultiServer(uint32_t uKey, uint32_t uValueCount, std::vector<uint32_t>& vecValues, const std::string &strSuffix);
		virtual uint32_t HashServerByNet(uint32_t uKey, const std::string &strSuffix, uint32_t uNetType = NET_TYPE_ANY);
		virtual uint32_t HashServerByGroup(uint32_t uKey, const std::string &strSuffix, uint32_t uGroupId);
		virtual uint32_t HashServerByNetGroup(uint32_t uKey, const std::string &strSuffix, uint32_t uNetType, uint32_t uGroupId);

		//--------------------------------------------------------------------------------------------------------------------------
	};


}
