#ifndef SERVER_MULTIBROADCAST_H_
#define SERVER_MULTIBROADCAST_H_

#include <boost/thread/tss.hpp>
#include <boost/thread/mutex.hpp>
#include "ImTransHelper.h"
#include "ImServersInfo.h"

// 多线程环境ImTransferHelper

namespace server
{
	class CMultiImTransHelper : public server::CImTransHelper
	{
	public:
		// CImTransfer 接口
		virtual uint32_t GetTransferID(uint32_t Uid, uint32_t netType)
		{
			boost::mutex::scoped_lock s1(m_mutex);
			return CImTransHelper::GetTransferID(Uid, netType);
		}

		virtual void BatchGetTransferID(const std::vector<uint32_t>& vecUid, std::vector<uint32_t>& vecTransId, uint32_t netType)
		{
			boost::mutex::scoped_lock s1(m_mutex);
			CImTransHelper::BatchGetTransferID(vecUid, vecTransId, netType);
		}

		virtual void BatchGetTransferID(const std::set<uint32_t>& setUid, std::vector<uint32_t>& vecTransId, uint32_t netType)
		{
			boost::mutex::scoped_lock s1(m_mutex);
			CImTransHelper::BatchGetTransferID(setUid, vecTransId, netType);
		}

		virtual void BatchGetTransferByimlinkd(const std::vector<uint32_t>& vecUid, std::vector<uint32_t>& vecTransId, uint32_t linkdId)
		{
			boost::mutex::scoped_lock s1(m_mutex);
			CImTransHelper::BatchGetTransferByimlinkd(vecUid, vecTransId, linkdId);
		}

		virtual void BatchGetTransferByimlinkds(std::vector<CIMUserTransfer>& vec)
		{
			boost::mutex::scoped_lock s1(m_mutex);
			CImTransHelper::BatchGetTransferByimlinkds(vec);
		}


		//--------------------------------------------------------------------
		// CImServersInfo 接口

		virtual std::vector<uint32_t> GetServersIDByNameType(const std::string& name, const uint32_t type)
		{
			boost::mutex::scoped_lock s1(m_mutex);
			return CImServersInfo::GetServersIDByNameType(name, type);
		}

		virtual std::vector<uint32_t> GetServersIDByName(const std::string& name)
		{
			boost::mutex::scoped_lock s1(m_mutex);
			return CImServersInfo::GetServersIDByName(name);
		}

		virtual uint32_t RandomServerIDByNameType(const std::string& name, const uint32_t type)
		{
			boost::mutex::scoped_lock s1(m_mutex);
			return CImServersInfo::RandomServerIDByNameType(name, type);
		}

		virtual uint32_t RandomServerIDByName(const std::string& name)
		{
			boost::mutex::scoped_lock s1(m_mutex);
			return CImServersInfo::RandomServerIDByName(name);
		}

		virtual uint32_t GetNetTypeByID(const uint32_t serverID)
		{
			boost::mutex::scoped_lock s1(m_mutex);
			return CImServersInfo::GetNetTypeByID(serverID);
		}

		virtual bool IsSameNetType(const uint32_t srvId1, const uint32_t srvId2)
		{
			boost::mutex::scoped_lock s1(m_mutex);
			return CImServersInfo::IsSameNetType(srvId1, srvId2);
		}

		//---------------------------------------------------------------------
		//IDaemonClient 接口实现

		virtual void onRefreshSV()
		{
			boost::mutex::scoped_lock s1(m_mutex);
			CImTransHelper::onRefreshSV();
		}

		virtual void onAddSV(const sdaemon::FavorStrSV & fs)
		{
			boost::mutex::scoped_lock s1(m_mutex);
			CImTransHelper::onAddSV(fs);
		}

		virtual void onServerRemovedSV(const sdaemon::FavorStrSV & fs)
		{
			boost::mutex::scoped_lock s1(m_mutex);
			CImTransHelper::onServerRemovedSV(fs);
		}

		virtual void onRecoverRefreshSV()
		{
			boost::mutex::scoped_lock s1(m_mutex);
			CImTransHelper::onRecoverRefreshSV();
		}

		virtual void onServerStatusChangeSV(const sdaemon::FavorStrSV & fs)
		{
			boost::mutex::scoped_lock s1(m_mutex);
			CImTransHelper::onServerStatusChangeSV(fs);
		}

	protected:

		virtual bool _RouteBySrvId(uint32_t Uid, uint32_t linkdId, uint32_t Uri, const sox::Marshallable &obj);

		virtual bool _RouteByNetType(uint32_t Uid, uint32_t netType, uint32_t Uri, const sox::Marshallable &obj);

	private:
		boost::mutex m_mutex;
	};
}

#endif
