#include "ImOnlineHashHelper.h"
#include "core/sox/logger.h"

using namespace std;
using namespace core;
using namespace sdaemon;
using namespace server;


CImOnHashHelper::~CImOnHashHelper()
{
	for (map<uint32_t, PairHashMap_T*>::iterator it = m_mapGrpConsisHash.begin(); it != m_mapGrpConsisHash.end(); ++it)
	{
		delete it->second;
	}
}

// 先hash imonline组，再hash 组内imonline

uint32_t CImOnHashHelper::DoubleHash(uint32_t uUid)
{
	uint32_t uGroupId = m_hashGroup.first.GetValueByKey(uUid);

	if (uGroupId != (uint32_t)-1)
	{
		return HashInOneSrvGrp(uUid, uGroupId);
	}

	return -1;
}

//

uint32_t CImOnHashHelper::HashBySrvGrp(uint32_t key)
{
	uint32_t uGroupId = m_hashGroup.first.GetValueByKey(key);

	return uGroupId;
}

// 对组内imonline进行hash

uint32_t CImOnHashHelper::HashInOneSrvGrp(uint32_t uUid, uint32_t uGroupId)
{
	map<uint32_t, PairHashMap_T*>::iterator it = m_mapGrpConsisHash.find(uGroupId);
	if (it != m_mapGrpConsisHash.end())
	{
		uint32_t uGroupId = it->second->first.GetValueByKey(uUid);

		return uGroupId;
	}

	return -1;
}

// 根据uid, 对uGroupId组内的imonline进行hash

void CImOnHashHelper::BatchHashInOneSrvGrp(uint32_t uGroupId, 
					  const std::set<uint32_t>& setUid, 
					  std::map< uint32_t, std::set<uint32_t> >& mapSrvUids	)
{
	map<uint32_t, PairHashMap_T*>::iterator itMap = m_mapGrpConsisHash.find(uGroupId);
	if (itMap == m_mapGrpConsisHash.end())
	{
		log (Error, "[CImOnHashHelper::BatchHashInOneSrvGrp] group %d 's server not in my circle.", uGroupId);
		return;
	}

	PairHashMap_T* tmpPair = itMap->second;
	for (set<uint32_t>::const_iterator it = setUid.begin(); it != setUid.end(); ++it)
	{
		uint32_t serverId = tmpPair->first.GetValueByKey(*it);
		if (serverId != (uint32_t)-1)
		{
			mapSrvUids[serverId].insert(*it);
		}
	}
}

// 跟据uid，对每个online组的imonline进行hash，uGroupId组除外

void CImOnHashHelper::BatchHashInAllSrvGrpEx(uint32_t uSrvGrp, 
											 const std::set<uint32_t>& setUids, 
											 std::map<uint32_t, std::set<uint32_t> >& mapSrv2Uids)
{
	for (map<uint32_t, PairHashMap_T*>::iterator it = m_mapGrpConsisHash.begin(); it != m_mapGrpConsisHash.end(); ++it)
	{
		if (it->first != uSrvGrp)
		{
			BatchHashInOneSrvGrp(it->first, setUids, mapSrv2Uids);
		}
	}
}

void CImOnHashHelper::BatchHashInAllSrvGrpEx(uint32_t uSrvGrp, uint32_t uUid, std::set<uint32_t>& setSrvs)
{
	for (map<uint32_t, PairHashMap_T*>::iterator it = m_mapGrpConsisHash.begin(); it != m_mapGrpConsisHash.end(); ++it)
	{
		uint32_t uSrvId;
		if (it->first != uSrvGrp && (uSrvId = HashInOneSrvGrp(uUid, it->first)) != (uint32_t)-1)
		{
			setSrvs.insert(uSrvId);
		}
	}
}


// 加入hash圈

void CImOnHashHelper::AddHashNode(uint32_t uServerId, uint32_t uGroupId, ISPType net /* = CTL */)
{
	map<uint32_t, PairHashMap_T*>::iterator it = m_mapGrpConsisHash.find(uGroupId);
	if (it != m_mapGrpConsisHash.end())
	{
		it->second->first.AddServer(uServerId);
	}
	else
	{
		PairHashMap_T* tmpPair = new PairHashMap_T;
		tmpPair->first.AddServer(uServerId);
		m_mapGrpConsisHash.insert(make_pair(uGroupId, tmpPair));
	}

	// 只记录特定ISP的服务器组
	if (net == m_ISP && m_setGrpId.find(uGroupId) == m_setGrpId.end())
	{
		m_setGrpId.insert(uGroupId);
		m_hashGroup.first.AddServer(uGroupId);
	}
	
	log (Info, "[CImOnHashHelper::AddHashNode] add server(%d)-group(%d) ok.", uServerId, uGroupId);
}

// 从hash圈中删除

void CImOnHashHelper::RemoveHashNode(uint32_t uServerId, uint32_t uGroupId)
{
	map<uint32_t, PairHashMap_T*>::iterator it = m_mapGrpConsisHash.find(uGroupId);
	if (it != m_mapGrpConsisHash.end())
	{
		PairHashMap_T* tmpPair = it->second;
		tmpPair->first.RemoveServer(uServerId);

		// 该group内没有server
		if (tmpPair->second.empty())
		{
			delete tmpPair;
			tmpPair = NULL;
			m_mapGrpConsisHash.erase(it);
			m_hashGroup.first.RemoveServer(uGroupId);
			m_setGrpId.erase(uGroupId);

			log(Info, "[CImOnHashHelper::RemoveHashNode] Remove server(%d) and group(%d)", uServerId, uGroupId);
		}
		else
		{
			log (Info, "[CImOnHashHelper::RemoveHashNode] Remove server(%d) but not remove group(%d)", uServerId, uGroupId);
		}
	}
}

// Daemon事件

void CImOnHashHelper::onRefreshSV()
{
	m_hashGroup.second.clear();
	m_mapGrpConsisHash.clear();
	m_setGrpId.clear();
	log (Info, "[CImOnHashHelper::onRefreshSV] Clear all hash node.");
	
	vector<FavorStrSV> fs = client->getServersByNameSV(m_strSuffix);
	for (vector<FavorStrSV>::iterator it = fs.begin(); it != fs.end(); ++it)
	{
		if (it->serverStatus != RUNNING)
		{
			continue;
		}

		if (it->dip != 0) // 双线？ 当电信吧。。。
		{
			AddHashNode(it->serverId, it->groupId);
		}
		else if (it->wip != 0)
		{
			AddHashNode(it->serverId, it->groupId, CNC);
		}
	}
}

void CImOnHashHelper::onAddSV(const sdaemon::FavorStrSV & fs)
{
	if (fs.sstr == m_strSuffix && fs.serverStatus == RUNNING)
	{
		if (fs.dip != 0)
		{
			AddHashNode(fs.serverId, fs.groupId);
		}
		else if (fs.wip != 0)
		{
			AddHashNode(fs.serverId, fs.groupId, CNC);
		}
	}
}

void CImOnHashHelper::onRecoverRefreshSV()
{
	onRefreshSV();
}

void CImOnHashHelper::onServerRemovedSV(const sdaemon::FavorStrSV & fs)
{
	if (fs.sstr == m_strSuffix)
	{
		RemoveHashNode(fs.serverId, fs.groupId);
	}
}

void CImOnHashHelper::onServerStatusChangeSV(const sdaemon::FavorStrSV & fs)
{
	onAddSV(fs);
}
