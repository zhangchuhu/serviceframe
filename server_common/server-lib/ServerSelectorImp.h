#pragma once 

#include "common/packet.h"
#include "common/watch_helper.h"
#include "core/sox/logger.h"
#include "server_common/server-lib/iclient.h"

#include "IServerSelector.h"
#include <stdio.h>

namespace server
{
	//调试帮助类

	class CDebugHelper
	{
	public:
		CDebugHelper() {};

		//vector<uint32_t>打印

		static std::string SetIntToString(const std::set<uint32_t> &setInt)
		{
			std::stringstream ssDebug;
			for (std::set<uint32_t>::const_iterator iterInt = setInt.begin(); iterInt != setInt.end(); iterInt ++)
			{
				if (!ssDebug.str().empty())
					ssDebug << ",";
				ssDebug << *iterInt;
			}
			return ssDebug.str();
		}

		//IP地址转换为字符串

		static std::string SVToIP(const sdaemon::FavorStrSV & fs)
		{
			std::string strIp = "";

			if (fs.dip != 0)
			{
				unsigned char *pIp = (unsigned char*)&fs.dip;
				char szTmp[32] = {0};
				snprintf(szTmp, sizeof(szTmp), "dip:%u.%u.%u.%u ", pIp[0], pIp[1], pIp[2], pIp[3]);
				strIp += szTmp;
			}
			if (fs.wip != 0)
			{
				unsigned char *pIp = (unsigned char*)&fs.wip;
				char szTmp[32] = {0};
				snprintf(szTmp, sizeof(szTmp), "wip:%u.%u.%u.%u ", pIp[0], pIp[1], pIp[2], pIp[3]);
				strIp += szTmp;
			}

			return strIp;
		}

		static std::string IPToString(uint32_t uIp)
		{
			unsigned char *pIp = (unsigned char*)&uIp;

			char szTmp[32] = {0};
			snprintf(szTmp, sizeof(szTmp), "ip:%u.%u.%u.%u ", pIp[0], pIp[1], pIp[2], pIp[3]);
			return std::string(szTmp);
		}
	};

	//服务器结构定义

	struct SServerInfo
	{
		std::string strSuffix;;
		uint32_t uServerId;
		uint32_t uGroupId;
		uint32_t dIp;
		uint32_t wIp;

		SServerInfo() {};
		SServerInfo(const std::string &_strSuffix, uint32_t _uServerId, uint32_t _uGroupId, uint32_t _dIp, uint32_t _wIp)
		{
			strSuffix = _strSuffix;
			uServerId = _uServerId;
			uGroupId = _uGroupId;
			dIp = _dIp;
			wIp = _wIp;
		}

		static uint32_t TestNetType(const SServerInfo &stServerInfo)
		{
			if (stServerInfo.dIp != 0 && stServerInfo.wIp == 0)
				return NET_TYPE_CTL;
			else if (stServerInfo.dIp == 0 && stServerInfo.wIp != 0)
				return NET_TYPE_CNC;
			else if (stServerInfo.dIp != 0 && stServerInfo.wIp != 0)
				return NET_TYPE_CTL|NET_TYPE_CNC;

			return NET_TYPE_NONE;
		}

		static uint32_t TestSrvIp(const SServerInfo &stServerInfo)
		{
			if (stServerInfo.dIp != 0)
				return stServerInfo.dIp;
			else if (stServerInfo.wIp != 0)
				return stServerInfo.wIp;

			return -1;
		}
	};

	//服务器结构容器类

	class CServerInfoContainer : public virtual IServerContainer
	{
	protected:
		//服务器容器 map<Suffix, map<ServerId, SServerInfo> >
		std::map<std::string, std::map<uint32_t, SServerInfo> > m_mapServerInfo;
	public:

		//---------------------------------------------------------------------------------------------------------
		//本节为容器管理方法

		virtual bool AddServer(const sdaemon::FavorStrSV & fs);
		virtual void RemoveServer(const sdaemon::FavorStrSV & fs);
		virtual void ClearServers();
		
		virtual void GetServersInfo(const std::string &strSuffix, std::map<uint32_t, SServerInfo> &mapSuffixInfo);

		//---------------------------------------------------------------------------------------------------------
		//本节为获取指定的服务器ID集合
		
		virtual std::set<uint32_t> GetServerIps(const std::string &strSuffix);

		virtual std::set<uint32_t> GetServerIds(const std::string &strSuffix);
		virtual std::set<uint32_t> GetServerIdsByNet(const std::string &strSuffix, uint32_t uNetType = NET_TYPE_ANY);
		virtual std::set<uint32_t> GetServerIdsByGroup(const std::string &strSuffix, uint32_t uGroupId);
		virtual std::set<uint32_t> GetServerIdsByNetGroup(const std::string &strSuffix, uint32_t uNetType, uint32_t uGroupId);
		virtual std::set<uint32_t> GetServerIdsByIp(const std::string &strSuffix, uint32_t uIp);

