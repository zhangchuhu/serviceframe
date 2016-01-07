#include "ImMemDBHelper.h"

using namespace server::im;
using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//设置用户在线状态、所在ImLinkdId

bool CImMemDBHelper::SetUserOnlineInfo(uint32_t Uid, const protocol::E_IMSTATUS &emStatus, uint32_t uImLinkdId)
{
	CUserOnlineInfo stUserOnline(Uid, uImLinkdId, emStatus);
	return __SetUserOnlineToCache(Uid, stUserOnline);
}

bool CImMemDBHelper::SetUserOnlineInfo(uint32_t Uid, CUserOnlineInfo &stUserOnline)
{
	return __SetUserOnlineToCache(Uid, stUserOnline);
}

//获取用户在线状态、所在ImLinkdId
//离线返回false,其他返回true

bool CImMemDBHelper::GetUserOnlineInfo(uint32_t Uid, protocol::E_IMSTATUS &emStatus, uint32_t &uImLinkdId)
{
	CUserOnlineInfo stUserOnline;
	bool bGetOnline = __GetUserOnlineFromCache(Uid, stUserOnline);
	if (bGetOnline)
	{
		emStatus = stUserOnline.m_emStatus;
		uImLinkdId = stUserOnline.m_uImLinkdId;
	}
	else
	{
		emStatus = protocol::IMOFFLINE;
		uImLinkdId = -1;
	}
	return stUserOnline.IsNotOffline(); //既然获取失败，仍然可以放心这样使用，因为stUserOnline构造时被初使化离线
}

//离线返回false,其他返回true

bool CImMemDBHelper::GetUserOnlineInfo(uint32_t Uid, CUserOnlineInfo &stUserOnline)
{
	bool bGetOnline = __GetUserOnlineFromCache(Uid, stUserOnline);
	if (!bGetOnline)
	{
		stUserOnline.m_emStatus = protocol::IMOFFLINE;
		stUserOnline.m_uImLinkdId = -1;
	}
	return stUserOnline.IsNotOffline();
}

//离线返回false,其他返回true

bool CImMemDBHelper::GetUserOnlineInfo(uint32_t Uid, uint32_t& uImLinkdId)
{
	CUserOnlineInfo stUserOnline;
	bool bGetOnline = __GetUserOnlineFromCache(Uid, stUserOnline);
	if (bGetOnline)
	{
		uImLinkdId = stUserOnline.m_uImLinkdId;
	}
	else
	{
		uImLinkdId = -1;
	}
	return stUserOnline.IsNotOffline();
}

//擦除用户在线状态

bool CImMemDBHelper::EraseUserOnlineInfo(uint32_t Uid)
{
	return __EraseUserOnlineInfo(Uid);
}

//重置Online memcached

bool CImMemDBHelper::ClearAllOnlineInfo()
{
	return __ClearAllOnlineInfo();
}

//批量设置用户在线状态、所在ImLinkdId

bool CImMemDBHelper::BatchSetUserOnlineInfo(vector<CUserOnlineInfo> &vecUserOnline)
{
	server::mc::Memcached* mc = getMemcachedFactory()->getMemcached(IMONLINE_MEMDB);
	if (mc == NULL)
		return false;

	for (vector<CUserOnlineInfo>::iterator iterUserOnline = vecUserOnline.begin();
		iterUserOnline != vecUserOnline.end();
		iterUserOnline ++)
	{
		//__SetUserOnlineToCache(iterUserOnline->m_Uid, *iterUserOnline); //不使用此用是为了防止循环多次调用getMemcached
		mc->mcSet(__Uid2Key(iterUserOnline->m_Uid), *iterUserOnline, EXPIRATION_TIME);
	}
	return true;
}

//批量获取用户在线状态、所在ImLinkdId

bool CImMemDBHelper::BatchGetUserOnlineInfo(const vector<uint32_t> &vecUid, std::vector<CUserOnlineInfo> &vecUserOnline)
{
	//在返回集中，必须给请求的所有UID返回结果（即如果查不到记录，设为离线）

	return __BatchGetUserOnlineInfo(vecUid, vecUserOnline);

	//vecUserOnline.clear();

	//for (vector<uint32_t>::const_iterator iterUid = vecUid.begin();
	//	 iterUid != vecUid.end();
	//	 iterUid ++)
	//{
	//	CUserOnlineInfo stUserOnline;
	//	bool bGetOnline = __GetUserOnlineFromCache(*iterUid, stUserOnline);
	//	if (!bGetOnline)
	//	{
	//		stUserOnline.m_Uid = *iterUid;
	//		stUserOnline.m_uImLinkdId = -1;
	//		stUserOnline.m_emStatus = protocol::IMOFFLINE;
	//	}
	//	vecUserOnline.push_back(stUserOnline);
	//}

	//return true;
}

//批量擦除用户在线状态、所在ImLinkdId

