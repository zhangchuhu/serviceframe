#ifndef MULTI_IMSERVERINFO_H
#define MULTI_IMSERVERINFO_H

#include <boost/thread/tss.hpp>
#include <boost/thread/mutex.hpp>
#include "ImServersInfo.h"

namespace server{
	// 多线程环境CImServersInfo实现
	class CMultiImServersInfo : public CImServersInfo
	{
	public:
		virtual ~CMultiImServersInfo() {} 

		virtual std::vector<uint32_t> GetServersIDByNameType(const std::string& name, const uint32_t type);

		virtual std::vector<uint32_t> GetServersIDByNameTypeEx(const std::string& name, const uint32_t type);

		virtual std::vector<uint32_t> GetServersIDByName(const std::string& name);

		virtual uint32_t RandomServerIDByNameType(const std::string& name, const uint32_t type);

		virtual uint32_t RandomServerIDByNameTypeEx(const std::string& name, const uint32_t type);

		virtual uint32_t RandomServerIDByName(const std::string& name);

		virtual uint32_t GetNetTypeByID(const uint32_t serverID);

		virtual bool IsSameNetType(const uint32_t srvId1, const uint32_t srvId2);

	public:
		virtual void onRefreshSV();

		virtual void onRecoverRefreshSV();

		virtual void onAddSV(const sdaemon::FavorStrSV & fs);		

		virtual void onServerRemovedSV(const sdaemon::FavorStrSV & fs);

		virtual void onServerStatusChangeSV(const sdaemon::FavorStrSV & fs);

	private:
		boost::mutex m_mutex;
	};
}

#endif