		//---------------------------------------------------------------------------------------------------------
		//本节提供IP-ID的转换
		//当以IP做规则时，应保证每种类型进程每台机器只启动一个

		virtual uint32_t ServerIpToId(const std::string &strSuffix, uint32_t uServerIp, std::set<uint32_t> &setServerIds);
		virtual uint32_t ServerIdToIp(const std::string &strSuffix, uint32_t uServerId);
	};

	//注册的服务器类型容器

	class CSuffixContainer
	{
	protected:

		struct SHashInfo
		{
			EHashServerValueType emValueType;
			EHashFuntcionType emFuncType;

			SHashInfo(EHashServerValueType _emValueType, EHashFuntcionType _emFuncType)
			{
				emValueType = _emValueType;
				emFuncType = _emFuncType;
			}
		};
		
		std::map<std::string, SHashInfo> m_mapSuffix;
		typedef std::map<std::string, SHashInfo> MAP_SUFFIX;

	public:

		void AddSuffix(const std::string &strSuffix, EHashServerValueType emValueType = ENUM_HASH_SERVERID, EHashFuntcionType emFuncType = ENUM_FUNC_MD5)
		{
			if ( m_mapSuffix.find(strSuffix) == m_mapSuffix.end() )
				m_mapSuffix.insert( std::make_pair(strSuffix, SHashInfo(emValueType, emFuncType)) );
		}

		void RemoveSuffix(const std::string &strSuffix)
		{
			m_mapSuffix.erase(strSuffix);
		}

		void ClearSuffix()
		{
			m_mapSuffix.clear();
		}

		bool FindSuffix(const std::string &strSuffix, EHashServerValueType &emValueType, EHashFuntcionType &emFuncType)
		{
			MAP_SUFFIX::iterator iterSuffix = m_mapSuffix.find(strSuffix);
			if (iterSuffix != m_mapSuffix.end())
			{
				emValueType = iterSuffix->second.emValueType;
				emFuncType = iterSuffix->second.emFuncType;
				return true;
			}
			return false;
		}
	};

	//节点小哈希环

	class CHashNode
	{
	protected:
		//map<HashValue, ServerValue>
		std::map<uint32_t, uint32_t> m_mapServerCirle;
		typedef std::map<uint32_t, uint32_t> MAP_CIRCLE;

	public:
		//构造时生成小哈希环
		CHashNode(uint32_t uServerValue, EHashFuntcionType emFuncType = ENUM_FUNC_MD5)
		{
			CConsistentHash Hash(m_mapServerCirle, emFuncType);
			Hash.AddServer(uServerValue);
		}

		//将本节点的小哈希环导出到外部大哈希环中
		void ExportCircle(std::map<uint32_t, uint32_t> &mapBigCircle)
		{
			for (MAP_CIRCLE::iterator iter = m_mapServerCirle.begin(); iter != m_mapServerCirle.end(); iter ++)
			{
				mapBigCircle.insert ( *iter );
			}
		}
	};

	//节点小哈希环容器

	class CHashContainer
	{
	protected:
		//map<ServerId, CHashNode>
		std::map<uint32_t, CHashNode> m_mapHashNode;
		typedef std::map<uint32_t, CHashNode> MAP_HASHNODE;

	public:
		void AddServer(uint32_t uServerId, uint32_t uServerValue, EHashFuntcionType emFuncType = ENUM_FUNC_MD5)
		{
			m_mapHashNode.insert( std::make_pair(uServerId, CHashNode(uServerValue, emFuncType)) );
		}
		void RemoveServer(uint32_t uServerId)
		{
			m_mapHashNode.erase( uServerId );
		}
		void ClearServers()
		{
			m_mapHashNode.clear();
		}

		//提供ID的集合，返回一个大的circle哈希环
		void GetServerCircle(const std::set<uint32_t> &setServerId, std::map<uint32_t, uint32_t> &mapServerCirle)
		{
			mapServerCirle.clear();

			for (std::set<uint32_t>::const_iterator iterServerId = setServerId.begin(); iterServerId != setServerId.end(); iterServerId ++)
			{
				MAP_HASHNODE::iterator iterNode = m_mapHashNode.find( *iterServerId );
				if (iterNode != m_mapHashNode.end())
				{
					iterNode->second.ExportCircle(mapServerCirle);
				}
			}
		}
	};

	//哈希选择器缓存代理

	class CHashCacheSelector
	{
	protected:
		CHashContainer &m_HashContainer;
		CHashContainer &m_HashIpContainer;

		std::map<std::string, std::map<uint32_t, uint32_t> > m_mapCircleCache;
		
