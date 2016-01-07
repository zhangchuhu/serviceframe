#include <algorithm>
#include "ImServersInfo.h"
#include "core/sox/logger.h"
#include "server_common/server-lib/id_def.h"
#include "core/sox/sockethelper.h"


using namespace server;
using namespace std;
 
// 插入服务器记录
void CImServersContainer::AddServer(const CIMServer& srv)
{
	m_mapServers.insert(make_pair(srv.m_serverId, srv));	
}


// 删除服务器记录
void CImServersContainer::RemoveServer(const uint32_t srvId)
{
	m_mapServers.erase(srvId);
}


// 根据服务器类型名和网络类型，获取服务器ID
void CImServersContainer::GetServersByNameType(const std::string& name, const uint32_t type, std::vector<uint32_t>& vecIDs)
{
	for (ServersMap_T::iterator iter = m_mapServers.begin(); iter != m_mapServers.end(); iter++)
	{
		if ((*iter).second.m_netType == type && (*iter).second.m_sstr == name)
		{
			vecIDs.push_back((*iter).first);
		}
	}

	log (Info, "[CImServersContainer::GetServersByNameType]: finish. name-type is %s-%u", name.data(), type);
}

//
void CImServersContainer::GetServersByNameTypeEx(const std::string& name, const uint32_t type, std::vector<uint32_t>& vecIDs)
{
	uint32_t multiLineType = CNC | CTL;

	for (ServersMap_T::iterator iter = m_mapServers.begin(); iter != m_mapServers.end(); iter++)
	{
		if (((*iter).second.m_netType == type || (*iter).second.m_netType == multiLineType)
			&& (*iter).second.m_sstr == name)
		{
			vecIDs.push_back((*iter).first);
		}
	}

	log (Info, "[CImServersContainer::GetServersByNameTypeEx]: finish. name-type is %s-%u", name.data(), type);
}

// 根据服务器类型获取服务器ID
void CImServersContainer::GetServersByName(const std::string& name, std::vector<uint32_t>& vecIDs)
{
	for (ServersMap_T::iterator iter = m_mapServers.begin(); iter != m_mapServers.end(); iter++)
	{
		if ((*iter).second.m_sstr == name)
		{
			vecIDs.push_back((*iter).first);
		}
	}

	//log (Info, "[CImServersContainer::GetServersByName]: finish. name is %s", name.data());
}

// 清空容器
void CImServersContainer::ClearServer()
{
	m_mapServers.clear();
}

//
CIMServer CImServersContainer::GetServerByID(const uint32_t serverID)
{
	ServersMap_T::iterator iter = m_mapServers.find(serverID);

	if (iter == m_mapServers.end())
	{
		return CIMServer();
	}
	else
	{
		return (*iter).second;
	}
}

//
//
//    CImServersInfo 实现
//
//

// 通过服务器类型名和网络类型，获取相同网络下同一类型的所有服务器ID
vector<uint32_t> CImServersInfo::GetServersIDByNameType(const std::string& name, const uint32_t type)
{
	vector<uint32_t> vecIds;
	m_srvContainer.GetServersByNameType(name, type, vecIds);
	
	return vecIds;
}

vector<uint32_t> CImServersInfo::GetServersIDByNameTypeEx(const std::string& name, const uint32_t type)
{
	vector<uint32_t> vecIds;
	m_srvContainer.GetServersByNameTypeEx(name, type, vecIds);

	return vecIds;
}

// 通过服务器类型名，获取一类型的所有服务器ID
vector<uint32_t> CImServersInfo::GetServersIDByName(const std::string& name)
{
	vector<uint32_t> vecIds;
	m_srvContainer.GetServersByName(name, vecIds);

	return vecIds;
}

// 通过服务器名和网络类型，随即获取一个服务器ID
uint32_t CImServersInfo::RandomServerIDByNameType(const std::string& name, const uint32_t type)
{
	vector<uint32_t> vecIds;
	m_srvContainer.GetServersByNameType(name, type, vecIds);	
	if (!vecIds.empty())
	{
		random_shuffle(vecIds.begin(), vecIds.end());
		return vecIds.front();
	}
	else
	{
		log (Error, "[CImServersInfo::RandomServerIDByNameType] no server found. name(%s)-ISP(%d)", name.data(), type);
	}
	return -1;
}

uint32_t CImServersInfo::RandomServerIDByNameTypeEx(const std::string& name, const uint32_t type)
{
	vector<uint32_t> vecIds;
	m_srvContainer.GetServersByNameTypeEx(name, type, vecIds);	
	if (!vecIds.empty())
	{
		random_shuffle(vecIds.begin(), vecIds.end());
		return vecIds.front();
	}
	else
	{
		log (Error, "[CImServersInfo::RandomServerIDByNameTypeEx] no server found. name(%s)-ISP(%d)", name.data(), type);
	}
	return -1;
}

