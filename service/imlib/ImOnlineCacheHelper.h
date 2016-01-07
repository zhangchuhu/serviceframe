#ifndef CIMONLINECACHEHELPER_H
#define CIMONLINECACHEHELPER_H

#include <string>
#include <vector>
#include <map>
#include "protocol/const.h"
#include "server_common/dblib/Memcached.h"
#include "server_common/dblib/IMemcachedFactory.h"
#include "server_common/protocol/pimonline.h"
#include "server_common/protocol/PImOnlineCache.h"


#define IMONLINE_CACHED "imonline_cached"
#define ONLINECACHE_PREFIX "oc-"

namespace server
{
	class CImOnlineCacheHelper : public mc::MemcachedFactoryAware
	{
	public:
		// 将uid转成key
		std::string Uid2Key(uint32_t uid);

		// 将key转成uid
		uint32_t Key2Uid(std::string key);

		// 从memcached中获取imlinkd ID
		uint32_t GetImlinkdByUid(uint32_t uid);

		// 从memcached中获取用户状态
		protocol::E_IMSTATUS GetStatusByUid(uint32_t uid);

		// 从memcached中获取用户状态及所属imlinkd
		bool GetOnlineInfoByUid(uint32_t uid, uint32_t& linkdId, protocol::E_IMSTATUS& status);

		// 从memcached中批量获取用户状态及所属imlinkd
		bool MultiGetOnlineInfoByKeys(
									const std::vector<std::string>& vecUids, 
									std::map<std::string, imonlinecache::COnlineInfo>& mapOnlineInfo
									);

		bool MultiGetOnlineInfoByKeys(const std::vector<uint32_t> &vecUid, std::vector<imonlinecache::COnlineInfo> &vecOnlineInfo);

		// 向memcached中插入用户状态信息
		bool SetOnlineInfoByUid(mc::Memcached* mc, uint32_t uid, imonlinecache::COnlineInfo& obj);

		// 从memcached中删除用户信息
		bool DeleteOnlineInfoByUid(mc::Memcached* mc, uint32_t uid);

		bool DeleteOnlineInfoByKey(mc::Memcached* mc, std::string key);
	};
}


#endif
