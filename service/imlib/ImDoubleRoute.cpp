#include "ImDoubleRoute.h"
#include "core/sox/logger.h"

using namespace server;
using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint32_t CDRouteServer::GetServerCount(uint32_t uNetType)
{
	if (uNetType == CNC)
	{
		return m_mapServerCNC.size();
	}
	else if (uNetType == CTL)
	{
		return m_mapServerCTL.size();
	}

	return m_mapServer.size();
}

uint32_t CDRouteServer::Number2ServerId(uint32_t uNumber, uint32_t uNetType)
{
	if (uNetType == CNC)
	{
		std::map<uint32_t, uint32_t>::iterator iterServer = m_mapServerCNC.find(uNumber);
		if (iterServer != m_mapServerCNC.end())
			return iterServer->second;
	}
	else if (uNetType == CTL)
	{
		std::map<uint32_t, uint32_t>::iterator iterServer = m_mapServerCTL.find(uNumber);
		if (iterServer != m_mapServerCTL.end())
			return iterServer->second;
	}
	else
	{
		std::map<uint32_t, uint32_t>::iterator iterServer = m_mapServer.find(uNumber);
		if (iterServer != m_mapServer.end())
			return iterServer->second;
	}

	return -1;
}

uint32_t CDRouteServer::ServerId2Number(uint32_t uServerId, uint32_t uNetType)
{
	if (uNetType == CNC)
	{
		std::map<uint32_t, uint32_t>::iterator iterServer = m_mapServerCNC.begin();
		for ( ; iterServer != m_mapServerCNC.end(); iterServer ++)
		{
			if ( iterServer->second == uServerId )
				return iterServer->first;
		}
	}
	else if (uNetType == CTL)
	{
		std::map<uint32_t, uint32_t>::iterator iterServer = m_mapServerCTL.begin();
		for ( ; iterServer != m_mapServerCTL.end(); iterServer ++)
		{
			if ( iterServer->second == uServerId )
				return iterServer->first;
		}
	}
	else
	{
		std::map<uint32_t, uint32_t>::iterator iterServer = m_mapServer.begin();
		for ( ; iterServer != m_mapServer.end(); iterServer ++)
		{
			if ( iterServer->second == uServerId )
				return iterServer->first;
		}
	}

	return -1;
}

void CDRouteServer::onRefreshSV()
{
	log (Info, "[CDRouteServer::onRefreshSV] start");

	__FreshServerSet();

	log (Info, "[CDRouteServer::onRefreshSV] end");
}

void CDRouteServer::onAddSV(const sdaemon::FavorStrSV &fs)
{
	log (Info, "[CDRouteServer::onAddSV] start ServerName[%s] ServerId[%d] Status[%d]", fs.sstr.c_str(), fs.serverId, fs.serverStatus);

	__AddServerToSet(fs);

	log (Info, "[CDRouteServer::onAddSV] end");
}

void CDRouteServer::onServerRemovedSV(const sdaemon::FavorStrSV &fs)
{
	log (Info, "[CDRouteServer::onServerRemovedSV] start ServerName[%s] ServerId[%d]", fs.sstr.c_str(), fs.serverId);

	__DelServerFromSet(fs);

	log (Info, "[CDRouteServer::onServerRemovedSV] end");
}

void CDRouteServer::onRecoverRefreshSV()
{
	log (Info, "[CDRouteServer::onRecoverRefreshSV] start");

	onRefreshSV();

	log (Info, "[CDRouteServer::onRecoverRefreshSV] end");
}

void CDRouteServer::onServerStatusChangeSV(const sdaemon::FavorStrSV &fs)
{
	log (Info, "[CDRouteServer::onServerStatusChangeSV] start ServerName[%s] ServerId[%d] Status[%d]", fs.sstr.c_str(), fs.serverId, fs.serverStatus);

	onAddSV(fs);

	log (Info, "[CDRouteServer::onServerStatusChangeSV] end");
}

