#include "ImOnlineIDHelper.h"
#include "core/sox/logger.h"
#include "server_common/server-lib/id_def.h"

using namespace server;
using namespace core;
using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//添加服务器

bool CImOnlineIDHelper::AddServer(uint32_t uImOnlineId)
{
	//boost::mutex::scoped_lock sl(m_mutexHash);

	std::vector<uint32_t>::iterator iter = std::find(m_vecImOnline.begin(), m_vecImOnline.end(), uImOnlineId);
	if(iter == m_vecImOnline.end())
	{
		m_vecImOnline.push_back(uImOnlineId);
		m_hash.AddServer(uImOnlineId);
	}	
	return true;
}

//删除服务器

bool CImOnlineIDHelper::RemoveServer(uint32_t uImOnlineId)
{
	//boost::mutex::scoped_lock sl(m_mutexHash);

	std::vector<uint32_t>::iterator iter = std::find(m_vecImOnline.begin(), m_vecImOnline.end(), uImOnlineId);
	if(iter != m_vecImOnline.end())
	{
		m_vecImOnline.erase(iter);
		m_hash.RemoveServer(uImOnlineId);
		return true;
	}
	return false;
}

//清空所有服务器

void CImOnlineIDHelper::ClearServer()
{
	//boost::mutex::scoped_lock sl(m_mutexHash);

	for(std::vector<uint32_t>::iterator iter = m_vecImOnline.begin(); iter != m_vecImOnline.end(); ++iter)
	{
		m_hash.RemoveServer(*iter);
	}
	m_vecImOnline.clear();
}

//从imlinkdid计算出imonlineid,如果imonlined集合为空,返回 NONE_SERVER_ID

uint32_t CImOnlineIDHelper::ImLinkdId2ImOnlineId(uint32_t uImLinkdId)
{
	uint32_t result = -1;
	std::stringstream ss;

	{
		//boost::mutex::scoped_lock sl(m_mutexHash);

		for (uint32_t i = 0; i < m_vecImOnline.size(); i ++)
		{
			ss << m_vecImOnline[i] << ", ";
		}

		result = m_hash.GetServer(uImLinkdId);
	}

	log(Info, "[CImOnlineIDHelper::ImLinkdId2ImOnlineId]: imlinkd: [%d]   imonline2:[%d]   m_vecImOnline = [%s] ", uImLinkdId, result, ss.str().c_str());
	return result;
}

//ImOnlineId是否存在

bool CImOnlineIDHelper::ImOnlineIdExist(uint32_t uImOnlineId)
{
	//boost::mutex::scoped_lock sl(m_mutexHash);

	return ( std::find(m_vecImOnline.begin(), m_vecImOnline.end(), uImOnlineId) != m_vecImOnline.end() );
	//return ( std::count(m_vecImOnline.begin(), m_vecImOnline.end(), uImOnlineId) >= 1 );
}

//取全部ImOnlineId集合

void CImOnlineIDHelper::GetImOnlineIdMapSet(std::vector<uint32_t> &vecImOnlinedId)
{
	//boost::mutex::scoped_lock sl(m_mutexHash);

	vecImOnlinedId.clear();
	vecImOnlinedId.assign(m_vecImOnline.begin(), m_vecImOnline.end());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CImOnlineHelperSelf::DoRefreshSV()
{
	CHECK_DAEMONCLIENT(m_pDaemonClient);

	m_ImOnlineIdHelper.ClearServer();	//先清空hashset

	std::vector<sdaemon::FavorStrSV> ff = m_pDaemonClient->getServersSV();

	for(std::vector<sdaemon::FavorStrSV>::iterator it = ff.begin(); it != ff.end(); ++it)
	{
		if (it->sstr == SUFFIX_IMONLINE2 && it->serverStatus == sdaemon::RUNNING)
		{
			m_ImOnlineIdHelper.AddServer(it->serverId);

			log(Info, "[CImOnlineHelperSelf::DoRefreshSV] AddServer %d", it->serverId);
		}
	}
}

bool CImOnlineHelperSelf::DoAddSV(const sdaemon::FavorStrSV &fs)
{
	CHECK_DAEMONCLIENT_RET(m_pDaemonClient, false);

	if(fs.sstr == SUFFIX_IMONLINE2 && fs.serverStatus == sdaemon::RUNNING)
	{
		log(Info, "[CImOnlineHelperSelf::DoAddSV] AddServer %d", fs.serverId);

		return m_ImOnlineIdHelper.AddServer(fs.serverId);
	}
	return false;
}

bool CImOnlineHelperSelf::DoServerRemovedSV(const sdaemon::FavorStrSV &fs)
{
	CHECK_DAEMONCLIENT_RET(m_pDaemonClient, false);

	if(fs.sstr == SUFFIX_IMONLINE2 && m_ImOnlineIdHelper.ImOnlineIdExist(fs.serverId))
	{
		log(Info, "[CImOnlineHelperSelf::DoServerRemovedSV] RemoveServer %d", fs.serverId);

		return m_ImOnlineIdHelper.RemoveServer(fs.serverId);
	}
	return false;
}

void CImOnlineHelperSelf::DoRecoverRefreshSV()
{
	this->DoRefreshSV();
}

void CImOnlineHelperSelf::DoServerStatusChangeSV(const sdaemon::FavorStrSV &fs)
{
	this->DoAddSV(fs);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CWatchImOnlineDaemonClient::onRefreshSV()
{
	log(Info, "[CWatchImOnlineDaemonClient::onRefreshSV] start...");

	m_ImOnlineHelperSelf.SetDaemonClient(client);
	m_ImOnlineHelperSelf.DoRefreshSV();

	//只需要根据具体情况作哈希调整了

	onImOnlineIDSetChanged();

	log(Info, "[CWatchImOnlineDaemonClient::onRefreshSV] end...");
}

void CWatchImOnlineDaemonClient::onAddSV(const sdaemon::FavorStrSV & fs)
{
	log(Info, "[CWatchImOnlineDaemonClient::onAddSV] start...");

	m_ImOnlineHelperSelf.SetDaemonClient(client);
	bool bAddSV = m_ImOnlineHelperSelf.DoAddSV(fs);

	//只需要根据具体情况作哈希调整了

	if (bAddSV)
	{
		onImOnlineIDSetChanged();
	}

	log(Info, "[CWatchImOnlineDaemonClient::onAddSV] end...");
}

void CWatchImOnlineDaemonClient::onServerRemovedSV(const sdaemon::FavorStrSV & fs)
{
	log(Info, "[CWatchImOnlineDaemonClient::onServerRemovedSV] start...");

	m_ImOnlineHelperSelf.SetDaemonClient(client);
	bool bRemoveSV = m_ImOnlineHelperSelf.DoServerRemovedSV(fs);

	//只需要根据具体情况作哈希调整了

	if (bRemoveSV)
	{
		onImOnlineIDSetChanged();
	}

	log(Info, "[CWatchImOnlineDaemonClient::onServerRemovedSV] end...");
}

void CWatchImOnlineDaemonClient::onRecoverRefreshSV()
{
	log(Info, "[CWatchImOnlineDaemonClient::onRecoverRefreshSV] start...");

	onRefreshSV();

	log(Info, "[CWatchImOnlineDaemonClient::onRecoverRefreshSV] start...");
}

void CWatchImOnlineDaemonClient::onServerStatusChangeSV(const sdaemon::FavorStrSV & fs)
{
	log(Info, "[CWatchImOnlineDaemonClient::onServerStatusChangeSV] start...");

	onAddSV(fs);

	log(Info, "[CWatchImOnlineDaemonClient::onServerStatusChangeSV] start...");
}
