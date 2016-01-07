#pragma once

#include <iostream>
#include <algorithm>
#include <string.h>
#include <vector>
#include <map>
#include <set>

#include "server_common/server-lib/id_def.h" //define SUFFIX_XXX
#include "server_common/server-lib/iclient.h" //define IDaemonClientAwareW
#include "server_common/server-lib/RouteTarget.h" //define IRouteTarget
#include "common/packet.h"
#include "core/sox/logger.h"

#include "ConsistentHashing.h"

//#include "server_common/protocol/PGTransfer.h"

namespace server
{
	namespace im
	{

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//通用服务器哈希帮助类

		class CHashServerValueHelper
		{
		public:

			virtual ~CHashServerValueHelper() {};

			virtual bool AddServer(uint32_t uServerValue);

			virtual bool RemoveServer(uint32_t uServerValue);

			virtual void ClearServer();

			//根据因子，从集合中挑出一个Value值，如果集合为空,返回 NONE_SERVER_ID

			virtual uint32_t GetServerValueByFactor(uint32_t uFactor);

			//检查服务器Value在集合中是否存在

			virtual bool IsServerValueExist(uint32_t uServerValue);

			//取全部服务器ID集合

			virtual void GetServerValueSet(std::set<uint32_t> &setServerValue);

		private:
			UserConsistentHash m_Hash;
			std::set<uint32_t> m_setServerValue;
		};

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//服务器哈希类型

		enum EHashServerValueType
		{
			ENUM_HASH_SERVERID = 0, //以serverId做哈希结果值
			ENUM_HASH_SERVERIP //以ServerIp做哈希结果值
		};

		//通用的服务器哈希帮助类

		template <typename THashServerValueHelper>
		class CTBaseDaemonClientW : public core::IDaemonClientAwareW
		{
			typedef typename std::map<std::string, THashServerValueHelper*>::iterator MAP_HASHSERVERS_ITER;

			std::map<std::string, THashServerValueHelper*> m_mapHashHelper; //服务器类型 - 哈希帮助对象
			std::map<std::string, EHashServerValueType> m_mapHashType; //服务器类型 - 哈希结果值类型
			std::map<uint32_t, sdaemon::FavorStrSV> m_mapServers; //维护关心的服务器列表

		protected:
			//------------------------------------------------------------------------------------------------
			//添加服务器类型哈希

			void AddServerName(std::string strServerName, EHashServerValueType emHashType = ENUM_HASH_SERVERID)
			{
				THashServerValueHelper *pNew = new THashServerValueHelper;
				if (pNew)
					m_mapHashHelper[strServerName] = pNew;

				m_mapHashType[strServerName] = emHashType;

				log(Info, "[CTBaseDaemonClientW::AddServerName] Register ServerName[%s]", strServerName.c_str());
			}

		public:
			virtual ~CTBaseDaemonClientW()
			{
				MAP_HASHSERVERS_ITER iterHashHelper = m_mapHashHelper.begin();
				for ( ; iterHashHelper != m_mapHashHelper.end(); iterHashHelper ++)
				{
					delete iterHashHelper->second;
					iterHashHelper->second = NULL;
				}
				m_mapHashHelper.clear();
			}

			//获取指定服务器类型哈希对象

			THashServerValueHelper *GetHashHelper(std::string strServerName)
			{
				MAP_HASHSERVERS_ITER iterHashHelper = m_mapHashHelper.find(strServerName);
				if (iterHashHelper != m_mapHashHelper.end())
					return iterHashHelper->second;

				log(Info, "[CTBaseDaemonClientW::GetHashHelper] ServerName[%s] return NULL, (just look here)", strServerName.c_str());
				return NULL;
			}

			//------------------------------------------------------------------------------------------------

