#include "ServerSelectorImp.h"
#include "DaemonClient.h"

#include <stdio.h>

using namespace server;
using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//本节为容器管理方法

bool CServerInfoContainer::AddServer(const sdaemon::FavorStrSV & fs)
{
	std::map<uint32_t, SServerInfo> &mapSuffixInfo = m_mapServerInfo[fs.sstr];
	std::pair<std::map<uint32_t, SServerInfo>::iterator, bool> pairRet = mapSuffixInfo.insert( std::make_pair( fs.serverId, SServerInfo(fs.sstr, fs.serverId, fs.groupId, fs.dip, fs.wip) ) );
	return pairRet.second;
}

void CServerInfoContainer::RemoveServer(const sdaemon::FavorStrSV & fs)
{
	std::map<uint32_t, SServerInfo> &mapSuffixInfo = m_mapServerInfo[fs.sstr];
	mapSuffixInfo.erase(fs.serverId);
}

void CServerInfoContainer::ClearServers()
{
	m_mapServerInfo.clear();
}

void CServerInfoContainer::GetServersInfo(const std::string &strSuffix, std::map<uint32_t, SServerInfo> &mapSuffixInfo)
{
	std::map<std::string, std::map<uint32_t, SServerInfo> >::iterator iterSuffix = m_mapServerInfo.find(strSuffix);
	if (iterSuffix != m_mapServerInfo.end())
	{
		mapSuffixInfo = iterSuffix->second;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//本节为获取指定的服务器IP集合
std::set<uint32_t> CServerInfoContainer::GetServerIps(const std::string &strSuffix)
{
	std::set<uint32_t> setServerIp;
	//key:serverid, value:server info
	std::map<uint32_t, SServerInfo> mapSuffixInfo;
	CServerInfoContainer::GetServersInfo(strSuffix, mapSuffixInfo);

	for (std::map<uint32_t, SServerInfo>::iterator iterInfo = mapSuffixInfo.begin(); iterInfo != mapSuffixInfo.end(); iterInfo ++)
	{
		uint32_t ip = iterInfo->second.dIp ? iterInfo->second.dIp : iterInfo->second.wIp;
		setServerIp.insert(ip);
	}

	return setServerIp;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//本节为获取指定的服务器ID集合
std::set<uint32_t> CServerInfoContainer::GetServerIds(const std::string &strSuffix)
{
	std::set<uint32_t> setServerId;

	std::map<uint32_t, SServerInfo> mapSuffixInfo;
	CServerInfoContainer::GetServersInfo(strSuffix, mapSuffixInfo);

	for (std::map<uint32_t, SServerInfo>::iterator iterInfo = mapSuffixInfo.begin(); iterInfo != mapSuffixInfo.end(); iterInfo ++)
	{
		uint32_t uServerId = iterInfo->first;

		setServerId.insert( uServerId );
	}

	return setServerId;
}

std::set<uint32_t> CServerInfoContainer::GetServerIdsByGroup(const std::string &strSuffix, uint32_t uGroupId)
{
	std::set<uint32_t> setServerId;

	std::map<uint32_t, SServerInfo> mapSuffixInfo;
	CServerInfoContainer::GetServersInfo(strSuffix, mapSuffixInfo);

	for (std::map<uint32_t, SServerInfo>::iterator iterInfo = mapSuffixInfo.begin(); iterInfo != mapSuffixInfo.end(); iterInfo ++)
	{
		uint32_t uServerId = iterInfo->first;
		SServerInfo &stSrvInfo = iterInfo->second;
		
		if ( stSrvInfo.uGroupId == uGroupId )
		{
			setServerId.insert( uServerId );
		}
	}

	return setServerId;
}

std::set<uint32_t> CServerInfoContainer::GetServerIdsByNet(const std::string &strSuffix, uint32_t uNetType)
{
	std::set<uint32_t> setServerId;

	std::map<uint32_t, SServerInfo> mapSuffixInfo;
	CServerInfoContainer::GetServersInfo(strSuffix, mapSuffixInfo);

	for (std::map<uint32_t, SServerInfo>::iterator iterInfo = mapSuffixInfo.begin(); iterInfo != mapSuffixInfo.end(); iterInfo ++)
	{
		uint32_t uServerId = iterInfo->first;
		SServerInfo &stSrvInfo = iterInfo->second;

		//由于SServerInfo::TestNetType并不能返回NET_TYPE_ANY，所以需要处理这个特殊情况

		if (uNetType == NET_TYPE_ANY)
		{
			setServerId.insert( uServerId );
		}
		else
		{
			uint32_t theNetType = SServerInfo::TestNetType(stSrvInfo);
			if (theNetType == uNetType)
			{
				setServerId.insert( uServerId );
			}
		}
	}

	return setServerId;
}

std::set<uint32_t> CServerInfoContainer::GetServerIdsByNetGroup(const std::string &strSuffix, uint32_t uNetType, uint32_t uGroupId)
{
	std::set<uint32_t> setServerId;

	std::map<uint32_t, SServerInfo> mapSuffixInfo;
	CServerInfoContainer::GetServersInfo(strSuffix, mapSuffixInfo);

	for (std::map<uint32_t, SServerInfo>::iterator iterInfo = mapSuffixInfo.begin(); iterInfo != mapSuffixInfo.end(); iterInfo ++)
	{
		uint32_t uServerId = iterInfo->first;
		SServerInfo &stSrvInfo = iterInfo->second;

		if (stSrvInfo.uGroupId != uGroupId)
			continue;

		//由于SServerInfo::TestNetType并不能返回NET_TYPE_ANY，所以需要处理这个特殊情况

		if (uNetType == NET_TYPE_ANY)
		{
			setServerId.insert( uServerId );
		}
		else
		{
			uint32_t theNetType = SServerInfo::TestNetType(stSrvInfo);
			if (theNetType == uNetType)
			{
				setServerId.insert( uServerId );
			}
		}
	}

	return setServerId;
}

std::set<uint32_t> CServerInfoContainer::GetServerIdsByIp(const std::string &strSuffix, uint32_t uIp)
{
	std::set<uint32_t> setServerId;

	std::map<uint32_t, SServerInfo> mapSuffixInfo;
	CServerInfoContainer::GetServersInfo(strSuffix, mapSuffixInfo);

	for (std::map<uint32_t, SServerInfo>::iterator iterInfo = mapSuffixInfo.begin(); iterInfo != mapSuffixInfo.end(); iterInfo ++)
	{
		uint32_t uServerId = iterInfo->first;
		SServerInfo &stSrvInfo = iterInfo->second;

		if ((stSrvInfo.dIp == uIp) || (stSrvInfo.wIp == uIp))
		{
			setServerId.insert( uServerId );
		}
	}

	return setServerId;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//本节提供IP-ID的转换

uint32_t CServerInfoContainer::ServerIpToId(const std::string &strSuffix, uint32_t uServerIp, std::set<uint32_t> &setServerIds)
{
	std::map<uint32_t, SServerInfo> mapSuffixInfo;
	CServerInfoContainer::GetServersInfo(strSuffix, mapSuffixInfo);

	for (std::map<uint32_t, SServerInfo>::iterator iterInfo = mapSuffixInfo.begin(); iterInfo != mapSuffixInfo.end(); iterInfo ++)
	{
		uint32_t uServerId = iterInfo->first;
		SServerInfo &stSrvInfo = iterInfo->second;

		if (stSrvInfo.dIp == uServerIp || stSrvInfo.wIp == uServerIp)
		{
			setServerIds.insert(uServerId);
		}
	}

	std::set<uint32_t>::iterator iterServerId = setServerIds.begin();
	if (iterServerId != setServerIds.end())
		return *iterServerId;

	return -1;
}

uint32_t CServerInfoContainer::ServerIdToIp(const std::string &strSuffix, uint32_t uServerId)
{
	std::map<uint32_t, SServerInfo> mapSuffixInfo;
	CServerInfoContainer::GetServersInfo(strSuffix, mapSuffixInfo);

	for (std::map<uint32_t, SServerInfo>::iterator iterInfo = mapSuffixInfo.begin(); iterInfo != mapSuffixInfo.end(); iterInfo ++)
	{
		uint32_t theServerId = iterInfo->first;
		SServerInfo &stSrvInfo = iterInfo->second;

		if (theServerId == uServerId)
		{
			uint32_t theSrvIp = SServerInfo::TestSrvIp(stSrvInfo);
			return theSrvIp;
		}
	}

	return -1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//本节为注册指定类型的服务器

void CServerSelectorImp::RegisterServerSuffix(const std::string &strSuffix, EHashServerValueType emValueType, EHashFuntcionType emFuncType)
{
	log(Info, "Register suffix:%s, valuetype:%d, functype:%d", strSuffix.c_str(), emValueType, emFuncType);
	m_SuffixContainer.AddSuffix(strSuffix, emValueType, emFuncType);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//本节为关于服务器调整的相关虚接口

void CServerSelectorImp::onRefreshSV()
{
	m_bRefreshSV = true;

	CServerInfoContainer::ClearServers();
	m_HashContainer.ClearServers();

	std::vector<sdaemon::FavorStrSV> vecServers = client->getServersSV();

	for (std::vector<sdaemon::FavorStrSV>::iterator iterServer = vecServers.begin(); 
		iterServer != vecServers.end(); iterServer ++)
	{
		CServerSelectorImp::onAddSV( *iterServer );
	}
 
	uint32_t uTimeIntv = 0;
	core::DaemonClient *pDaemonClient = (core::DaemonClient*)client;

	//检查和daemon的Ping是否超时
	if (pDaemonClient->__IsDaemonConnTimeout(uTimeIntv))
	{
		//初始化调整，Watcher接口，但要注意死锁
		forEachWatcher0(&IServerSelectorWatcher::OnServerInit);

		log(Info, "[CServerSelectorImp::onRefreshSV] call IServerSelectorWatcher::OnServerInit");
	}
	else
	{
		log(Info, "[CServerSelectorImp::onRefreshSV] PingTimeout %u seconds", uTimeIntv);
	}

	//启动Ping更新
	pDaemonClient->__SetProcessReadyFlag(true);

	m_bRefreshSV = false;
}

void CServerSelectorImp::onAddSV(const sdaemon::FavorStrSV & fs)
{
	EHashServerValueType emValueType = ENUM_HASH_SERVERID;
	EHashFuntcionType emFuncType = ENUM_FUNC_MD5;

	if ( fs.serverStatus == sdaemon::RUNNING && m_SuffixContainer.FindSuffix(fs.sstr, emValueType, emFuncType) )
	{
		//将关心的服务器加进容器中
		bool bAdd = CServerInfoContainer::AddServer( fs );

		if (bAdd)
		{

			//增加一个小哈希环节点, 1. 需要判断哈希类型 2. 需要使用注册的算法
			if (emValueType == ENUM_HASH_SERVERIP)
			{
				uint32_t theSrvIp = SServerInfo::TestSrvIp( SServerInfo(fs.sstr, fs.serverId, fs.groupId, fs.dip, fs.wip) );
				m_HashContainer.AddServer( fs.serverId, theSrvIp, emFuncType );
			}
			else if (emValueType == ENUM_HASH_SERVERIPID)
			{
				uint32_t theSrvIp = SServerInfo::TestSrvIp( SServerInfo(fs.sstr, fs.serverId, fs.groupId, fs.dip, fs.wip) );
				//将同类ip组成一个hash环
				m_HashIpContainer.AddServer(theSrvIp, theSrvIp, emFuncType);
				//将同ip的各sid组成一个hash环
				m_HashContainer.AddServer(fs.serverId, fs.serverId, emFuncType);
				//记录同ip的各sid
				m_mapIpServerIds[theSrvIp].insert(fs.serverId);
			}
			else
			{
				m_HashContainer.AddServer( fs.serverId, fs.serverId, emFuncType );
			}

			//令哈希缓存失效
			m_HashCacheSelector.FreshHashCache( fs.sstr );

			if (!m_bRefreshSV)
			{
				//服务器调整时，Watcher接口，但要注意死锁
				forEachWatcherRef1(&IServerSelectorWatcher::OnServerAddjust, fs.sstr);
				//服务器调整时，Watcher接口，但要注意死锁
				forEachWatcherRef1(&IServerSelectorWatcher::OnServerAddjust2, fs);
				log(Info, "[CServerSelectorImp::onAddSV] call IServerSelectorWatcher::OnServerAddjust");
			}

		}
		else
		{
			log(Info, "[CServerSelectorImp::onAddSV] server(id:%u) has in hash set!", fs.serverId);
		}
	}
/*
	//打印服务器启停记录
	if (sdaemon::RUNNING == fs.serverStatus)
	{
		log(Info, "[CServerSelectorImp::onAddSV] ServerId[%d] ServerName[%s] ServerIp[%s]",
			fs.serverId, fs.sstr.c_str(), CDebugHelper::SVToIP(fs).c_str());
	}*/
}

void CServerSelectorImp::onServerRemovedSV(const sdaemon::FavorStrSV & fs)
{
	EHashServerValueType emValueType = ENUM_HASH_SERVERID;
	EHashFuntcionType emFuncType = ENUM_FUNC_MD5;

	if ( m_SuffixContainer.FindSuffix(fs.sstr, emValueType, emFuncType) )
	{
		//将关心的服务器从容器中移除
		CServerInfoContainer::RemoveServer( fs );

		//移除一个小哈希环节点
		m_HashContainer.RemoveServer( fs.serverId );

		uint32_t theSrvIp = SServerInfo::TestSrvIp( SServerInfo(fs.sstr, fs.serverId, fs.groupId, fs.dip, fs.wip));
		m_mapIpServerIds[theSrvIp].erase(fs.serverId);

		if (m_mapIpServerIds[theSrvIp].empty())
		{
			m_HashIpContainer.RemoveServer(theSrvIp);
			m_mapIpServerIds.erase(theSrvIp);
		}

		//令哈希缓存失效
		m_HashCacheSelector.FreshHashCache( fs.sstr );

		//服务器调整时，Watcher接口，但要注意死锁
		//forEachWatcherRef1(&IServerSelectorWatcher::OnServerAddjust, fs.sstr);
	}

	//打印服务器启停记录
	//log(Info, "[CServerSelectorImp::onServerRemovedSV] ServerId[%d] ServerName[%s] ServerIp[%s]",
	//	fs.serverId, fs.sstr.c_str(), CDebugHelper::SVToIP(fs).c_str());
}

void CServerSelectorImp::onRecoverRefreshSV()
{
	CServerSelectorImp::onRefreshSV();
}

void CServerSelectorImp::onServerStatusChangeSV(const sdaemon::FavorStrSV & fs)
{
	CServerSelectorImp::onAddSV( fs );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//本节为随机选择一个需要的服务器

uint32_t CServerSelectorImp::RandomServer(const std::string &strSuffix)
{
	std::set<uint32_t> setServerId = CServerInfoContainer::GetServerIds(strSuffix);

	if (setServerId.empty())
		return -1;

	if (setServerId.size() == 1)
		return *setServerId.begin();

	std::vector<uint32_t> vecServerId;
	vecServerId.assign(setServerId.begin(), setServerId.end());
	random_shuffle(vecServerId.begin(),vecServerId.end());
	return *vecServerId.begin();
}

uint32_t CServerSelectorImp::RandomServerByNet(const std::string &strSuffix, uint32_t uNetType)
{
	std::set<uint32_t> setServerId = CServerInfoContainer::GetServerIdsByNet(strSuffix, uNetType);

	if (setServerId.empty())
		return -1;

	if (setServerId.size() == 1)
		return *setServerId.begin();

	std::vector<uint32_t> vecServerId;
	vecServerId.assign(setServerId.begin(), setServerId.end());
	random_shuffle(vecServerId.begin(),vecServerId.end());
	return *vecServerId.begin();
}

uint32_t CServerSelectorImp::RandomServerByGroup(const std::string &strSuffix, uint32_t uGroupId)
{
	std::set<uint32_t> setServerId = CServerInfoContainer::GetServerIdsByGroup(strSuffix, uGroupId);

	if (setServerId.empty())
		return -1;

	if (setServerId.size() == 1)
		return *setServerId.begin();

	std::vector<uint32_t> vecServerId;
	vecServerId.assign(setServerId.begin(), setServerId.end());
	random_shuffle(vecServerId.begin(),vecServerId.end());
	return *vecServerId.begin();
}

uint32_t CServerSelectorImp::RandomServerByNetGroup(const std::string &strSuffix, uint32_t uNetType, uint32_t uGroupId)
{
	std::set<uint32_t> setServerId = CServerInfoContainer::GetServerIdsByNetGroup(strSuffix, uNetType, uGroupId);

	if (setServerId.empty())
		return -1;

	if (setServerId.size() == 1)
		return *setServerId.begin();

	std::vector<uint32_t> vecServerId;
	vecServerId.assign(setServerId.begin(), setServerId.end());
	random_shuffle(vecServerId.begin(),vecServerId.end());
	return *vecServerId.begin();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//本节为哈希选择一个需要的服务器

uint32_t CServerSelectorImp::HashServer(uint32_t uKey, const std::string &strSuffix)
{
	//必须先查询注册类型，确认算法
	EHashServerValueType emValueType = ENUM_HASH_SERVERID;
	EHashFuntcionType emFuncType = ENUM_FUNC_MD5;
	bool bFind = m_SuffixContainer.FindSuffix(strSuffix, emValueType, emFuncType);
	if (!bFind)
	{
		log(Info, "[CServerSelectorImp::HashServer] m_SuffixContainer.FindSuffix(%s) return false", strSuffix.c_str());
		return -1;
	}

	//先直接从缓存取
	//缓存不存在，再从CHashContainer取
	bool bGet = false;
	uint32_t uServerValue = m_HashCacheSelector.HashValueFromCache(strSuffix, -1, -1, uKey, emFuncType, bGet);
	if (!bGet)
	{
		std::set<uint32_t> setServerId = CServerInfoContainer::GetServerIds(strSuffix);

		if (setServerId.empty())
		{
			log(Info, "[CServerSelectorImp::HashServer] CServerInfoContainer::GetServerIds(%s) return empty", strSuffix.c_str());
			return -1;
		}

		if (setServerId.size() == 1)
			return *setServerId.begin();

		uServerValue = m_HashCacheSelector.HashValueFromContainer(strSuffix, -1, -1, setServerId, uKey, emFuncType);

		//log(Info, "[CServerSelectorImp::HashServer] %d = m_HashCacheSelector.HashValueFromContainer(%s, -1, -1, <%s>, %d)",
		//	uServerValue, strSuffix.c_str(), CDebugHelper::SetIntToString(setServerId).c_str(), uKey);
	}

	if (emValueType == ENUM_HASH_SERVERIP)
	{
		std::set<uint32_t> setServerIdTmp;
		uint32_t uServerIp = uServerValue;
		uServerValue = CServerInfoContainer::ServerIpToId(strSuffix, uServerValue, setServerIdTmp);

		log(Info, "[CServerSelectorImp::HashServer] %d(%s) = CServerInfoContainer::ServerIpToId(%s, %d)",
			uServerValue, CDebugHelper::IPToString(uServerIp).c_str(), strSuffix.c_str(), uServerIp);
	}

	return uServerValue;
}

bool CServerSelectorImp::HashMultiServer(uint32_t uKey, uint32_t uValueCount, std::vector<uint32_t>& vecValues, const std::string &strSuffix)
{
	//必须先查询注册类型，确认算法
	EHashServerValueType emValueType = ENUM_HASH_SERVERID;
	EHashFuntcionType emFuncType = ENUM_FUNC_MD5;
	bool bFind = m_SuffixContainer.FindSuffix(strSuffix, emValueType, emFuncType);
	if (!bFind)
	{
		log(Info, "[CServerSelectorImp::HashMultiServer] m_SuffixContainer.FindSuffix(%s) return false", strSuffix.c_str());
		return -1;
	}

	std::set<uint32_t> setValues;

	//先直接从缓存取
	//缓存不存在，再从CHashContainer取
	bool bGet = false;
	bool bRet = m_HashCacheSelector.HashMultiValueFromCache(strSuffix, -1, -1, uKey, uValueCount, setValues, emFuncType, bGet);
	if (!bRet)
	{
		return false;
	}

	if (!bGet)
	{
		std::set<uint32_t> setServerId;
		if (emValueType == ENUM_HASH_SERVERIPID)
		{
			//获取所有的server ip
			setServerId = CServerInfoContainer::GetServerIps(strSuffix);

			if (setServerId.empty())
			{
				log(Info, "[CServerSelectorImp::HashMultiServer] CServerInfoContainer::GetServerIds(%s) return empty", strSuffix.c_str());
				return false;
			}
			
			if (setServerId.size() <= uValueCount)
			{
				setValues = setServerId;
			}
			else
			{
				//hash选择指定数量的server ip
				bRet = m_HashCacheSelector.HashMultiValueFromContainer(strSuffix, -1, -1, setServerId, uKey, uValueCount, setValues, emValueType, emFuncType);
				if (!bRet)
				{
					return false;
				}
			}
		}
		else
		{
			setServerId = CServerInfoContainer::GetServerIds(strSuffix);
			
			if (setServerId.empty())
			{
				log(Info, "[CServerSelectorImp::HashMultiServer] CServerInfoContainer::GetServerIds(%s) return empty", strSuffix.c_str());
				return false;
			}
			
			if (setServerId.size() == 1)
			{
				return *setServerId.begin();
			}
			else
			{
 				bRet = m_HashCacheSelector.HashMultiValueFromContainer(strSuffix, -1, -1, setServerId, uKey, uValueCount, setValues, emValueType, emFuncType);
				if (!bRet)
				{
					return false;
				}
			}
		}

		//log(Info, "[CServerSelectorImp::HashServer] %d = m_HashCacheSelector.HashValueFromContainer(%s, -1, -1, <%s>, %d)",
		//	uServerValue, strSuffix.c_str(), CDebugHelper::SetIntToString(setServerId).c_str(), uKey);
	}

	if (emValueType == ENUM_HASH_SERVERIP)
	{
		for (std::set<uint32_t>::iterator it = setValues.begin();
			it != setValues.end();
			it++)
		{
			std::set<uint32_t> setServerIdTmp;
			uint32_t uServerIp = *it;
			uint32_t uServerValue = CServerInfoContainer::ServerIpToId(strSuffix, uServerIp, setServerIdTmp);
			
			log(Info, "[CServerSelectorImp::HashMultiServer] %d(%s) = CServerInfoContainer::ServerIpToId(%s, %d)",
				uServerValue, CDebugHelper::IPToString(uServerIp).c_str(), strSuffix.c_str(), uServerIp);

			vecValues.push_back(uServerValue);
		}
	}
	else if (ENUM_HASH_SERVERIPID)
	{
		uint32_t uServerId;
		//遍历ip列表，为每个ip选择一个serverid
		for (std::set<uint32_t>::iterator it = setValues.begin();
			it != setValues.end();
			it++)
		{
			std::map<uint32_t, std::set<uint32_t> >::iterator itmap = m_mapIpServerIds.find(*it);
			if (itmap != m_mapIpServerIds.end())
			{	
				//构造hash缓存的key，suffix已经用来标示ip hash了，每个ip下的serveridhash key就用suffix+ip
				std::stringstream ss;
				ss << strSuffix << ":" << *it;

				uServerId = m_HashCacheSelector.HashValueFromContainer(ss.str(), -1, -1, itmap->second, uKey, emFuncType);
				if (uServerId != (uint32_t)-1)
				{
					//log(Info, "[CServerSelectorImp::HashMultiServer] server ip:%d server id:%d is selected", *it, uServerId);
					vecValues.push_back(uServerId);
				}
			}
		}

		if (vecValues.empty())
		{
			return false;
		}
	}
	else
	{
		vecValues.assign(setValues.begin(), setValues.end());
	}

	return true;
}

uint32_t CServerSelectorImp::HashServerByNet(uint32_t uKey, const std::string &strSuffix, uint32_t uNetType)
{
	EHashServerValueType emValueType = ENUM_HASH_SERVERID;
	EHashFuntcionType emFuncType = ENUM_FUNC_MD5;
	bool bFind = m_SuffixContainer.FindSuffix(strSuffix, emValueType, emFuncType);
	if (!bFind)
	{
		log(Info, "[CServerSelectorImp::HashServerByNet] m_SuffixContainer.FindSuffix(%s) return false", strSuffix.c_str());
		return -1;
	}

	bool bGet = false;
	uint32_t uServerValue = m_HashCacheSelector.HashValueFromCache(strSuffix, uNetType, -1, uKey, emFuncType, bGet);
	if (!bGet)
	{
		std::set<uint32_t> setServerId = CServerInfoContainer::GetServerIdsByNet(strSuffix, uNetType);

		if (setServerId.empty())
		{
			log(Info, "[CServerSelectorImp::HashServerByNet] CServerInfoContainer::GetServerIdsByNet(%s, %d) return empty", strSuffix.c_str(), uNetType);
			return -1;
		}

		if (setServerId.size() == 1)
			return *setServerId.begin();

		uServerValue = m_HashCacheSelector.HashValueFromContainer(strSuffix, uNetType, -1, setServerId, uKey, emFuncType);
	}

	if (emValueType == ENUM_HASH_SERVERIP)
	{
		std::set<uint32_t> setServerIdTmp;
		//uint32_t uServerIp = uServerValue;
		uServerValue = CServerInfoContainer::ServerIpToId(strSuffix, uServerValue, setServerIdTmp);
	}

	return uServerValue;
}

uint32_t CServerSelectorImp::HashServerByGroup(uint32_t uKey, const std::string &strSuffix, uint32_t uGroupId)
{
	EHashServerValueType emValueType = ENUM_HASH_SERVERID;
	EHashFuntcionType emFuncType = ENUM_FUNC_MD5;
	bool bFind = m_SuffixContainer.FindSuffix(strSuffix, emValueType, emFuncType);
	if (!bFind)
	{
		log(Info, "[CServerSelectorImp::HashServerByGroup] m_SuffixContainer.FindSuffix(%s) return false", strSuffix.c_str());
		return -1;
	}

	bool bGet = false;
	uint32_t uServerValue = m_HashCacheSelector.HashValueFromCache(strSuffix, -1, uGroupId, uKey, emFuncType, bGet);
	if (!bGet)
	{
		std::set<uint32_t> setServerId = CServerInfoContainer::GetServerIdsByGroup(strSuffix, uGroupId);

		if (setServerId.empty())
		{
		  //log(Info, "[CServerSelectorImp::HashServerByGroup] CServerInfoContainer::GetServerIdsByGroup(%s, %d) return empty", strSuffix.c_str(), uGroupId);
			return -1;
		}

		if (setServerId.size() == 1)
			return *setServerId.begin();

		uServerValue = m_HashCacheSelector.HashValueFromContainer(strSuffix, -1, uGroupId, setServerId, uKey, emFuncType);
	}

	if (emValueType == ENUM_HASH_SERVERIP)
	{
		std::set<uint32_t> setServerIdTmp;
		//uint32_t uServerIp = uServerValue;
		uServerValue = CServerInfoContainer::ServerIpToId(strSuffix, uServerValue, setServerIdTmp);
	}

	return uServerValue;
}

uint32_t CServerSelectorImp::HashServerByNetGroup(uint32_t uKey, const std::string &strSuffix, uint32_t uNetType, uint32_t uGroupId)
{
	EHashServerValueType emValueType = ENUM_HASH_SERVERID;
	EHashFuntcionType emFuncType = ENUM_FUNC_MD5;
	bool bFind = m_SuffixContainer.FindSuffix(strSuffix, emValueType, emFuncType);
	if (!bFind)
	{
		log(Info, "[CServerSelectorImp::HashServerByNetGroup] m_SuffixContainer.FindSuffix(%s) return false", strSuffix.c_str());
		return -1;
	}

	bool bGet = false;
	uint32_t uServerValue = m_HashCacheSelector.HashValueFromCache(strSuffix, uNetType, uGroupId, uKey, emFuncType, bGet);
	if (!bGet)
	{
		std::set<uint32_t> setServerId = CServerInfoContainer::GetServerIdsByNetGroup(strSuffix, uNetType, uGroupId);

		if (setServerId.empty())
		{
			log(Info, "[CServerSelectorImp::HashServerByNetGroup] CServerInfoContainer::GetServerIdsByNetGroup(%s, %d, %d) return empty", 
				strSuffix.c_str(), uNetType, uGroupId);
			return -1;
		}

		if (setServerId.size() == 1)
			return *setServerId.begin();

		uServerValue = m_HashCacheSelector.HashValueFromContainer(strSuffix, uNetType, uGroupId, setServerId, uKey, emFuncType);
	}

	if (emValueType == ENUM_HASH_SERVERIP)
	{
		std::set<uint32_t> setServerIdTmp;
		//uint32_t uServerIp = uServerValue;
		uServerValue = CServerInfoContainer::ServerIpToId(strSuffix, uServerValue, setServerIdTmp);
	}

	return uServerValue;
}

uint32_t CServerSelectorImp::HashServerByIp(uint32_t uKey, const std::string &strSuffix, uint32_t uIp)
{
	EHashServerValueType emValueType = ENUM_HASH_SERVERID;
	EHashFuntcionType emFuncType = ENUM_FUNC_MD5;
	bool bFind = m_SuffixContainer.FindSuffix(strSuffix, emValueType, emFuncType);
	if (!bFind)
	{
		log(Info, "[CServerSelectorImp::HashServerByIp] m_SuffixContainer.FindSuffix(%s) return false", strSuffix.c_str());
		return -1;
	}

	bool bGet = false;
	uint32_t uServerValue = m_HashCacheSelector.HashValueFromCache(strSuffix, uIp, -1, uKey, emFuncType, bGet);
	if (!bGet)
	{
		std::set<uint32_t> setServerId = CServerInfoContainer::GetServerIdsByIp(strSuffix, uIp);

		if (setServerId.empty())
		{
			log(Info, "[CServerSelectorImp::HashServerByIp] CServerInfoContainer::GetServerIdsByIp(%s, %d) return empty", strSuffix.c_str(), uIp);
			return -1;
		}

		if (setServerId.size() == 1)
			return *setServerId.begin();

		uServerValue = m_HashCacheSelector.HashValueFromContainer(strSuffix, uIp, -1, setServerId, uKey, emFuncType);
	}

	return uServerValue;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
