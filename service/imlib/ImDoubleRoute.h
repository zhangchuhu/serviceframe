#pragma once 

#include "server_common/server-lib/iclient.h"
#include "server_common/server-lib/id_def.h"

#include <iostream>
#include <algorithm>
#include <map>
#include <set>

#include "protocol/const.h"

namespace server
{

	// 获取服务器所在机房类型
	inline uint32_t GetNetType(const sdaemon::FavorStrSV &fs)
	{
		uint32_t type;
		if (fs.dip == 0 && fs.wip != 0)
		{
			type = CNC;
		}
		else if (fs.dip != 0 && fs.wip == 0)
		{
			type = CTL;
		}
		else if (fs.dip != 0 && fs.wip != 0)
		{
			type = CNC | CTL;
		}
		else
		{
			type = -1;
		}

		return type;
	}

	//双路选择基础类

	class CDRouteServer : public core::IDaemonClientAwareW
	{
		std::string m_strServerName;

		std::set<uint32_t> m_setServerCNC; //ServerId
		std::map<uint32_t, uint32_t> m_mapServerCNC; //Number - ServerId

		std::set<uint32_t> m_setServerCTL; //ServerId
		std::map<uint32_t, uint32_t> m_mapServerCTL; //Number - ServerId

		std::set<uint32_t> m_setServer; //ServerId
		std::map<uint32_t, uint32_t> m_mapServer; //Number - ServerId

	public:
		CDRouteServer(std::string strServerName) : m_strServerName(strServerName) {};

		uint32_t GetServerCount(uint32_t uNetType = CNC | CTL);
		uint32_t Number2ServerId(uint32_t uNumber, uint32_t uNetType = CNC | CTL);
		uint32_t ServerId2Number(uint32_t uServerId, uint32_t uNetType = CNC | CTL);

		virtual void onRefreshSV();
		virtual void onRecoverRefreshSV();
		virtual void onAddSV(const sdaemon::FavorStrSV &fs);		
		virtual void onServerRemovedSV(const sdaemon::FavorStrSV &fs);
		virtual void onServerStatusChangeSV(const sdaemon::FavorStrSV &fs);

	private:
		void __FreshServerSet();
		void __AddServerToSet(const sdaemon::FavorStrSV &fs);
		void __DelServerFromSet(const sdaemon::FavorStrSV &fs);
	};

	//ImChat2双路选择帮助类

	class CDRouteImChat2 : public CDRouteServer
	{
	public:
		CDRouteImChat2() : CDRouteServer(SUFFIX_IMCAHT2D) {};

		virtual uint32_t GetMasterImChat2Id(uint32_t uSeqId, uint32_t uNetType = CNC | CTL);
		virtual uint32_t GetSlaveImChat2Id(uint32_t uSeqId, uint32_t uNetType = CNC | CTL);
	};

	//ImBridge双路选择帮助类

	class CDRouteImBridge : public CDRouteServer
	{
	public:
		CDRouteImBridge() : CDRouteServer(SUFFIX_IMBRIDGE) {};

		virtual uint32_t GetMasterImBridgeId(uint32_t uSeqId, uint32_t uNetType = CNC | CTL);
		virtual uint32_t GetSlaveImBridgeId(uint32_t uSeqId, uint32_t uNetType = CNC | CTL);
	};

	class CDRouteForImLinkAware
	{
		CDRouteImChat2 *m_pDRouteImChat2;
	public:
		CDRouteForImLinkAware() : m_pDRouteImChat2(NULL) {};

		void SetDRouteImChat2(CDRouteImChat2 *pDRouteImChat2)
		{
			m_pDRouteImChat2 = pDRouteImChat2;
		}

		CDRouteImChat2 *GetDRouteImChat2()
		{
			return m_pDRouteImChat2;
		}
	};

	class CDRouteForImChat2Aware
	{
		CDRouteImBridge *m_pDRouteImBridge;
	public:
		CDRouteForImChat2Aware() : m_pDRouteImBridge(NULL) {};

		void SetDRouteImBridge(CDRouteImBridge *pDRouteImBridge)
		{
			m_pDRouteImBridge = pDRouteImBridge;
		}

		CDRouteImBridge *GetDRouteImBridge()
		{
			return m_pDRouteImBridge;
		}
	};

}