			virtual void onRefreshSV()
			{
				ClearMapServers();

				//清除所有哈希服务器
				MAP_HASHSERVERS_ITER iterHashHelper = m_mapHashHelper.begin();
				for ( ; iterHashHelper != m_mapHashHelper.end(); iterHashHelper ++)
				{
					iterHashHelper->second->ClearServer();
				}

				std::vector<sdaemon::FavorStrSV> ff = client->getServersSV();

				for (std::vector<sdaemon::FavorStrSV>::iterator it = ff.begin(); it != ff.end(); it ++)
				{
					MAP_HASHSERVERS_ITER iterHashHelper = m_mapHashHelper.find(it->sstr);
					if ( (iterHashHelper != m_mapHashHelper.end()) &&
						(it->serverStatus == sdaemon::RUNNING) )
					{
						uint32_t uServerValue = GetServerValue(*it); //找不到注册类型，就返回-1
						if (uServerValue != (uint32_t)(-1))
							iterHashHelper->second->AddServer(uServerValue);
						else
							log(Info, "[CTBaseDaemonClientW::onRefreshSV] GetServerValue(%s) return -1, (just look here)", it->sstr.c_str());

						AddMapServer(*it);
					}

					//打印服务器启停记录
					if (it->serverStatus == sdaemon::RUNNING)
					{
						log(Info, "[CWatchServerDaemonClient::onRefreshSV] ServerId[%d] ServerName[%s] ServerIp[%s]", 
							it->serverId, it->sstr.c_str(), __SVToIP(*it).c_str());
					}
				}

				OnUserInit(); //初使化调整
			}

			virtual void onAddSV(const sdaemon::FavorStrSV & fs)
			{
				MAP_HASHSERVERS_ITER iterHashHelper = m_mapHashHelper.find(fs.sstr);
				if ( (iterHashHelper != m_mapHashHelper.end()) &&
					(fs.serverStatus == sdaemon::RUNNING) )
				{
					uint32_t uServerValue = GetServerValue(fs);
					if (uServerValue != (uint32_t)(-1))
						iterHashHelper->second->AddServer(uServerValue);
					else
						log(Info, "[CTBaseDaemonClientW::onAddSV] GetServerValue(%s) return -1, (just look here)", fs.sstr.c_str());

					AddMapServer(fs);

					OnUserAddjust(fs.sstr); //服务器调整时，调用派生类的处理
				}

				//打印服务器启停记录
				log(Info, "[CWatchServerDaemonClient::onAddSV] AddServer ServerId[%d] ServerName[%s] ServerIp[%s]", 
					fs.serverId, fs.sstr.c_str(), __SVToIP(fs).c_str());
			}

			virtual void onServerRemovedSV(const sdaemon::FavorStrSV & fs)
			{
				MAP_HASHSERVERS_ITER iterHashHelper = m_mapHashHelper.find(fs.sstr);
				if ( iterHashHelper != m_mapHashHelper.end() )
				{
					uint32_t uServerValue = GetServerValue(fs);

					CHashServerValueHelper *pHashHelper = iterHashHelper->second;
					if ( pHashHelper->IsServerValueExist(uServerValue) )
					{
						pHashHelper->RemoveServer(uServerValue); //确保-1也被删除
					}

					RemoveMapServer(fs);

					OnUserAddjust(fs.sstr); //服务器调整时，调用派生类的处理
				}

				//打印服务器启停记录
				log(Info, "[CWatchServerDaemonClient::onServerRemovedSV] RemoveServer ServerId[%d] ServerName[%s] ServerIp[%s]", 
					fs.serverId, fs.sstr.c_str(), __SVToIP(fs).c_str());
			}

			virtual void onRecoverRefreshSV()
			{
				onRefreshSV();
			}

			virtual void onServerStatusChangeSV(const sdaemon::FavorStrSV & fs)
			{
				onAddSV(fs);
			}

			virtual void OnUserInit() {} //onRefreshSV时调用
			virtual void OnUserAddjust(const std::string strServerName) {} //onAddSV onServerRemovedSV时调用

		protected:

			//------------------------------------------------------------------------------------------------