// 通过服务器名，随即获取一个服务器ID
uint32_t CImServersInfo::RandomServerIDByName(const std::string& name)
{
	vector<uint32_t> vecIds;
	m_srvContainer.GetServersByName(name, vecIds);	
	if (!vecIds.empty())
	{
		random_shuffle(vecIds.begin(), vecIds.end());
		return vecIds.front();
	}
	else
	{
		log (Error, "[CImServersInfo::RandomServerIDByName] no server found. name(%s)", name.data());
	}
	return -1;
}

// 获取网络类型
uint32_t CImServersInfo::GetNetTypeByID(const uint32_t serverID)
{
	 CIMServer tmp = m_srvContainer.GetServerByID(serverID);

	 if (tmp.m_netType == (uint32_t)-1)
	 {
		 log (Error, "[CImServersInfo::GetNetTypeByID]: server: %u not found.", serverID);
	 }
	 else
	 {
		 //log (Debug, "[CImServersInfo::GetNetTypeByID]: server: %u type is %u", serverID, tmp.m_netType);
		 return tmp.m_netType;
	 }
	 return -1;
}

// 判断两个服务器是否相同网络类型
bool CImServersInfo::IsSameNetType(const uint32_t srvId1, const uint32_t srvId2)
{
	CIMServer tmp1 = m_srvContainer.GetServerByID(srvId1);
	CIMServer tmp2 = m_srvContainer.GetServerByID(srvId2);

	return (tmp1.m_netType == tmp2.m_netType);
}


// 添加电信，网通 或 双线服务器
void CImServersInfo::onAddSV(const sdaemon::FavorStrSV & fs)
{
	uint32_t type = CImServerFunc::GetNetType(fs);
	string dip = sox::addr_ntoa(fs.dip);
	string wip = sox::addr_ntoa(fs.wip);

	if (type == (uint32_t)-1)
	{
		log (Error, "[CImServersInfo::onAddSV]: faild to match server-type, name(%s)-dip(%s)-wip(%s)-srvID(%d)",
			fs.sstr.data(), dip.data(), wip.data(), fs.serverId);
		return;
	}

	if (fs.serverStatus == sdaemon::RUNNING && fs.sstr != SUFFIX_SESSION && fs.sstr != SUFFIX_LINKD)
	{
		m_srvContainer.AddServer(CIMServer(fs.sstr, fs.serverId, type));
		log (Info, "[CImServersInfo::onAddSV]: add server ok. name(%s)-dip(%s)-wip(%s)-srvID(%d)",
			fs.sstr.data(), dip.data(), wip.data(), fs.serverId);
	}
	else
	{
		log (Info, "[CImServersInfo::onAddSV]: add server faild, serverNotRun. name(%s)-dip(%s)-wip(%s)-srvID(%d)",
			fs.sstr.data(), dip.data(), wip.data(), fs.serverId);
	}
}

// 
void CImServersInfo::onRecoverRefreshSV()
{
	CImServersInfo::onRefreshSV();
	log(Info, "[CImServersInfo::onRecoverRefreshSV]: finish");
}

//
void CImServersInfo::onServerStatusChangeSV(const sdaemon::FavorStrSV & fs)
{
	CImServersInfo::onAddSV(fs);
	log(Info, "[CImServersInfo::onServerStatusChangeSV]: finish");
}

//
void CImServersInfo::onServerRemovedSV(const sdaemon::FavorStrSV & fs)
{
	//uint32_t type = CImServerFunc::GetNetType(fs);
	//if (type == (uint32_t)-1)
	//{
	//	log (Error, "[CImServersInfo::onServerRemovedSV]: faild to match server-type, type is: %d", type);
	//	return;
	//}
	string dip = sox::addr_ntoa(fs.dip);
	string wip = sox::addr_ntoa(fs.wip);

	m_srvContainer.RemoveServer(fs.serverId);
	log (Info, "[CImServersInfo::onServerRemovedSV]: delete server ok. name(%s)-dip(%s)-wip(%s)-srvID(%d)",
		fs.sstr.data(), dip.data(), wip.data(), fs.serverId);
}

//
void CImServersInfo::onRefreshSV()
{
	m_srvContainer.ClearServer();
	vector<sdaemon::FavorStrSV> servers = client->getServersSV();

	for (vector<sdaemon::FavorStrSV>::iterator iter = servers.begin(); iter != servers.end(); iter++)
	{
		CImServersInfo::onAddSV(*iter);
	}

	log(Info, "[CImServersInfo::onRefreshSV]: finish");
}

