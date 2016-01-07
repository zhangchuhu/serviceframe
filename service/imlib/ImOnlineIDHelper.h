#ifndef IMONLINEIDHELPER_H_
#define IMONLINEIDHELPER_H_

#include <string.h>
#include <algorithm>
#include <map>
#include <vector>
#include "./ConsistentHashing.h"

#include "server_common/server-lib/iclient.h" //define IDaemonClientAwareW
#include "server_common/server-lib/RouteTarget.h" //define IRouteTarget
#include "common/packet.h"
#include "core/sox/logger.h"

#include <common/boost/thread/mutex.hpp>

namespace server
{
	//---------------------------------------------------------------------------------------------
	//注意：本类以imlinkdid作hash，计算出ImOnlineId

	class CImOnlineIDHelper
	{
	public:
		CImOnlineIDHelper() {};

		bool AddServer(uint32_t uImOnlineId);

		bool RemoveServer(uint32_t uImOnlineId);

		void ClearServer();

		//从imlinkdid计算出imonlineid,如果imonlined集合为空,返回 NONE_SERVER_ID

		uint32_t ImLinkdId2ImOnlineId(uint32_t uImLinkdId);

		//ImOnlineId是否存在

		bool ImOnlineIdExist(uint32_t uImOnlineId);

		//取全部ImOnlineId集合

		void GetImOnlineIdMapSet(std::vector<uint32_t> &vecImOnlinedId);

	private:
		UserConsistentHash m_hash;
		std::vector<uint32_t> m_vecImOnline;
		boost::mutex m_mutexHash;
	};

	//---------------------------------------------------------------------------------------------
	//本类主要供ImOnline来根据ImLinkdId来判断是不是自己

	#define CHECK_DAEMONCLIENT(x)	{ if ((x) == NULL) return; }
	#define CHECK_DAEMONCLIENT_RET(x, ret)	{ if ((x) == NULL) return (ret); }

	class CImOnlineHelperSelf
	{
		CImOnlineIDHelper m_ImOnlineIdHelper;
		core::IDaemonClient *m_pDaemonClient;

	public:
		CImOnlineHelperSelf() : m_pDaemonClient(NULL) {};

		//使用DoXXX方法前，必须为本类设置IDaemonClient指针

		virtual void SetDaemonClient(core::IDaemonClient *pDaemonClient)
		{
			m_pDaemonClient = pDaemonClient;
		}

		virtual void DoRefreshSV();
		virtual bool DoAddSV(const sdaemon::FavorStrSV &fs);
		virtual bool DoServerRemovedSV(const sdaemon::FavorStrSV & fs);
		virtual void DoRecoverRefreshSV();
		virtual void DoServerStatusChangeSV(const sdaemon::FavorStrSV & fs);

		//通过ImLinkdId计算ImOnlineId

		virtual uint32_t ImLinkdId2ImOnlineId(uint32_t uImLinkdId)
		{
			return m_ImOnlineIdHelper.ImLinkdId2ImOnlineId(uImLinkdId);
		}

		//判断ImLinkdId计算后是否与指定的ImOnlineId相等

		virtual bool IsEqualImOnlineId(uint32_t uImLinkdId, uint32_t uImOnlineId)
		{
			return ( uImOnlineId == this->ImLinkdId2ImOnlineId(uImLinkdId) );
		}
	};

	//---------------------------------------------------------------------------------------------
	//本类的主要功能是供CImOnDispatcher向ImOnline发包

	class CWatchImOnlineDaemonClient : public core::IDaemonClientAwareW,
									   public core::IRouteTarget
	{
		CImOnlineHelperSelf m_ImOnlineHelperSelf;

	public:

		virtual void onRefreshSV();
		virtual void onAddSV(const sdaemon::FavorStrSV & fs);		
		virtual void onServerRemovedSV(const sdaemon::FavorStrSV & fs);
		virtual void onRecoverRefreshSV();
		virtual void onServerStatusChangeSV(const sdaemon::FavorStrSV & fs);

		//ImOnline发生变化

		virtual void onImOnlineIDSetChanged() {};

		virtual bool RouteToImOnline(const std::string &strFrom, uint32_t uImLinkdId, uint32_t Uri, const sox::Marshallable &objMarshal)
		{
			uint32_t uImOnline2Id = m_ImOnlineHelperSelf.ImLinkdId2ImOnlineId(uImLinkdId);
			routeS(strFrom, uImOnline2Id, Uri, objMarshal);
			log(Info, "[CWatchImOnlineDaemonClient::RouteToImOnline] from:%s to imonline2id:%d ", strFrom.c_str(), uImOnline2Id);
			return true;
		}

		virtual bool RouteToImOnline(const std::string &strFrom, uint32_t uImLinkdId, uint32_t Uri, const std::string &strPacket)
		{
			uint32_t uImOnline2Id = m_ImOnlineHelperSelf.ImLinkdId2ImOnlineId(uImLinkdId);
			routeS(strFrom, uImOnline2Id, Uri, strPacket);
			log(Info, "[CWatchImOnlineDaemonClient::RouteToImOnline] from:%s to imonline2id:%d ", strFrom.c_str(), uImOnline2Id);
			return true;
		}
	};
}

#endif