			virtual void ClearMapServers()
			{
				m_mapServers.clear();
			}

			virtual void AddMapServer(const sdaemon::FavorStrSV & fs)
			{
				m_mapServers.insert( std::make_pair(fs.serverId, fs) );
			}

			virtual void RemoveMapServer(const sdaemon::FavorStrSV & fs)
			{
				m_mapServers.erase(fs.serverId);
			}

			//根据服务器VALUE值 -> 服务器ID （无论VALUE值是 IP 还是 ID）
			//但必须指定VALUE类型值，否则无法在服务器中检索
			virtual uint32_t GetServerIdByValue(uint32_t uServerValue, const std::string &strServerName, EHashServerValueType emHashType = ENUM_HASH_SERVERID)
			{
				if (ENUM_HASH_SERVERID == emHashType)
				{
					return uServerValue;
				}

				else if (ENUM_HASH_SERVERIP == emHashType)
				{
					std::map<uint32_t, sdaemon::FavorStrSV>::iterator iterServer = m_mapServers.begin();
					for ( ; iterServer != m_mapServers.end(); iterServer ++)
					{
						sdaemon::FavorStrSV &fs = iterServer->second;
						if ( fs.sstr == strServerName &&
							 (fs.dip == uServerValue || fs.wip == uServerValue) )
							return fs.serverId;
					}
				}

				return -1;
			}

			//通过服务器数据结构返回所关心的服务器HASH结果值
			//调用本函数时，必须保证加入了服务器类型

			inline uint32_t GetServerValue(const sdaemon::FavorStrSV &fs)
			{
				EHashServerValueType emValueType = m_mapHashType[fs.sstr];

				if (ENUM_HASH_SERVERIP == emValueType)
				{
					if (fs.dip != 0)
						return fs.dip;
					else
						return fs.wip;
				}

				return fs.serverId;
			}

		private:
			//IP地址转换为字符串

			inline std::string __SVToIP(const sdaemon::FavorStrSV & fs)
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
		};


		typedef CTBaseDaemonClientW<CHashServerValueHelper>  CBaseDaemonClientW;


		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//gtransfer服务器哈希帮助类
		//供其它服务器给gtransfer发包使用

		//template <typename TBaseDaemonClientW>
		//class CTGTransDaemonClientW : public TBaseDaemonClientW, //may CBaseDaemonClientW,CRBaseDaemonClientW
		//									 public core::IRouteTarget
		//{
		//public:
		//	CTGTransDaemonClientW()
		//	{
		//		//注册gtrans服务器哈希，并置结果集合为IP
		//		TBaseDaemonClientW::AddServerName(SUFFIX_GTRANS, ENUM_HASH_SERVERIP);
		//	}

		//	//功能：将包直接发给hash(gid) -> gtransfer
		//	//参数：uGid为群/组ID
		//	bool RouteGT(uint32_t uGid, uint32_t Uri, const sox::Marshallable &obj)
		//	{
		//		CHashServerValueHelper *pHashHelper = TBaseDaemonClientW::GetHashHelper(SUFFIX_GTRANS);
		//		if (pHashHelper == NULL)
		//			return false;

		//		//根据因子计算服务器结果值，这里为IP
		//		uint32_t uServerValue = pHashHelper->GetServerValueByFactor(uGid);

		//		//根据服务器结果值计算出serverId
		//		uint32_t uServerId = TBaseDaemonClientW::GetServerIdByValue(uServerValue, SUFFIX_GTRANS, ENUM_HASH_SERVERIP);

		//		//发送到计算出的gtransferId
		//		if (uServerId != (uint32_t)(-1))
		//			routeS(uServerId, Uri, obj);
		//		return true;
		//	}

		//	bool RouteGT(std::string &strFrom, uint32_t uGid, uint32_t Uri, const std::string &strPacket)
		//	{
		//		CHashServerValueHelper *pHashHelper = TBaseDaemonClientW::GetHashHelper(SUFFIX_GTRANS);
		//		if (pHashHelper == NULL)
		//			return false;

