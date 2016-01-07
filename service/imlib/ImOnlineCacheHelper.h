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
		// ��uidת��key
		std::string Uid2Key(uint32_t uid);

		// ��keyת��uid
		uint32_t Key2Uid(std::string key);

		// ��memcached�л�ȡimlinkd ID
		uint32_t GetImlinkdByUid(uint32_t uid);

		// ��memcached�л�ȡ�û�״̬
		protocol::E_IMSTATUS GetStatusByUid(uint32_t uid);

		// ��memcached�л�ȡ�û�״̬������imlinkd
		bool GetOnlineInfoByUid(uint32_t uid, uint32_t& linkdId, protocol::E_IMSTATUS& status);

		// ��memcached��������ȡ�û�״̬������imlinkd
		bool MultiGetOnlineInfoByKeys(
									const std::vector<std::string>& vecUids, 
									std::map<std::string, imonlinecache::COnlineInfo>& mapOnlineInfo
									);

		bool MultiGetOnlineInfoByKeys(const std::vector<uint32_t> &vecUid, std::vector<imonlinecache::COnlineInfo> &vecOnlineInfo);

		// ��memcached�в����û�״̬��Ϣ
		bool SetOnlineInfoByUid(mc::Memcached* mc, uint32_t uid, imonlinecache::COnlineInfo& obj);

		// ��memcached��ɾ���û���Ϣ
		bool DeleteOnlineInfoByUid(mc::Memcached* mc, uint32_t uid);

		bool DeleteOnlineInfoByKey(mc::Memcached* mc, std::string key);
	};
}


#endif
