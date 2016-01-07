#include <sstream>

#include "ImOnlineCacheHelper.h"
#include "server_common/protocol/pimonline.h"
#include "server_common/helper/server_const.h"
#include "core/sox/logger.h"

using namespace std;
using namespace server;
using namespace server::mc;
using namespace server::imonlinecache;


// ��memcached�л�ȡimlinkd ID
uint32_t CImOnlineCacheHelper::GetImlinkdByUid(uint32_t uid)
{
	uint32_t linkdId = NONE_IMLINKD;
	string key = Uid2Key(uid);

	COnlineInfo p;
	Memcached *mc = getMemcachedFactory()->getMemcached(IMONLINE_CACHED);
	if (mc->mcGet(key, p))
	{
		linkdId = p.m_linkdId;	
		log(Debug, "[CImOnlineCacheHelper::GetImlinkdByUid]: user: %u is on imlind: %u !", uid, linkdId);
	}
	else
	{
		log(Error, "[CImOnlineCacheHelper::GetImlinkdByUid]: user: %u not found !", uid);
	}
	
	return linkdId;
}

// ��memcached�л�ȡ�û�״̬
protocol::E_IMSTATUS CImOnlineCacheHelper::GetStatusByUid(uint32_t uid)
{
	protocol::E_IMSTATUS status = protocol::IMOFFLINE;
	string key = Uid2Key(uid);

	COnlineInfo p;
	Memcached *mc = getMemcachedFactory()->getMemcached(IMONLINE_CACHED);
	if (mc->mcGet(key, p))
	{
		status = p.m_status;
		log(Debug, "[CImOnlineCacheHelper::GetStatusByUid]: user: %u status is %d !", uid, status);
	}
	else
	{
		log(Error, "[CImOnlineCacheHelper::GetStatusByUid]: user: %u not found !", uid);
	}

	return status;
}

// ��memcached�л�ȡ�û�״̬������imlinkd
bool CImOnlineCacheHelper::GetOnlineInfoByUid(uint32_t uid, uint32_t& linkdId, protocol::E_IMSTATUS& status)
{
	string key = Uid2Key(uid);

	COnlineInfo p;
	Memcached *mc = getMemcachedFactory()->getMemcached(IMONLINE_CACHED);
	if (mc->mcGet(key, p))
	{
		linkdId = p.m_linkdId;
		status = p.m_status;
		log(Debug, "[CImOnlineCacheHelper::GetOnlineInfoByUid]: user: %u status: %d on imlinkd: %u !", uid, status, linkdId);
		return true;
	}
	else
	{
		log(Error, "[CImOnlineCacheHelper::GetOnlineInfoByUid]: user: %u not found !", uid);
	}

	return false;
}

// ��memcached��������ȡ�û�״̬������imlinkd
bool CImOnlineCacheHelper::MultiGetOnlineInfoByKeys(const std::vector<string>& vecUids, 
												   std::map<std::string, imonlinecache::COnlineInfo>& mapOnlineInfo)
{
	Memcached *mc = getMemcachedFactory()->getMemcached(IMONLINE_CACHED);
	return mc->mcGetMulti(vecUids, mapOnlineInfo);
}

bool CImOnlineCacheHelper::MultiGetOnlineInfoByKeys(const std::vector<uint32_t> &vecUid, std::vector<imonlinecache::COnlineInfo> &vecOnlineInfo)
{
	//��������key
	std::vector<std::string> vecKey;
	for (std::vector<uint32_t>::const_iterator iter = vecUid.begin(); iter != vecUid.end(); iter ++)
	{
		vecKey.push_back( Uid2Key(*iter) );
	}

	std::map<std::string, imonlinecache::COnlineInfo> mapOnlineInfo;
	Memcached *mc = getMemcachedFactory()->getMemcached(IMONLINE_CACHED);
	bool bGet = mc->mcGetMulti(vecKey, mapOnlineInfo);
	if (!bGet)
		return false;

	vecOnlineInfo.clear();

	for (std::vector<uint32_t>::const_iterator iter = vecUid.begin(); iter != vecUid.end(); iter ++)
	{
		std::map<std::string, imonlinecache::COnlineInfo>::iterator iterOnlineInfo = mapOnlineInfo.find( Uid2Key(*iter) );
		if ( iterOnlineInfo  != mapOnlineInfo.end() )
		{
			vecOnlineInfo.push_back(iterOnlineInfo->second );
		}
		else
		{
			COnlineInfo stOnlineInfo;
			stOnlineInfo.m_linkdId = -1;
			stOnlineInfo.m_uid = *iter;
			stOnlineInfo.m_status = protocol::IMOFFLINE;
			vecOnlineInfo.push_back( stOnlineInfo);
		}
	}
	return true;
}

//
std::string CImOnlineCacheHelper::Uid2Key(uint32_t uid)
{
	stringstream tmpSs;
	tmpSs << ONLINECACHE_PREFIX << uid;

	return tmpSs.str();
}

//
uint32_t CImOnlineCacheHelper::Key2Uid(std::string key)
{
	return atoi(key.c_str() + 3);
}

//��memcached�в����û�״̬��Ϣ
bool CImOnlineCacheHelper::SetOnlineInfoByUid(mc::Memcached *mc, uint32_t uid, imonlinecache::COnlineInfo& obj)
{
	string key = Uid2Key(uid);
	return mc->mcSet(key, obj);
}

// ��memcached��ɾ���û���Ϣ
bool CImOnlineCacheHelper::DeleteOnlineInfoByUid(mc::Memcached *mc, uint32_t uid)
{
	string key = Uid2Key(uid);
	return mc->mcDel(key);
}

bool CImOnlineCacheHelper::DeleteOnlineInfoByKey(mc::Memcached *mc, std::string key)
{
	return mc->mcDel(key);
}