		//		//根据因子计算服务器结果值，这里为IP
		//		uint32_t uServerValue = pHashHelper->GetServerValueByFactor(uGid);

		//		//根据服务器结果值计算出serverId
		//		uint32_t uServerId = TBaseDaemonClientW::GetServerIdByValue(uServerValue, SUFFIX_GTRANS, ENUM_HASH_SERVERIP);

		//		//发送到计算出的gtransferId
		//		if (uServerId != (uint32_t)(-1))
		//			routeS(strFrom, uServerId, Uri, strPacket);

		//		return true;
		//	}

		//	//功能：将广播包发给hash(gid) -> gtransfer
		//	//参数：uGid为群/组ID
		//	bool GBroadcast(uint32_t uSenderId, uint32_t uGid, protocol::gtransfer::EOfflineMsgMode emMode,
		//		uint32_t Uri, const sox::Marshallable &obj)
		//	{
		//		server::gtransfer::PSS_GBroadcast objBroadcast(Uri, obj);
		//		objBroadcast.m_stGBoardcast.SetSendId(uSenderId);
		//		objBroadcast.m_stGBoardcast.SetOfflineMode(emMode);
		//		objBroadcast.m_stGBoardcast.SetGid(uGid); //fixed by liny

		//		log(Info, "[%s] (GID=%d)", __FUNCTION__, uGid);

		//		return RouteGT(uGid, objBroadcast.uri, objBroadcast);
		//	}

		//	//功能：将转发包发给hash(gid) -> gtransfer
		//	//参数：uGid为群/组ID，如果uGid=-1，则随机选择一个gtransfer
		//	bool GForward(uint32_t uSenderId, uint32_t uGid, const std::vector<uint32_t> &vecRecvUid, protocol::gtransfer::EOfflineMsgMode emMode,
		//		uint32_t Uri, const sox::Marshallable &obj)
		//	{
		//		server::gtransfer::PSS_GForward objForward(Uri, obj);
		//		objForward.m_stGForward.SetSendId(uSenderId);
		//		objForward.m_stGForward.SetRecvUidVect(vecRecvUid);
		//		objForward.m_stGForward.SetOfflineMode(emMode);

		//		log(Info, "[%s] (GID=%d)", __FUNCTION__, uGid);

		//		if (uGid == (uint32_t)(-1))
		//		{
		//			route(SUFFIX_GTRANS, objForward.uri, objForward);
		//			return true;
		//		}

		//		return RouteGT(uGid, objForward.uri, objForward);
		//	}

		//	//功能：将转发包发给hash(gid) -> gtransfer
		//	//参数：uGid为群/组ID，如果uGid=-1，则随机选择一个gtransfer
		//	bool GForward2(uint32_t uSenderId, uint32_t uGid, const std::vector<uint32_t> &vecRecvUid1, const std::vector<uint32_t> &vecRecvUid2,
		//		protocol::gtransfer::EOfflineMsgMode emMode, uint32_t Uri, const sox::Marshallable &obj)
		//	{
		//		server::gtransfer::PSS_GForward2 objForward2(Uri, obj);
		//		objForward2.m_stGForward2.SetSendId(uSenderId);
		//		objForward2.m_stGForward2.SetRecvUidVect1(vecRecvUid1);
		//		objForward2.m_stGForward2.SetRecvUidVect2(vecRecvUid2);
		//		objForward2.m_stGForward2.SetOfflineMode(emMode);

		//		log(Info, "[%s] (GID=%d)", __FUNCTION__, uGid);

		//		if (uGid == (uint32_t)(-1))
		//		{
		//			route(SUFFIX_GTRANS, objForward2.uri, objForward2);
		//			return true;
		//		}

		//		return RouteGT(uGid, objForward2.uri, objForward2);
		//	}
		//};

		//typedef CTGTransDaemonClientW<CBaseDaemonClientW>  CGTransDaemonClientW;

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}