bool CImMemDBHelper::BatchEraseUserOnlineInfo(const vector<uint32_t> &vecUid)
{
	server::mc::Memcached* mc = getMemcachedFactory()->getMemcached(IMONLINE_MEMDB);
	if (mc == NULL)
		return false;

	for (vector<uint32_t>::const_iterator iterUid = vecUid.begin();
		iterUid != vecUid.end();
		iterUid ++)
	{
		//__EraseUserOnlineInfo(*iterUid);
		mc->mcDel(__Uid2Key(*iterUid));
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//设置/获取缓存的用户在线信息（非线程安全）

bool CImMemDBHelper::__SetUserOnlineToCache(uint32_t Uid, CUserOnlineInfo &stUserOnline)
{
	server::mc::Memcached* mc = getMemcachedFactory()->getMemcached(IMONLINE_MEMDB);
	if (mc == NULL)
		return false;

	return mc->mcSet(__Uid2Key(Uid), stUserOnline, EXPIRATION_TIME);
}

bool CImMemDBHelper::__GetUserOnlineFromCache(uint32_t Uid, CUserOnlineInfo &stUserOnline)
{
	server::mc::Memcached* mc = getMemcachedFactory()->getMemcached(IMONLINE_MEMDB);
	if (mc == NULL)
		return false;

	return mc->mcGet(__Uid2Key(Uid), stUserOnline);
}

bool CImMemDBHelper::__EraseUserOnlineInfo(uint32_t Uid)
{
	server::mc::Memcached* mc = getMemcachedFactory()->getMemcached(IMONLINE_MEMDB);
	if (mc == NULL)
		return false;

	return mc->mcDel(__Uid2Key(Uid));
}

bool CImMemDBHelper::__ClearAllOnlineInfo()
{
	server::mc::Memcached* mc = getMemcachedFactory()->getMemcached(IMONLINE_MEMDB);
	if (mc == NULL)
		return false;

	return mc->mcFlush();
}

bool CImMemDBHelper::__BatchGetUserOnlineInfo(const std::vector<uint32_t> &vecUid, std::vector<CUserOnlineInfo> &vecUserOnline)
{
	server::mc::Memcached* mc = getMemcachedFactory()->getMemcached(IMONLINE_MEMDB);
	if (mc == NULL)
		return false;

	//批量构造KEY
	std::vector<std::string> vecKey;
	for (std::vector<uint32_t>::const_iterator iterUid = vecUid.begin(); 
		 iterUid != vecUid.end();
		 iterUid ++)
	{
		vecKey.push_back(__Uid2Key(*iterUid));
	}

	std::map<std::string, CUserOnlineInfo> mapUserOnline;
	bool bGet = mc->mcGetMulti(vecKey, mapUserOnline);
	if (!bGet)
		return false;

	vecUserOnline.clear();

	for (std::vector<uint32_t>::const_iterator iterUid = vecUid.begin();
		iterUid != vecUid.end();
		iterUid ++)
	{
		//先查返回的mapUserOnline里是否有数据，以保持返回数据与提供参数的一致性
		std::map<std::string, CUserOnlineInfo>::const_iterator iterUser = mapUserOnline.find(__Uid2Key(*iterUid));
		if (iterUser != mapUserOnline.end())
		{
			vecUserOnline.push_back( iterUser->second );
		}
		else
		{
			CUserOnlineInfo stUserOnline(*iterUid, -1, protocol::IMOFFLINE);
			vecUserOnline.push_back(stUserOnline);
		}
	}


	//vecUserOnline.clear();
	////循环填充已获取的用户
	//for (std::map<std::string, CUserOnlineInfo>::iterator iterUser = mapUserOnline.begin();
	//	 iterUser != mapUserOnline.end();
	//	 iterUser ++)
	//{
	//	vecUserOnline.push_back( iterUser->second );
	//}
	////填允不在线的用户
	//for (std::vector<uint32_t>::const_iterator iterUid = vecUid.begin();
	//	iterUid != vecUid.end();
	//	iterUid ++)
	//{
	//	if (mapUserOnline.find(__Uid2Key(*iterUid)) == mapUserOnline.end())
	//	{
	//		CUserOnlineInfo stUserOnline(*iterUid, -1, protocol::IMOFFLINE);
	//		vecUserOnline.push_back(stUserOnline);
	//	}
	//}

	return true;
}

bool CImMemDBHelper::__BatchSetUserOnlineInfo(std::vector<CUserOnlineInfo> &vecUserOnline)
{
	//server::mc::Memcached* mc = getMemcachedFactory()->getMemcached(IMONLINE_MEMDB);
	//if (mc == NULL)
	//	return false;

	////批量构造KEY
	//vector<std::string> vecKey;
	//for (std::vector<CUserOnlineInfo>::const_iterator iterUser = vecUserOnline.begin(); 
	//	iterUser != vecUserOnline.end();
	//	iterUser ++)
	//{
	//	vecKey.push_back(__Uid2Key(iterUser->m_Uid));
	//}

	//mc->mcSetMulti(vecKey, );

	return true;
}

bool CImMemDBHelper::__BatchEraseUserOnlineInfo(const vector<uint32_t> &vecUid)
{
	return true;
}

inline string CImMemDBHelper::__Uid2Key(uint32_t Uid)
{
	stringstream ss;
	ss << IMONLINE_PREFIX << Uid;
	return ss.str();
}