void CDRouteServer::__FreshServerSet()
{
	m_setServer.clear();

	vector<sdaemon::FavorStrSV> vecServer = client->getServersByNameSV(m_strServerName);
	for (vector<sdaemon::FavorStrSV>::iterator iterServer = vecServer.begin(); iterServer != vecServer.end(); iterServer ++)
	{
		if (iterServer->serverStatus == sdaemon::RUNNING)
		{
			uint32_t uServerId = iterServer->serverId;

			uint32_t uNetType = GetNetType(*iterServer);
			if (uNetType == CTL)
			{
				m_setServerCTL.insert( uServerId );
			}
			else if (uNetType == CNC)
			{
				m_setServerCNC.insert( uServerId );
			}

			m_setServer.insert( uServerId );
		}
	}

	m_mapServerCTL.clear();
	m_mapServerCNC.clear();
	m_mapServer.clear();

	uint32_t uNumber = 0;
	std::set<uint32_t>::iterator iterServer = m_setServerCTL.begin();
	for ( ; iterServer != m_setServerCTL.end(); iterServer ++, uNumber++)
	{
		m_mapServerCTL.insert ( std::make_pair(uNumber, *iterServer) );
	}

	uNumber = 0;
	iterServer = m_setServerCNC.begin();
	for ( ; iterServer != m_setServerCNC.end(); iterServer ++, uNumber++)
	{
		m_mapServerCNC.insert ( std::make_pair(uNumber, *iterServer) );
	}

	uNumber = 0;
	iterServer = m_setServer.begin();
	for ( ; iterServer != m_setServer.end(); iterServer ++, uNumber++)
	{
		m_mapServer.insert ( std::make_pair(uNumber, *iterServer) );
	}
}

void CDRouteServer::__AddServerToSet(const sdaemon::FavorStrSV &fs)
{
	if (fs.sstr == m_strServerName && fs.serverStatus == sdaemon::RUNNING)
	{
		uint32_t uNetType = GetNetType(fs);
		if (uNetType == CTL)
		{
			m_setServerCTL.insert( fs.serverId );

			m_mapServerCTL.clear();

			uint32_t uNumber = 0;//有一个服务加入，就重新编号
			std::set<uint32_t>::iterator iterServer = m_setServerCTL.begin();
			for ( ; iterServer != m_setServerCTL.end(); iterServer ++, uNumber++)
			{
				m_mapServerCTL.insert ( std::make_pair(uNumber, *iterServer) );
			}
		}
		else if (uNetType == CNC)
		{
			m_setServerCNC.insert( fs.serverId );

			m_mapServerCNC.clear();

			uint32_t uNumber = 0;
			std::set<uint32_t>::iterator iterServer = m_setServerCNC.begin();
			for ( ; iterServer != m_setServerCNC.end(); iterServer ++, uNumber++)
			{
				m_mapServerCNC.insert ( std::make_pair(uNumber, *iterServer) );
			}
		}

		m_setServer.insert( fs.serverId );

		m_mapServer.clear();

		uint32_t uNumber = 0;
		std::set<uint32_t>::iterator iterServer = m_setServer.begin();
		for ( ; iterServer != m_setServer.end(); iterServer ++, uNumber++)
		{
			m_mapServer.insert ( std::make_pair(uNumber, *iterServer) );
		}
	}
}