		typedef std::map<std::string, std::map<uint32_t, uint32_t> > MAP_CIRCLECACHE;
		typedef std::map<uint32_t, uint32_t> MAP_CIRCLE;

	protected:
		inline std::string ToKeyString(const std::string &_strSuffix, uint32_t _uNetType, uint32_t _uGroupId)
		{
			std::stringstream ssKey;
			ssKey << _strSuffix << ":" << _uNetType << ":" << _uGroupId;
			return ssKey.str();
		}
		inline std::string GetSuffixFromKey(const std::string &_strKey)
		{
			size_t nPos = _strKey.find_first_of(":");
			return _strKey.substr(0, nPos);
		}

	public:
		CHashCacheSelector(CHashContainer &HashContiner, CHashContainer &HashIpContiner) : 
		m_HashContainer(HashContiner), m_HashIpContainer(HashIpContiner) {};

		//指定服务器类型发生改变时，清除缓存的哈希环
		void FreshHashCache(const std::string &strSuffix)
		{
			MAP_CIRCLECACHE::iterator iterCache = m_mapCircleCache.begin();
			for ( ; iterCache != m_mapCircleCache.end(); )
			{
				//KEY的格式 Suffix:NetType:GroupId
				std::string strKey = iterCache->first;
				std::string theSuffix = GetSuffixFromKey(strKey);

				if (theSuffix == strSuffix)
				{
					log(Info, "[CHashCacheSelector::FreshHashCache] m_mapCircleCache.earse key:%s", strKey.c_str());

					m_mapCircleCache.erase(iterCache ++);
					continue;
				}
				iterCache ++;
			}
		}

		//指定选择器类型，进行哈希计算，并返回结果
		uint32_t HashValueFromCache(const std::string &strSuffix, uint32_t uNetType, uint32_t uGroupId, 
			uint32_t uKey, EHashFuntcionType emFuncType, bool &bGet)
		{
			std::string strKey = ToKeyString(strSuffix, uNetType, uGroupId);

			MAP_CIRCLECACHE::iterator iterCache = m_mapCircleCache.find(strKey);
			if ( iterCache != m_mapCircleCache.end())
			{
				bGet = true;
				std::map<uint32_t, uint32_t> &mapCircleCache = iterCache->second;

				CConsistentHash Hash(mapCircleCache, emFuncType);
				return Hash.GetValueByKey(uKey);
			}

			bGet = false;
			return -1;
		}

		//指定选择器类型，进行哈希计算，并返回结果
		bool HashMultiValueFromCache(const std::string &strSuffix, uint32_t uNetType, uint32_t uGroupId, 
			uint32_t uKey, uint32_t uValueCount, std::set<uint32_t>& setValues, EHashFuntcionType emFuncType, bool &bGet)
		{
			std::string strKey = ToKeyString(strSuffix, uNetType, uGroupId);

			MAP_CIRCLECACHE::iterator iterCache = m_mapCircleCache.find(strKey);
			if ( iterCache != m_mapCircleCache.end())
			{
				bGet = true;
				std::map<uint32_t, uint32_t> &mapCircleCache = iterCache->second;

				CConsistentHash Hash(mapCircleCache, emFuncType);
				return Hash.GetMultiValueByKey(uKey, uValueCount, setValues);

			}

			bGet = false;
			return true;
		}

