#ifndef IMSERVERS_INFO_H
#define IMSERVERS_INFO_H

#include <map>
#include <vector>
#include <string>

#include"server_common/server-lib/iclient.h"
#include "int_types.h"
#include"server_common/server-lib/iclient.h"
#include "packet.h"
#include "protocol/const.h"
#include "core/ibase.h"

namespace server
{
	class CImServerFunc
	{
	public:
		// 获取服务器所在机房类型
		static uint32_t GetNetType(const sdaemon::FavorStrSV & fs)
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
	};


	// 服务器信息：
	struct CIMServer : public sox::Marshallable
	{
		std::string m_sstr;              // 服务器类型
		uint32_t m_serverId;             // 服务器ID
		uint32_t m_netType;              // 服务器所在机房网络类型

		CIMServer() : m_sstr(""), m_serverId(-1), m_netType(-1)
		{
		}

		CIMServer(std::string name, uint32_t srvId, uint32_t netType) : m_sstr(name), m_serverId(srvId), m_netType(netType)
		{
		}
			
		virtual void marshal(sox::Pack &pk) const 
		{
			pk << m_sstr;
			pk << m_serverId << m_netType;
		}

		virtual void unmarshal(const sox::Unpack &up) 
		{
			up >> m_sstr;
			up >> m_serverId >> m_netType;
		}
	};

	
	// 保存服务器信息的容器
	class CImServersContainer
	{
	public:
		typedef std::map<uint32_t, CIMServer> ServersMap_T;

		void AddServer(const CIMServer& srv);
		
		void RemoveServer(const uint32_t srvId);
		
		void ClearServer();

		// 根据服务器类型，获取服务器ID
		void GetServersByName(const std::string& name, std::vector<uint32_t>& vecIDs);
		
		// 根据服务器类型和网络类型，获取服务器ID
		void GetServersByNameType(const std::string& name, const uint32_t type, std::vector<uint32_t>& vecIDs);

		// 根据服务器类型和网络类型，获取服务器ID（匹配网络类型的时候，包含双线服务器）
		void GetServersByNameTypeEx(const std::string& name, const uint32_t type, std::vector<uint32_t>& vecIDs);

		CIMServer GetServerByID(const uint32_t serverID);
		
	private:
		// 处于RUNNING态的所有服务器
		ServersMap_T m_mapServers;
	};

	class ICImServersInfo : public core::IDaemonClientAwareW
	{
	public:
		virtual ~ICImServersInfo(){}

		// 通过服务器类型名和网络类型，获取相同网络下同一类型的所有服务器ID
		// 注意： 包含自己
		virtual std::vector<uint32_t> GetServersIDByNameType(const std::string& name, const uint32_t type) = 0;

		// 匹配网络类型时，包含双线
		virtual std::vector<uint32_t> GetServersIDByNameTypeEx(const std::string& name, const uint32_t type) = 0;
		
		// 通过服务器类型名，获取一类型的所有服务器ID
		// 注意： 包含自己
		virtual std::vector<uint32_t> GetServersIDByName(const std::string& name) = 0;
		
		// 通过服务器名和网络类型，随即获取一个服务器ID
		//   成功返回：id
		//   失败返回：-1  
		// 注意： 包含自己
		virtual uint32_t RandomServerIDByNameType(const std::string& name, const uint32_t type) = 0;

		// 匹配网络类型时，包含双线
		virtual uint32_t RandomServerIDByNameTypeEx(const std::string& name, const uint32_t type) = 0;
		
		// 通过服务器名，随即获取一个服务器ID
		//   成功返回：id
		//   失败返回：-1  
		// 注意： 包含自己
		virtual uint32_t RandomServerIDByName(const std::string& name) = 0;
		
		// 获取网络类型
		virtual uint32_t GetNetTypeByID(const uint32_t serverID) = 0;

		// 判断两个服务器是否相同网络类型
		virtual bool IsSameNetType(const uint32_t srvId1, const uint32_t srvId2) = 0;
	};

	//
	class ICImServersInfoAware
	{
	protected:
		ICImServersInfo *m_srvInfo;
	public:
		virtual ~ICImServersInfoAware() {}
		virtual void SetImServerInfoHelper(ICImServersInfo *obj)
		{
			m_srvInfo = obj; 
		}
	};


///////////////////////////////// 单线程环境服务器信息容器实现 ////////////////////////////////

	class CImServersInfo : public ICImServersInfo
	{
	public:
		virtual ~CImServersInfo() {}

		// 通过服务器类型名和网络类型，获取相同网络下同一类型的所有服务器ID
		virtual std::vector<uint32_t> GetServersIDByNameType(const std::string& name, const uint32_t type);

		virtual std::vector<uint32_t> GetServersIDByNameTypeEx(const std::string& name, const uint32_t type);

		// 通过服务器类型名，获取一类型的所有服务器ID
		virtual std::vector<uint32_t> GetServersIDByName(const std::string& name);
		
		// 通过服务器名和网络类型，随即获取一个服务器ID
		virtual uint32_t RandomServerIDByNameType(const std::string& name, const uint32_t type);

		virtual uint32_t RandomServerIDByNameTypeEx(const std::string& name, const uint32_t type);

		// 通过服务器名，随即获取一个服务器ID
		virtual uint32_t RandomServerIDByName(const std::string& name);

		// 获取网络类型
		virtual uint32_t GetNetTypeByID(const uint32_t serverID);

		// 判断两个服务器是否相同网络类型
		virtual bool IsSameNetType(const uint32_t srvId1, const uint32_t srvId2);

	public:
		virtual void onRefreshSV();

		virtual void onRecoverRefreshSV();

		virtual void onAddSV(const sdaemon::FavorStrSV & fs);		

		virtual void onServerRemovedSV(const sdaemon::FavorStrSV & fs);

		virtual void onServerStatusChangeSV(const sdaemon::FavorStrSV & fs);

	protected:
		CImServersContainer m_srvContainer; 
	};
}


#endif