void CDRouteServer::__DelServerFromSet(const sdaemon::FavorStrSV &fs)
{
	if (fs.sstr == m_strServerName)
	{
		uint32_t uNetType = GetNetType(fs);
		if (uNetType == CTL)
		{
			m_setServerCTL.erase(fs.serverId);

			m_mapServerCTL.clear();

			uint32_t uNumber = 0;
			std::set<uint32_t>::iterator iterServer = m_setServerCTL.begin();
			for ( ; iterServer != m_setServerCTL.end(); iterServer ++, uNumber++)
			{
				m_mapServerCTL.insert ( std::make_pair(uNumber, *iterServer) );
			}
		}
		else if (uNetType == CNC)
		{
			m_setServerCNC.erase(fs.serverId);

			m_mapServerCNC.clear();

			uint32_t uNumber = 0;
			std::set<uint32_t>::iterator iterServer = m_setServerCNC.begin();
			for ( ; iterServer != m_setServerCNC.end(); iterServer ++, uNumber++)
			{
				m_mapServerCNC.insert ( std::make_pair(uNumber, *iterServer) );
			}
		}

		m_setServer.erase(fs.serverId);

		m_mapServer.clear();

		uint32_t uNumber = 0;
		std::set<uint32_t>::iterator iterServer = m_setServer.begin();
		for ( ; iterServer != m_setServer.end(); iterServer ++, uNumber++)
		{
			m_mapServer.insert ( std::make_pair(uNumber, *iterServer) );
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint32_t CDRouteImChat2::GetMasterImChat2Id(uint32_t uSeqId, uint32_t uNetType)
{
	uint32_t uImChat2Count = GetServerCount(uNetType);
	if (uImChat2Count == 0)
	{
		log (Info, "[CDRouteImChat2::GetMasterImChat2Id] nettype %ld imchat2 count is empty", uNetType);
		return -1;
	}

	uint32_t uImChat2NumberMaster = uSeqId % GetServerCount(uNetType);
	uint32_t uImChat2IdMaster = Number2ServerId(uImChat2NumberMaster, uNetType);
	log (Info, "[CDRouteImChat2::GetMasterImChat2Id] nettype %ld imchat2 %ld", uNetType, uImChat2IdMaster);
	return uImChat2IdMaster;
}

uint32_t CDRouteImChat2::GetSlaveImChat2Id(uint32_t uSeqId, uint32_t uNetType)
{
	uint32_t uImChat2Count = GetServerCount(uNetType);
	if (uImChat2Count == 0)
	{
		log (Info, "[CDRouteImChat2::GetSlaveImChat2Id] nettype %ld imchat2 count is empty", uNetType);
		return -1;
	}

	uint32_t uImChat2NumberSlave = (uSeqId + 1) % GetServerCount(uNetType);
	uint32_t uImChat2IdSlave = Number2ServerId(uImChat2NumberSlave, uNetType);

	log (Info, "[CDRouteImChat2::GetSlaveImChat2Id] nettype %ld imchat2 %ld", uNetType, uImChat2IdSlave);
	return uImChat2IdSlave;
}

uint32_t CDRouteImBridge::GetMasterImBridgeId(uint32_t uSeqId, uint32_t uNetType)
{
	uint32_t uImBridgeCount = GetServerCount(uNetType);
	if (uImBridgeCount == 0)
	{
		log (Info, "[CDRouteImBridge::GetMasterImBridgeId] nettype %ld imbridge count is empty", uNetType);
		return -1;
	}

	uint32_t uImBridgeNumberMaster = uSeqId % GetServerCount(uNetType);
	uint32_t uImBridgeIdMaster = Number2ServerId(uImBridgeNumberMaster, uNetType);

	log (Info, "[CDRouteImBridge::GetMasterImBridgeId] nettype %ld imbridge %ld", uNetType, uImBridgeIdMaster);
	return uImBridgeIdMaster;
}

uint32_t CDRouteImBridge::GetSlaveImBridgeId(uint32_t uSeqId, uint32_t uNetType)
{
	uint32_t uImBridgeCount = GetServerCount(uNetType);
	if (uImBridgeCount == 0)
	{
		log (Info, "[CDRouteImBridge::GetSlaveImBridgeId] nettype %ld imbridge count is empty", uNetType);
		return -1;
	}

	uint32_t uImBridgeNumberSlave = (uSeqId + 1) % GetServerCount(uNetType);
	uint32_t uImBridgeIdSlave = Number2ServerId(uImBridgeNumberSlave, uNetType);

	log (Info, "[CDRouteImBridge::GetSlaveImBridgeId] nettype %ld imbridge %ld", uNetType, uImBridgeIdSlave);
	return uImBridgeIdSlave;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