		//指定选择器类型，进行哈希计算，并返回结果
		uint32_t HashValueFromContainer(const std::string &strSuffix, uint32_t uNetType, uint32_t uGroupId, 
			const std::set<uint32_t> &setServerId, uint32_t uKey, EHashFuntcionType emFuncType = ENUM_FUNC_MD5)
		{
			std::string strKey = ToKeyString(strSuffix, uNetType, uGroupId);

			MAP_CIRCLECACHE::iterator iterCache = m_mapCircleCache.find(strKey);
			if ( iterCache != m_mapCircleCache.end())
			{
				MAP_CIRCLE &mapCircleCache = iterCache->second;

				CConsistentHash Hash(mapCircleCache, emFuncType);
				return Hash.GetValueByKey(uKey);
			}

			//缓存不存在，从大哈希环容器中拿
			MAP_CIRCLE mapCircleCache;
			m_HashContainer.GetServerCircle(setServerId, mapCircleCache);

			log(Info, "[CHashCacheSelector::HashValueFromContainer] m_HashContainer.GetServerCircle(<%s>)", CDebugHelper::SetIntToString(setServerId).c_str());

			//将拿到的大哈希环写进缓存
			m_mapCircleCache.insert( std::make_pair(strKey, mapCircleCache) );

			//返回计算结果
			CConsistentHash Hash(mapCircleCache, emFuncType);
			return Hash.GetValueByKey(uKey);
		}

		
		//指定选择器类型，进行哈希计算，并返回结果
		bool HashMultiValueFromContainer(const std::string &strSuffix, uint32_t uNetType, uint32_t uGroupId, 
			const std::set<uint32_t> &setServerId, uint32_t uKey, uint32_t uValueCount, std::set<uint32_t>& setValues, 
			EHashServerValueType emValueType, EHashFuntcionType emFuncType = ENUM_FUNC_MD5)
		{
			std::string strKey = ToKeyString(strSuffix, uNetType, uGroupId);

			MAP_CIRCLECACHE::iterator iterCache = m_mapCircleCache.find(strKey);
			if ( iterCache != m_mapCircleCache.end())
			{
				MAP_CIRCLE &mapCircleCache = iterCache->second;

				CConsistentHash Hash(mapCircleCache, emFuncType);
				return Hash.GetMultiValueByKey(uKey, uValueCount, setValues);
			}

			//缓存不存在，从大哈希环容器中拿
			MAP_CIRCLE mapCircleCache;
			if (ENUM_HASH_SERVERIPID == emValueType)
			{
				m_HashIpContainer.GetServerCircle(setServerId, mapCircleCache);
			}
			else
			{
				m_HashContainer.GetServerCircle(setServerId, mapCircleCache);
			}

			log(Info, "[CHashCacheSelector::HashMultiValueFromContainer] m_HashContainer.GetServerCircle(<%s>)", CDebugHelper::SetIntToString(setServerId).c_str());

			//将拿到的大哈希环写进缓存
			m_mapCircleCache.insert( std::make_pair(strKey, mapCircleCache) );

			//返回计算结果
			CConsistentHash Hash(mapCircleCache, emFuncType);
			return Hash.GetMultiValueByKey(uKey, uValueCount, setValues);
		}
	};


	class CServerSelectorImp : 
		public core::IDaemonClientAwareW,
		public virtual CServerInfoContainer,
		public IServerSelector,
		public WatchHelper<IServerSelctorWatcherBase, IServerSelectorWatcher> //借用这个模板类实现了watch,和revoke，forEachWatcherRef1,2,...的函数
	{
	protected:
		CSuffixContainer m_SuffixContainer;
		CHashContainer m_HashContainer;
		CHashContainer m_HashIpContainer;
		CHashCacheSelector m_HashCacheSelector;
		//key:ip, value:serverids
		std::map<uint32_t, std::set<uint32_t> > m_mapIpServerIds;

		bool m_bRefreshSV;

	public:

		CServerSelectorImp() : m_HashCacheSelector(m_HashContainer, m_HashIpContainer), m_bRefreshSV(false) {};


		//--------------------------------------------------------------------------------------------------------------------------
		//本节为注册指定类型的服务器

		virtual void RegisterServerSuffix(const std::string &strSuffix, EHashServerValueType emValueType = ENUM_HASH_SERVERID, EHashFuntcionType emFuncType = ENUM_FUNC_MD5);

		//--------------------------------------------------------------------------------------------------------------------------
		//本节为关于服务器调整的相关虚接口

		virtual void onRefreshSV();
		virtual void onAddSV(const sdaemon::FavorStrSV & fs);
		virtual void onServerRemovedSV(const sdaemon::FavorStrSV & fs);
		virtual void onRecoverRefreshSV();
		virtual void onServerStatusChangeSV(const sdaemon::FavorStrSV & fs);

		//--------------------------------------------------------------------------------------------------------------------------
		//本节为随机选择一个需要的服务器

		virtual uint32_t RandomServer(const std::string &strSuffix);
		virtual uint32_t RandomServerByNet(const std::string &strSuffix, uint32_t uNetType = NET_TYPE_ANY);
		virtual uint32_t RandomServerByGroup(const std::string &strSuffix, uint32_t uGroupId);
		virtual uint32_t RandomServerByNetGroup(const std::string &strSuffix, uint32_t uNetType, uint32_t uGroupId);

		//--------------------------------------------------------------------------------------------------------------------------
		//本李为哈希选择一个需要的服务器

		virtual uint32_t HashServer(uint32_t uKey, const std::string &strSuffix);		
		virtual bool HashMultiServer(uint32_t uKey, uint32_t uValueCount, std::vector<uint32_t>& vecValues, const std::string &strSuffix);
		virtual uint32_t HashServerByNet(uint32_t uKey, const std::string &strSuffix, uint32_t uNetType = NET_TYPE_ANY);
		virtual uint32_t HashServerByGroup(uint32_t uKey, const std::string &strSuffix, uint32_t uGroupId);
		virtual uint32_t HashServerByNetGroup(uint32_t uKey, const std::string &strSuffix, uint32_t uNetType, uint32_t uGroupId);
		virtual uint32_t HashServerByIp(uint32_t uKey, const std::string &strSuffix, uint32_t uIp);
		//--------------------------------------------------------------------------------------------------------------------------
	};

}
