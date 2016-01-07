#pragma once
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <string.h>
#include <vector>
#include <map>
#include <set>
#include "common/core/iserver.h" //define IDaemonServerAwareW

#include "server_common/server-lib/id_def.h" //define SUFFIX_XXX
#include "server_common/server-lib/iclient.h" //define IDaemonClientAwareW
#include "server_common/server-lib/RouteTarget.h" //define IRouteTarget
#include "common/packet.h"
#include "core/sox/logger.h"
#include <boost/thread/thread.hpp>
#include "server_common/protocol/PGTransferSrv.h"

#include "ConsistentFnvHash.h"
#include "DaemonClient.h"
#include "ConsistentHash.h"
#include "ServerSelectorImp.h"
#include "MulServerSelectorImp.h"
#include "IServerSelector.h"

using namespace server;

namespace server
{
	namespace grp
	{

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//通用服务器哈希帮助类

		class CHashServerValueHelper2
		{
		public:

			virtual ~CHashServerValueHelper2() {};

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
                UserConsistentFnvHash m_Hash;

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
		class CTBaseDaemonClientW2 : public core::IDaemonClientAwareW
		{
			typedef typename std::map<std::string, THashServerValueHelper*>::iterator MAP_HASHSERVERS_ITER;

			std::map<std::string, THashServerValueHelper*> m_mapHashHelper; //服务器类型 - 哈希帮助对象
			std::map<std::string, EHashServerValueType> m_mapHashType; //服务器类型 - 哈希结果值类型
			std::map<uint32_t, sdaemon::FavorStrSV> m_mapServers; //维护关心的服务器列表 serverId -> fs

		protected:
			//------------------------------------------------------------------------------------------------
			//添加服务器类型哈希

			void AddServerName(std::string strServerName, EHashServerValueType emHashType = ENUM_HASH_SERVERID)
			{
				if (m_mapHashHelper.find(strServerName) != m_mapHashHelper.end())
					return;

				THashServerValueHelper *pNew = new THashServerValueHelper;
				if (pNew)
					m_mapHashHelper[strServerName] = pNew;

				m_mapHashType[strServerName] = emHashType;

				log(Info, "[CTBaseDaemonClientW::AddServerName] Register ServerName[%s] ServerType[%u].",
					strServerName.c_str(), (uint32_t)emHashType);
			}

		public:
			virtual ~CTBaseDaemonClientW2()
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

				log(Error, "[CTBaseDaemonClientW::GetHashHelper] ServerName[%s] return NULL!", strServerName.c_str());
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
							log(Warn, "[CTBaseDaemonClientW::onRefreshSV] GetServerValue(%s) return -1!", it->sstr.c_str());

						AddMapServer(*it);
					}

					//打印服务器启停记录
					if (sdaemon::RUNNING == it->serverStatus)
					{
						log(Info, "[CWatchServerDaemonClient::onRefreshSV] ServerId[%d] ServerName[%s] ServerIp[%s]",
							it->serverId, it->sstr.c_str(), __SVToIP(*it).c_str());
					}
				}

                /**
                 * Check if initialize the cache data.
                 **/
                uint32_t uTimeIntv;
                core::DaemonClient* pDaemonClient = (core::DaemonClient*)client;
                if(true == pDaemonClient->__IsDaemonConnTimeout(uTimeIntv))
                {
					log(Info, "[CWatchServerDaemonClient::%s](%us):Ask process to initialize the cache data.", __FUNCTION__, uTimeIntv);
				    OnUserInit(); //初始化调整
                }
				else
				{
					log(Info, "[CWatchServerDaemonClient::%s](%us):Not initialize cache data,since Daemon connection isn't timeout.", __FUNCTION__, uTimeIntv);
				}
				
				/*Set process indication as ready.*/
				pDaemonClient->__SetProcessReadyFlag(true);
				
			}

			virtual void onAddSV(const sdaemon::FavorStrSV & fs)
			{
				MAP_HASHSERVERS_ITER iterHashHelper = m_mapHashHelper.find(fs.sstr);
				if ( (iterHashHelper != m_mapHashHelper.end()) && (fs.serverStatus == sdaemon::RUNNING) )
				{
					bool bDuplicateAdd = IsReadyInMapServer(fs.serverId);
					uint32_t uServerValue = GetServerValue(fs);
					if (uServerValue != (uint32_t)(-1))
					{
						iterHashHelper->second->AddServer(uServerValue);
					}
					else
					{
						log(Info, "[CTBaseDaemonClientW::onAddSV] GetServerValue(%s) return -1, (just look here)", fs.sstr.c_str());
					}

					AddMapServer(fs);

                    if(!bDuplicateAdd)
                    {
						log(Info, "[CTBaseDaemonClientW::%s]Notify process to adjust hash,since add new process(%u:%s).", __FUNCTION__, fs.serverId, fs.sstr.c_str());
					    OnUserAddjust(fs.sstr); //服务器调整时，调用派生类的处理
                    }
					else
					{
						log(Warn, "[CTBaseDaemonClientW::%s]Not notify process to adjust hash,since process(%u:%s) duplicated.", __FUNCTION__, fs.serverId, fs.sstr.c_str());
					}
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

					CHashServerValueHelper2 *pHashHelper = iterHashHelper->second;
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

			/**---------------------------------------------------------------------------
			* Function	   : IsReadyInMapServer
			*
			* Description  : This routine to check if the serverId has been ready. 
			*
			* @param	   : uServerId - server id.
			*				
			* @return	   : true - ready/duplicated, false - new server id.
			*---------------------------------------------------------------------------*/
			virtual bool IsReadyInMapServer(const uint32_t uServerId)
			{
				return ((m_mapServers.find(uServerId) != m_mapServers.end()) ? true : false);
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
					return -1; // not found.
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

		typedef CTBaseDaemonClientW2<CHashServerValueHelper2>  CBaseDaemonClientW2;

        class CRBaseDaemonClientW2 : public CTBaseDaemonClientW2<CHashServerValueHelper2>
        {
            boost::mutex m_mutexServers;
        protected:
            virtual void ClearMapServers()
            {
                boost::mutex::scoped_lock sl(m_mutexServers);
                CTBaseDaemonClientW2<CHashServerValueHelper2>::ClearMapServers();
            }


            virtual void AddMapServer(const sdaemon::FavorStrSV & fs)
            {
                boost::mutex::scoped_lock sl(m_mutexServers);
                CTBaseDaemonClientW2<CHashServerValueHelper2>::AddMapServer(fs);
            }


            virtual void RemoveMapServer(const sdaemon::FavorStrSV & fs)
            {
                boost::mutex::scoped_lock sl(m_mutexServers);
                CTBaseDaemonClientW2<CHashServerValueHelper2>::RemoveMapServer(fs);
            }


            virtual uint32_t GetServerIdByValue(uint32_t uServerValue, const std::string &strServerName, EHashServerValueType emHashType)
            {
                boost::mutex::scoped_lock sl(m_mutexServers);
                return CTBaseDaemonClientW2<CHashServerValueHelper2>::GetServerIdByValue(uServerValue, strServerName, emHashType);
            }
        };

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//gtransfer服务器哈希帮助类
		//供其它服务器给gtransfer发包使用

		template <typename TBaseDaemonClientW>
		class CTGTransDaemonClientW : public TBaseDaemonClientW, //may CBaseDaemonClientW,CRBaseDaemonClientW
			virtual public core::IRouteTarget
		{
		public:
			CTGTransDaemonClientW()
			{
				//注册gtrans服务器哈希，并置结果集合为IP
				TBaseDaemonClientW::AddServerName(SUFFIX_GTRANS, ENUM_HASH_SERVERIP);
				//注册gchat服务器哈希，并置结果集合为IP
				TBaseDaemonClientW::AddServerName(SUFFIX_GCHAT, ENUM_HASH_SERVERIP);
				//注册gauth服务器哈希，并置结果集合为IP
				TBaseDaemonClientW::AddServerName(SUFFIX_GAUTH, ENUM_HASH_SERVERIP);
				//注册gauth服务器哈希，并置结果集合为IP
				TBaseDaemonClientW::AddServerName(SUFFIX_GMSGCACHE, ENUM_HASH_SERVERIP);
				//注册gcheck服务器哈希，并置结果集合为ID
				TBaseDaemonClientW::AddServerName(SUFFIX_GCHECK, ENUM_HASH_SERVERIP);
			}
			/*
			//功能：将包直接发给hash(gid) -> gtransfer
			//参数：uGid为群/组ID
			bool RouteGT(uint32_t uGid, uint32_t Uri, const sox::Marshallable &obj)
			{
				CHashServerValueHelper2 *pHashHelper = TBaseDaemonClientW::GetHashHelper(SUFFIX_GTRANS);
				if (pHashHelper == NULL)
					return false;

				//根据因子计算服务器结果值，这里为IP
				uint32_t uServerValue = pHashHelper->GetServerValueByFactor(uGid);

				//根据服务器结果值计算出serverId
				uint32_t uServerId = TBaseDaemonClientW::GetServerIdByValue(uServerValue, SUFFIX_GTRANS, ENUM_HASH_SERVERIP);

				//发送到计算出的gtransferId
				if (uServerId != (uint32_t)(-1))
				{
					routeS(uServerId, Uri, obj);
					return true;
				}
				else
				{
					return false;
				}
			}

			bool RouteGTReliable(uint32_t uGid, uint32_t Uri, const sox::Marshallable &obj)
			{
				CHashServerValueHelper2 *pHashHelper = TBaseDaemonClientW::GetHashHelper(SUFFIX_GTRANS);
				if (pHashHelper == NULL)
					return false;

				//根据因子计算服务器结果值，这里为IP
				uint32_t uServerValue = pHashHelper->GetServerValueByFactor(uGid);

				//根据服务器结果值计算出serverId
				uint32_t uServerId = TBaseDaemonClientW::GetServerIdByValue(uServerValue, SUFFIX_GTRANS, ENUM_HASH_SERVERIP);

				//发送到计算出的gtransferId
				if (uServerId != (uint32_t)(-1))
				{
					RouteReliable(uServerId, Uri, obj);
					return true;
				}
				else
				{
					return false;
				}
			}

			bool RouteGT(std::string &strFrom, uint32_t uGid, uint32_t Uri, const std::string &strPacket)
			{
				CHashServerValueHelper2 *pHashHelper = TBaseDaemonClientW::GetHashHelper(SUFFIX_GTRANS);
				if (pHashHelper == NULL)
					return false;

				//根据因子计算服务器结果值，这里为IP
				uint32_t uServerValue = pHashHelper->GetServerValueByFactor(uGid);

				//根据服务器结果值计算出serverId
				uint32_t uServerId = TBaseDaemonClientW::GetServerIdByValue(uServerValue, SUFFIX_GTRANS, ENUM_HASH_SERVERIP);

				//发送到计算出的gtransferId
				if (uServerId != (uint32_t)(-1))
				{
					routeS(strFrom, uServerId, Uri, strPacket);
					return true;
				}
				else
				{
					return false;
				}

			}

			//功能：将包直接发给hash(gid) -> gchat
			//参数：uGid为群ID
			bool RouteGC(uint32_t uGid, uint32_t Uri, const sox::Marshallable &obj)
			{
				CHashServerValueHelper2 *pHashHelper = TBaseDaemonClientW::GetHashHelper(SUFFIX_GCHAT);
				if (pHashHelper == NULL)
					return false;

				//根据因子计算服务器结果值，这里为IP
				uint32_t uServerValue = pHashHelper->GetServerValueByFactor(uGid);

				//根据服务器结果值计算出serverId
				uint32_t uServerId = TBaseDaemonClientW::GetServerIdByValue(uServerValue, SUFFIX_GCHAT, ENUM_HASH_SERVERIP);

				//发送到计算出的gchatId
				if (uServerId != (uint32_t)(-1))
				{
					routeS(uServerId, Uri, obj);
					return true;
				}
				else
				{
					return false;
				}
			}

			bool RouteGC(std::string &strFrom, uint32_t uGid, uint32_t Uri, const std::string &strPacket)
			{
				CHashServerValueHelper2 *pHashHelper = TBaseDaemonClientW::GetHashHelper(SUFFIX_GCHAT);
				if (pHashHelper == NULL)
					return false;

				//根据因子计算服务器结果值，这里为IP
				uint32_t uServerValue = pHashHelper->GetServerValueByFactor(uGid);

				//根据服务器结果值计算出serverId
				uint32_t uServerId = TBaseDaemonClientW::GetServerIdByValue(uServerValue, SUFFIX_GCHAT, ENUM_HASH_SERVERIP);

				//发送到计算出的gchatId
				if (uServerId != (uint32_t)(-1))
				{
					routeS(strFrom, uServerId, Uri, strPacket);
					return true;
				}
				else
				{
					return false;
				}
			}

			bool RouteGCReliable(uint32_t uGid, uint32_t Uri, const sox::Marshallable &obj)
			{
				CHashServerValueHelper2 *pHashHelper = TBaseDaemonClientW::GetHashHelper(SUFFIX_GCHAT);
				if (pHashHelper == NULL)
					return false;

				//根据因子计算服务器结果值，这里为IP
				uint32_t uServerValue = pHashHelper->GetServerValueByFactor(uGid);

				//根据服务器结果值计算出serverId
				uint32_t uServerId = TBaseDaemonClientW::GetServerIdByValue(uServerValue, SUFFIX_GCHAT, ENUM_HASH_SERVERIP);

				//发送到计算出的gchatId
				if (uServerId != (uint32_t)(-1))
				{
					RouteReliable(uServerId, Uri, obj);
					return true;
				}
				else
				{
					return false;
				}
			}

			//功能：将包直接发给hash(gid) -> gcheck
			//参数：uGid为群/组ID
			bool RouteGChkReliable(uint32_t uGid, uint32_t Uri, const sox::Marshallable &obj)
			{
				CHashServerValueHelper2 *pHashHelper = TBaseDaemonClientW::GetHashHelper(SUFFIX_GCHECK);
				if (pHashHelper == NULL)
					return false;

				//根据因子计算服务器结果值，这里为IP
				uint32_t uServerValue = pHashHelper->GetServerValueByFactor(uGid);

				//根据服务器结果值计算出serverId
				uint32_t uServerId = TBaseDaemonClientW::GetServerIdByValue(uServerValue, SUFFIX_GCHECK, ENUM_HASH_SERVERIP);

				//发送到计算出的gtransferId
				if (uServerId != (uint32_t)(-1))
				{
					RouteReliable(uServerId, Uri, obj);
					return true;
				}
				else
					return false;
			}

			//功能：将包直接发给hash(gid) -> gauth
			//参数：uGid为群/组ID
			bool RouteGA(uint32_t uGid, uint32_t Uri, const sox::Marshallable &obj)
			{
				CHashServerValueHelper2 *pHashHelper = TBaseDaemonClientW::GetHashHelper(SUFFIX_GAUTH);
				if (pHashHelper == NULL)
					return false;

				//根据因子计算服务器结果值，这里为IP
				uint32_t uServerValue = pHashHelper->GetServerValueByFactor(uGid);

				//根据服务器结果值计算出serverId
				uint32_t uServerId = TBaseDaemonClientW::GetServerIdByValue(uServerValue, SUFFIX_GAUTH, ENUM_HASH_SERVERIP);

				//发送到计算出的gauthid
				if (uServerId != (uint32_t)(-1))
				{
					routeS(uServerId, Uri, obj);
					return true;
				}
				else
				{
					return false;
				}
			}

			bool RouteGAReliable(uint32_t uGid, uint32_t Uri, const sox::Marshallable &obj)
			{
				CHashServerValueHelper2 *pHashHelper = TBaseDaemonClientW::GetHashHelper(SUFFIX_GAUTH);
				if (pHashHelper == NULL)
					return false;

				//根据因子计算服务器结果值，这里为IP
				uint32_t uServerValue = pHashHelper->GetServerValueByFactor(uGid);

				//根据服务器结果值计算出serverId
				uint32_t uServerId = TBaseDaemonClientW::GetServerIdByValue(uServerValue, SUFFIX_GAUTH, ENUM_HASH_SERVERIP);

				//发送到计算出的gtransferId
				if (uServerId != (uint32_t)(-1))
				{
					RouteReliable(uServerId, Uri, obj);
					return true;
				}
				else
				{
					return false;
				}
			}

			//功能：将包直接发给hash(gid) -> gmsgcache
			//参数：uGid为群/组ID
			bool RouteGM(uint32_t uGid, uint32_t Uri, const sox::Marshallable &obj)
			{
				CHashServerValueHelper2 *pHashHelper = TBaseDaemonClientW::GetHashHelper(SUFFIX_GMSGCACHE);
				if (pHashHelper == NULL)
					return false;

				//根据因子计算服务器结果值，这里为IP
				uint32_t uServerValue = pHashHelper->GetServerValueByFactor(uGid);

				//根据服务器结果值计算出serverId
				uint32_t uServerId = TBaseDaemonClientW::GetServerIdByValue(uServerValue, SUFFIX_GMSGCACHE, ENUM_HASH_SERVERIP);

				//发送到计算出的gmsgcacheid
				if (uServerId != (uint32_t)(-1))
				{
					routeS(uServerId, Uri, obj);
					return true;
				}
				else
				{
					return false;
				}
			}

			bool RouteGMReliable(uint32_t uGid, uint32_t Uri, const sox::Marshallable &obj)
			{
				CHashServerValueHelper2 *pHashHelper = TBaseDaemonClientW::GetHashHelper(SUFFIX_GMSGCACHE);
				if (pHashHelper == NULL)
					return false;

				//根据因子计算服务器结果值，这里为IP
				uint32_t uServerValue = pHashHelper->GetServerValueByFactor(uGid);

				//根据服务器结果值计算出serverId
				uint32_t uServerId = TBaseDaemonClientW::GetServerIdByValue(uServerValue, SUFFIX_GMSGCACHE, ENUM_HASH_SERVERIP);

				//发送到计算出的gmsgcacheid
				if (uServerId != (uint32_t)(-1))
				{
					RouteReliable(uServerId, Uri, obj);
				return true;
				}
				else
				{
					return false;
				}
			}

			//功能：将广播包发给hash(gid) -> gtransfer
			//参数：uGid为群ID
			bool GBroadcast(uint32_t uSenderId, uint32_t uBroadcastId, protocol::gtransfer::EOfflineMsgMode emMode,
				uint32_t Uri, const sox::Marshallable &obj, bool bReliable = false)
			{
				server::gtransfer::PSS_GBroadcast objBroadcast(Uri, obj);
				objBroadcast.m_stGBoardcast.SetSendId(uSenderId);
				objBroadcast.m_stGBoardcast.SetOfflineMode(emMode);
				objBroadcast.m_stGBoardcast.SetBroadCastId(uBroadcastId);
				objBroadcast.m_stGBoardcast.SetKey(uBroadcastId);

				log(Info, "[%s] (GID=%d)", __FUNCTION__, uBroadcastId);

				//not send to offline, send to gchat for broadcast
				if(protocol::gtransfer::ENUM_NOT_SAVE_OFFLINE == emMode)
				{
					return RouteGC(uBroadcastId, objBroadcast.uri, objBroadcast);
				}
				else
				{
					return bReliable?RouteGTReliable(uBroadcastId, objBroadcast.uri, objBroadcast):
						RouteGT(uBroadcastId, objBroadcast.uri, objBroadcast);
				}
			}

			//功能：将广播包发给hash(gid) -> gtransfer
			//参数：uHashKey为hash的key，必须是群id，uBroadcastId为广播的范围，可以是群或者组id
			bool GBroadcast(uint32_t uSenderId, uint32_t uHashKey, uint32_t uBroadcastId, protocol::gtransfer::EOfflineMsgMode emMode,
				uint32_t Uri, const sox::Marshallable &obj)
			{
				server::gtransfer::PSS_GBroadcast objBroadcast(Uri, obj);
				objBroadcast.m_stGBoardcast.SetSendId(uSenderId);
				objBroadcast.m_stGBoardcast.SetOfflineMode(emMode);
				objBroadcast.m_stGBoardcast.SetBroadCastId(uBroadcastId);
				objBroadcast.m_stGBoardcast.SetKey(uHashKey);

				log(Info, "[%s] (HashKey=%d, uBroadcastGid=%d)", __FUNCTION__, uHashKey, uBroadcastId);

				//not send to offline, send to gchat for broadcast
				if(protocol::gtransfer::ENUM_NOT_SAVE_OFFLINE == emMode)
				{
					return RouteGC(uHashKey, objBroadcast.uri, objBroadcast);
				}
				else
				{
					return RouteGT(uHashKey, objBroadcast.uri, objBroadcast);
				}
			}

			//功能：将广播包发给hash(gid) -> gtransfer
			//参数：uHashKey为hash的key，必须是群id，uBroadcastId为广播的范围，可以是群或者组id
			bool GBroadcast(uint32_t uSenderId, uint32_t uHashKey, uint32_t uBroadcastId, protocol::gtransfer::EOfflineMsgMode emMode,
				uint32_t Uri, const std::string &strPacket)
			{
				server::gtransfer::PSS_GBroadcast objBroadcast(Uri, strPacket);
				objBroadcast.m_stGBoardcast.SetSendId(uSenderId);
				objBroadcast.m_stGBoardcast.SetOfflineMode(emMode);
				objBroadcast.m_stGBoardcast.SetBroadCastId(uBroadcastId);
				objBroadcast.m_stGBoardcast.SetKey(uHashKey);

				log(Info, "[%s] (HashKey=%d, uBroadcastGid=%d)", __FUNCTION__, uHashKey, uBroadcastId);

				//not send to offline, send to gchat for broadcast
				if(protocol::gtransfer::ENUM_NOT_SAVE_OFFLINE == emMode)
				{
					return RouteGC(uHashKey, objBroadcast.uri, objBroadcast);
				}
				else
				{
					return RouteGT(uHashKey, objBroadcast.uri, objBroadcast);
				}
			}

			//功能：将广播包发给hash(gid) -> gtransfer
			//参数：uHashKey为hash的key，必须是群id，uBroadcastId为广播的范围，可以是群或者组id
			bool GBroadcast2(uint32_t uSenderId, uint32_t uHashKey, uint32_t uBroadcastId, protocol::gtransfer::EOfflineMsgMode emMode,
				uint32_t Uri, const sox::Marshallable &obj, bool bReliable = false)
			{
				server::gtransfer::PSS_GBroadcast2 objBroadcast(Uri, obj);
				objBroadcast.m_stGBoardcast.SetSendId(uSenderId);
				objBroadcast.m_stGBoardcast.SetOfflineMode(emMode);
				objBroadcast.m_stGBoardcast.SetGid(uBroadcastId);
				objBroadcast.m_stGBoardcast.SetKey(uHashKey);

				log(Info, "[%s] (HashKey=%d, uBroadcastGid=%d)", __FUNCTION__, uHashKey, uBroadcastId);

				//not send to offline, send to gchat for broadcast
				if(protocol::gtransfer::ENUM_NOT_SAVE_OFFLINE == emMode)
				{
					return RouteGC(uHashKey, objBroadcast.uri, objBroadcast);
				}
				else
				{
					//return RouteGT(uHashKey, objBroadcast.uri, objBroadcast);
					return bReliable?RouteGTReliable(uHashKey, objBroadcast.uri, objBroadcast):
						RouteGT(uHashKey, objBroadcast.uri, objBroadcast);
				}
			}

			//功能：将广播包发给hash(gid) -> gtransfer
			//参数：uHashKey为hash的key，必须是群id，uBroadcastId为广播的范围，可以是群或者组id
			bool GBroadcast(uint32_t uSenderId, uint32_t uHashKey, uint32_t uBroadcastId, uint32_t uFid,
				protocol::gtransfer::EOfflineMsgMode emMode, uint32_t uOfflineActionMode, uint32_t Uri, const sox::Marshallable &obj, bool bReliable = false)
			{
				server::gtransfer::PSS_GBroadcast objBroadcast(Uri, obj);
				objBroadcast.m_stGBoardcast.SetSendId(uSenderId);
				objBroadcast.m_stGBoardcast.SetKey(uHashKey);
				objBroadcast.m_stGBoardcast.SetOfflineMode(emMode);
				objBroadcast.m_stGBoardcast.SetBroadCastId(uBroadcastId);
				objBroadcast.m_stGBoardcast.SetFid(uFid);

				protocol::gtransfer::OfflineActionMode offlineActionMode;
				offlineActionMode.N = uOfflineActionMode;
				objBroadcast.m_stGBoardcast.SetOfflineActionMode(uOfflineActionMode);

				log(Info, "[%s] (HashKey=%d, uBroadcastGid=%d, offlineaction=%d)", __FUNCTION__, uHashKey, uBroadcastId, uOfflineActionMode);

				//return RouteGT(uHashKey, objBroadcast.uri, objBroadcast);
				return bReliable?RouteGTReliable(uHashKey, objBroadcast.uri, objBroadcast):
					RouteGT(uHashKey, objBroadcast.uri, objBroadcast);
			}

			//功能：将转发包发给hash(gid) -> gtransfer
			//参数：uGid为群ID，如果uGid=-1，则随机选择一个gtransfer
			bool GForward(uint32_t uSenderId, uint32_t uGid, const std::vector<uint32_t> &vecRecvUid, protocol::gtransfer::EOfflineMsgMode emMode,
				uint32_t Uri, const sox::Marshallable &obj)
			{
				server::gtransfer::PSS_GForward objForward(Uri, obj);
				objForward.m_stGForward.SetSendId(uSenderId);
				objForward.m_stGForward.SetRecvUidVect(vecRecvUid);
				objForward.m_stGForward.SetOfflineMode(emMode);

				log(Info, "[%s] (GID=%d)", __FUNCTION__, uGid);

				if (uGid == (uint32_t)(-1))
				{
					route(SUFFIX_GTRANS, objForward.uri, objForward);
					return true;
				}

				return RouteGT(uGid, objForward.uri, objForward);
			}

			//功能：将转发包发给hash(gid) -> gtransfer
			//参数：uGid为群ID，如果uGid=-1，则随机选择一个gtransfer
			bool GForward(uint32_t uSenderId, uint32_t uGid, uint32_t uFid, const std::vector<uint32_t> &vecRecvUid, 
			uint32_t uOfflineActionMode, protocol::gtransfer::EOfflineMsgMode emMode, uint32_t Uri, const sox::Marshallable &obj)
			{
				server::gtransfer::PSS_GForward objForward(Uri, obj);
				objForward.m_stGForward.SetSendId(uSenderId);
				objForward.m_stGForward.SetRecvUidVect(vecRecvUid);
				objForward.m_stGForward.SetOfflineMode(emMode);
				
				objForward.m_stGForward.SetGid(uGid);
				objForward.m_stGForward.SetFid(uFid);
				objForward.m_stGForward.SetOfflineActionMode(uOfflineActionMode);

				log(Info, "[%s] (GID=%d) (FID=%d) (offlineactionmode=%d)", __FUNCTION__, uGid, uFid, uOfflineActionMode);

				if (uGid == (uint32_t)(-1))
				{
					route(SUFFIX_GTRANS, objForward.uri, objForward);
					return true;
				}

				return RouteGT(uGid, objForward.uri, objForward);
			}

			//功能：将转发包发给hash(gid) -> gtransfer
			//参数：uGid为群ID，如果uGid=-1，则随机选择一个gtransfer
			//在vecRecvUid1里随机选择uMaxNumOfOnlineReceiver个在线解释者进行转发。如果没有在线的，
			//离线转发给vecRecvUid2里的所有用户
			bool GForward2(uint32_t uSenderId, uint32_t uGid, uint32_t uMaxNumOfOnlineReceiver,
				const std::vector<uint32_t> &vecRecvUid1, const std::vector<uint32_t> &vecRecvUid2,
				protocol::gtransfer::EOfflineMsgMode emMode, uint32_t Uri, const sox::Marshallable &obj)
			{
				server::gtransfer::PSS_GForward2 objForward2(Uri, obj);
				objForward2.m_stGForward2.SetSendId(uSenderId);
				objForward2.m_stGForward2.SetRecvUidVect1(vecRecvUid1);
				objForward2.m_stGForward2.SetRecvUidVect2(vecRecvUid2);
				objForward2.m_stGForward2.SetOfflineMode(emMode);
				objForward2.m_stGForward2.SetSendNum(uMaxNumOfOnlineReceiver);

				log(Info, "[%s] (GID=%d)", __FUNCTION__, uGid);

				if (uGid == (uint32_t)(-1))
				{
					route(SUFFIX_GTRANS, objForward2.uri, objForward2);
					return true;
				}

				return RouteGT(uGid, objForward2.uri, objForward2);
			}*/
		};

		typedef CTGTransDaemonClientW<CBaseDaemonClientW2>  CGTransDaemonClientW;

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//gtransfer服务器哈希帮助类
		//供其它服务器给gtransfer发包使用

		//多线程版本

		typedef CTGTransDaemonClientW<CRBaseDaemonClientW2>  CRGTransDaemonClientW;

		//gcheck改为以IP哈希的方式，以下代码移到CTGTransDaemonClientW里
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////非双线部署版本
		//template <typename TBaseDaemonClientW>
		//class CTGCheckDaemonClientW : public TBaseDaemonClientW, //may CBaseDaemonClientW,CRBaseDaemonClientW
		//	virtual public core::IRouteTarget
		//{
		//public:
		//	CTGCheckDaemonClientW()
		//	{
		//		//注册gcheck服务器哈希，并置结果集合为ID
		//		TBaseDaemonClientW::AddServerName(SUFFIX_GCHECK, ENUM_HASH_SERVERID);
		//	}

		//	//功能：将包直接发给hash(gid) -> gcheck
		//	//参数：uGid为群/组ID
		//	bool RouteGChkReliable(uint32_t uGid, uint32_t Uri, const sox::Marshallable &obj)
		//	{
		//		CHashServerValueHelper2 *pHashHelper = TBaseDaemonClientW::GetHashHelper(SUFFIX_GCHECK);
		//		if (pHashHelper == NULL)
		//			return false;

		//		//根据因子计算服务器结果值，这里为IP
		//		uint32_t uServerId = pHashHelper->GetServerValueByFactor(uGid);

		//		//根据服务器结果值计算出serverId
		//		//uint32_t uServerId = TBaseDaemonClientW::GetServerIdByValue(uServerValue, SUFFIX_GCHECK, ENUM_HASH_SERVERID);

		//		//发送到计算出的gtransferId
		//		if (uServerId != (uint32_t)(-1))
		//			RouteReliable(uServerId, Uri, obj);
		//		return true;
		//	}

		//};

		////提供给单线程服务器
		////typedef CTGCheckDaemonClientW<CBaseDaemonClientW>  CGCheckDaemonClientW;


		template <typename TServerSelectorImp>
		class CTGTransDispatcher : public TServerSelectorImp,
										  public core::IRouteTarget,
										  public core::IDaemonServerAware
		{
		public:
			CTGTransDispatcher()
			{
				TServerSelectorImp::RegisterServerSuffix(SUFFIX_GTRANS, server::ENUM_HASH_SERVERIP, ENUM_FUNC_FNVHASH);
				TServerSelectorImp::RegisterServerSuffix(SUFFIX_GMSGCACHE, server::ENUM_HASH_SERVERIP, ENUM_FUNC_FNVHASH);
				TServerSelectorImp::RegisterServerSuffix(SUFFIX_GCHAT, server::ENUM_HASH_SERVERIP, ENUM_FUNC_FNVHASH);
				TServerSelectorImp::RegisterServerSuffix(SUFFIX_GAUTH, server::ENUM_HASH_SERVERIP, ENUM_FUNC_FNVHASH);
				TServerSelectorImp::RegisterServerSuffix(SUFFIX_GCHECK, server::ENUM_HASH_SERVERIP, ENUM_FUNC_FNVHASH);
			}

			//在网通电信之间同步数据
			//uKey, 选择对端网络进程的hash key，-1表示随机选择
			bool GTransSync(uint32_t uKey, const std::string& strSuffix, uint32_t Uri, const sox::Marshallable &obj)
			{
				uint32_t uNetType = server->getNetType();

				if (CTL == uNetType)
				{
					uNetType = NET_TYPE_CNC;
				}
				else if (CNC == uNetType)
				{
					uNetType = NET_TYPE_CTL;
				}
				else
				{
					log(Error, "[CGTransDispatcher::GTransSync] NetType %d error", uNetType);
					return false;
				}

				if ((uint32_t)-1 == uKey)
				{
					//random选择其他网络的服务器
					uint32_t uServerId = TServerSelectorImp::RandomServerByNet(strSuffix, uNetType);
					if (uServerId != (uint32_t)-1)
					{
						routeM(uServerId, Uri, obj);
						log(Info, "[CGTransDispatcher::GTransSync] routeM to %s %d", strSuffix.c_str(), uServerId);
						return true;
					}
					else
					{
						log(Error, "[CGTransDispatcher::GTransSync] random failed: server %s, net %d", strSuffix.c_str(), uNetType);
						return false;
					}
				}
				else
				{
					//hash选择其他网络的服务器
					uint32_t uServerId = TServerSelectorImp::HashServerByNet(uKey, strSuffix, uNetType);
					if (uServerId != (uint32_t)-1)
					{
						routeM(uServerId, Uri, obj);
						log(Info, "[CGTransDispatcher::GTransSync] routeM to %s %d", strSuffix.c_str(), uServerId);
						return true;
					}
					else
					{
						log(Error, "[CGTransDispatcher::GTransSync] hash failed: key %d, server %s, net %d", uKey, strSuffix.c_str(), uNetType);
						return false;
					}
				}
			}

			bool GHashDispatch(uint32_t uKey, const std::string& strSuffix, uint32_t Uri, const sox::Marshallable &obj, 
									bool bReliable = false,	const std::string& strFrom = "")
			{
				uint32_t uNetType = server->getNetType();

				//双线发电信
				if ((NET_TYPE_CTL != uNetType) && (NET_TYPE_CNC != uNetType))
				{
					uNetType = NET_TYPE_CTL;
				}

				//选择同网络的服务器
				uint32_t uServerId = TServerSelectorImp::HashServerByNet(uKey, strSuffix, uNetType);
				if (uServerId != (uint32_t)-1)
				{
					if (!bReliable)
					{
						if (strFrom.empty())
						{
							routeS(uServerId, Uri, obj);
							log(Info, "[CGTransDispatcher::GHashDispatch] routeS to %s %d", strSuffix.c_str(), uServerId);
						}
						else
						{
							routeS(strFrom, uServerId, Uri, obj);
							log(Info, "[CGTransDispatcher::GHashDispatch] routeS to %s %d", strSuffix.c_str(), uServerId);
						}
					}
					else
					{
						RouteReliable(uServerId, Uri, obj);
						log(Info, "[CGTransDispatcher::GHashDispatch] RouteReliable to %s %d", strSuffix.c_str(), uServerId);
					}
					return true;
				}
				else
				{
					log(Error, "[CGTransDispatcher::GHashDispatch] hash failed: key %d, server %s, net %d", uKey, strSuffix.c_str(), uNetType);
					return false;
				}
			}

			void GHashDispatchDouble(uint32_t uKey, const std::string& strSuffix, uint32_t Uri, const sox::Marshallable &obj, bool bReliable = false,
										const std::string& strFrom = "")
			{
				uint32_t uCTLServerId = TServerSelectorImp::HashServerByNet(uKey, strSuffix, NET_TYPE_CTL);
				uint32_t uCNCServerId = TServerSelectorImp::HashServerByNet(uKey, strSuffix, NET_TYPE_CNC);

				if (!bReliable)
				{
					if (strFrom.empty())
					{
						if (uCTLServerId != (uint32_t)-1)
						{
							routeS(uCTLServerId, Uri, obj);
							log(Info, "[CGTransDispatcher::GHashDispatchDouble] routeS to CTL %s %d", strSuffix.c_str(), uCTLServerId);
						}
						
						if (uCNCServerId != (uint32_t)-1)
						{
							routeS(uCNCServerId, Uri, obj);
							log(Info, "[CGTransDispatcher::GHashDispatchDouble] routeS to CNC %s %d", strSuffix.c_str(), uCNCServerId);
						}
					}
					else
					{
						if (uCTLServerId != (uint32_t)-1)
						{
							routeS(strFrom, uCTLServerId, Uri, obj);
							log(Info, "[CGTransDispatcher::GHashDispatchDouble] routeS to CTL %s %d", strSuffix.c_str(), uCTLServerId);
						}

						if (uCNCServerId != (uint32_t)-1)
						{
							routeS(strFrom, uCNCServerId, Uri, obj);
							log(Info, "[CGTransDispatcher::GHashDispatchDouble] routeS to CNC %s %d", strSuffix.c_str(), uCNCServerId);
						}
					}
				}
				else
				{
					if (uCTLServerId != (uint32_t)-1)
					{
						RouteReliable(uCTLServerId, Uri, obj);
						log(Info, "[CGTransDispatcher::GHashDispatchDouble] RouteReliable to CTL %s %d", strSuffix.c_str(), uCTLServerId);
					}

					if (uCNCServerId != (uint32_t)-1)
					{
						RouteReliable(uCNCServerId, Uri, obj);
						log(Info, "[CGTransDispatcher::GHashDispatchDouble] RouteReliable to CNC %s %d", strSuffix.c_str(), uCNCServerId);
					}
				}
			}

			bool RouteGT(uint32_t uGid, uint32_t Uri, const sox::Marshallable &obj)
			{
				return GHashDispatch(uGid, SUFFIX_GTRANS, Uri, obj);
			}

			bool RouteGTReliable(uint32_t uGid, uint32_t Uri, const sox::Marshallable &obj)
			{
				return GHashDispatch(uGid, SUFFIX_GTRANS, Uri, obj, true);
			}

			//功能：将包直接发给hash(gid) -> gchat
			//参数：uGid为群ID
			bool RouteGC(uint32_t uGid, uint32_t Uri, const sox::Marshallable &obj)
			{
				return GHashDispatch(uGid, SUFFIX_GCHAT, Uri, obj);
			}

			bool RouteGC(std::string &strFrom, uint32_t uGid, uint32_t Uri, const sox::Marshallable &obj)
			{
				return GHashDispatch(uGid, SUFFIX_GCHAT, Uri, obj);
			}

			bool RouteGCReliable(uint32_t uGid, uint32_t Uri, const sox::Marshallable &obj)
			{
				return GHashDispatch(uGid, SUFFIX_GCHAT, Uri, obj, true);
			}

			//功能：将包直接发给hash(gid) -> gcheck
			//参数：uGid为群/组ID
			bool RouteGChkReliable(uint32_t uGid, uint32_t Uri, const sox::Marshallable &obj)
			{
				return GHashDispatch(uGid, SUFFIX_GCHECK, Uri, obj, true);
			}

			//功能：将包直接发给hash(gid) -> gauth
			//参数：uGid为群/组ID
			bool RouteGA(uint32_t uGid, uint32_t Uri, const sox::Marshallable &obj)
			{
				return GHashDispatch(uGid, SUFFIX_GAUTH, Uri, obj);
			}

			bool RouteGAReliable(uint32_t uGid, uint32_t Uri, const sox::Marshallable &obj)
			{
				return GHashDispatch(uGid, SUFFIX_GAUTH, Uri, obj, true);
			}

			//功能：将包直接发给hash(gid) -> gmsgcache
			//参数：uGid为群/组ID
			bool RouteGM(uint32_t uGid, uint32_t Uri, const sox::Marshallable &obj)
			{
				return GHashDispatch(uGid, SUFFIX_GMSGCACHE, Uri, obj);
			}

			bool RouteGMReliable(uint32_t uGid, uint32_t Uri, const sox::Marshallable &obj)
			{
				return GHashDispatch(uGid, SUFFIX_GMSGCACHE, Uri, obj, true);
			}

			//功能：将广播包发给hash(gid) -> gtransfer
			//参数：uHashKey为hash的key，必须是群id，uBroadcastId为广播的范围，可以是群或者组id
			bool GBroadcast(uint32_t uSenderId, uint32_t uKey, uint32_t uBroadcastId,  
								uint32_t Uri, const sox::Marshallable &obj,	protocol::gtransfer::EOfflineMsgMode emMode, 
								bool bReliable = false, uint32_t uOfflineActionMode = TO_ALL_MODE)
			{
				server::gtransfer::PSS_GBroadcast objBroadcast(Uri, obj);
				objBroadcast.m_stGBoardcast.SetSendId(uSenderId);
				objBroadcast.m_stGBoardcast.SetOfflineMode(emMode);
				objBroadcast.m_stGBoardcast.SetBroadCastId(uBroadcastId);
				objBroadcast.m_stGBoardcast.SetKey(uKey);

				protocol::gtransfer::OfflineActionMode offlineActionMode;
				offlineActionMode.N = uOfflineActionMode;
				objBroadcast.m_stGBoardcast.SetOfflineActionMode(uOfflineActionMode);

				log(Info, "[%s] (Key=%d, uBroadcastGid=%d)", __FUNCTION__, uKey, uBroadcastId);

				//not send to offline, send to gchat for broadcast
				if(protocol::gtransfer::ENUM_NOT_SAVE_OFFLINE == emMode)
				{
					return bReliable?RouteGCReliable(uKey, objBroadcast.uri, objBroadcast):RouteGC(uKey, objBroadcast.uri, objBroadcast);
				}
				else
				{
					return bReliable?RouteGTReliable(uKey, objBroadcast.uri, objBroadcast):RouteGT(uKey, objBroadcast.uri, objBroadcast);
				}
			}

			bool GBroadcast(uint32_t uSenderId, uint32_t uKey, uint32_t uBroadcastId,  
								uint32_t Uri, const sox::Marshallable &obj,	protocol::gtransfer::EOfflineMsgMode emMode, 
								uint32_t uFid, bool bReliable = false, uint32_t uOfflineActionMode = TO_ALL_MODE)
			{
				server::gtransfer::PSS_GBroadcast objBroadcast(Uri, obj);
				objBroadcast.m_stGBoardcast.SetSendId(uSenderId);
				objBroadcast.m_stGBoardcast.SetOfflineMode(emMode);
				objBroadcast.m_stGBoardcast.SetBroadCastId(uBroadcastId);
				objBroadcast.m_stGBoardcast.SetKey(uKey);
				objBroadcast.m_stGBoardcast.SetFid(uFid);

				protocol::gtransfer::OfflineActionMode offlineActionMode;
				offlineActionMode.N = uOfflineActionMode;
				objBroadcast.m_stGBoardcast.SetOfflineActionMode(uOfflineActionMode);

				log(Info, "[%s] (Key=%d, uBroadcastGid=%d)", __FUNCTION__, uKey, uBroadcastId);

				//not send to offline, send to gchat for broadcast
				if(protocol::gtransfer::ENUM_NOT_SAVE_OFFLINE == emMode)
				{
					return bReliable?RouteGCReliable(uKey, objBroadcast.uri, objBroadcast):RouteGC(uKey, objBroadcast.uri, objBroadcast);
				}
				else
				{
					return bReliable?RouteGTReliable(uKey, objBroadcast.uri, objBroadcast):RouteGT(uKey, objBroadcast.uri, objBroadcast);
				}
			}

			//功能：将广播包发给hash(gid) -> gtransfer
			//参数：uHashKey为hash的key，必须是群id，uBroadcastId为广播的范围，可以是群或者组id
			bool GBroadcast(uint32_t uSenderId, uint32_t uKey, uint32_t uBroadcastId,  
								uint32_t Uri, const std::string &strPacket,	protocol::gtransfer::EOfflineMsgMode emMode, 
								uint32_t uOfflineActionMode = TO_ALL_MODE)
			{
				server::gtransfer::PSS_GBroadcast objBroadcast(Uri, strPacket);
				objBroadcast.m_stGBoardcast.SetSendId(uSenderId);
				objBroadcast.m_stGBoardcast.SetOfflineMode(emMode);
				objBroadcast.m_stGBoardcast.SetBroadCastId(uBroadcastId);
				objBroadcast.m_stGBoardcast.SetKey(uKey);

				protocol::gtransfer::OfflineActionMode offlineActionMode;
				offlineActionMode.N = uOfflineActionMode;
				objBroadcast.m_stGBoardcast.SetOfflineActionMode(uOfflineActionMode);

				log(Info, "[%s] (Key=%d, uBroadcastGid=%d)", __FUNCTION__, uKey, uBroadcastId);

				//not send to offline, send to gchat for broadcast
				if(protocol::gtransfer::ENUM_NOT_SAVE_OFFLINE == emMode)
				{
					return RouteGC(uKey, objBroadcast.uri, objBroadcast);
				}
				else
				{
					return RouteGT(uKey, objBroadcast.uri, objBroadcast);
				}
			}

			//功能：将广播包发给hash(gid) -> gtransfer
			//参数：uHashKey为hash的key，必须是群id，uBroadcastId为广播的范围，可以是群或者组id
			bool GBroadcast2(uint32_t uSenderId, uint32_t uHashKey, uint32_t uBroadcastId, protocol::gtransfer::EOfflineMsgMode emMode,
				uint32_t Uri, const sox::Marshallable &obj, bool bReliable = false)
			{
				server::gtransfer::PSS_GBroadcast2 objBroadcast(Uri, obj);
				objBroadcast.m_stGBoardcast.SetSendId(uSenderId);
				objBroadcast.m_stGBoardcast.SetOfflineMode(emMode);
				objBroadcast.m_stGBoardcast.SetGid(uBroadcastId);
				objBroadcast.m_stGBoardcast.SetKey(uHashKey);

				log(Info, "[%s] (HashKey=%d, uBroadcastGid=%d)", __FUNCTION__, uHashKey, uBroadcastId);

				//not send to offline, send to gchat for broadcast
				if(protocol::gtransfer::ENUM_NOT_SAVE_OFFLINE == emMode)
				{
					return RouteGC(uHashKey, objBroadcast.uri, objBroadcast);
				}
				else
				{
					//return RouteGT(uHashKey, objBroadcast.uri, objBroadcast);
					return bReliable?RouteGTReliable(uHashKey, objBroadcast.uri, objBroadcast):
						RouteGT(uHashKey, objBroadcast.uri, objBroadcast);
				}
			}

			//功能：将转发包发给hash(gid) -> gtransfer
			//参数：uGid为群ID
			void GForward(uint32_t uSenderId, uint32_t uGid, const std::vector<uint32_t> &vecRecvUid, protocol::gtransfer::EOfflineMsgMode emMode,
				uint32_t Uri, const sox::Marshallable &obj)
			{
				server::gtransfer::PSS_GForward objForward(Uri, obj);
				objForward.m_stGForward.SetSendId(uSenderId);
				objForward.m_stGForward.SetRecvUidVect(vecRecvUid);
				objForward.m_stGForward.SetOfflineMode(emMode);

				log(Info, "[%s] (GID=%d)", __FUNCTION__, uGid);
				
				RouteGT(uGid, objForward.uri, objForward);
			}

			//功能：将转发包发给hash(gid) -> gtransfer
			//参数：uGid为群ID
			void GForward(uint32_t uSenderId, uint32_t uGid, uint32_t uFid, const std::vector<uint32_t> &vecRecvUid, 
			uint32_t uOfflineActionMode, protocol::gtransfer::EOfflineMsgMode emMode, uint32_t Uri, const sox::Marshallable &obj)
			{
				server::gtransfer::PSS_GForward objForward(Uri, obj);
				objForward.m_stGForward.SetSendId(uSenderId);
				objForward.m_stGForward.SetRecvUidVect(vecRecvUid);
				objForward.m_stGForward.SetOfflineMode(emMode);
				
				objForward.m_stGForward.SetGid(uGid);
				objForward.m_stGForward.SetFid(uFid);
				objForward.m_stGForward.SetOfflineActionMode(uOfflineActionMode);

				log(Info, "[%s] (GID=%d) (FID=%d) (offlineactionmode=%d)", __FUNCTION__, uGid, uFid, uOfflineActionMode);

				RouteGT(uGid, objForward.uri, objForward);
			}

			//功能：将转发包发给hash(gid) -> gtransfer
			//参数：uGid为群ID
			//在vecRecvUid1里随机选择uMaxNumOfOnlineReceiver个在线解释者进行转发。如果没有在线的，
			//离线转发给vecRecvUid2里的所有用户
			void GForward2(uint32_t uSenderId, uint32_t uGid, uint32_t uMaxNumOfOnlineReceiver,
				const std::vector<uint32_t> &vecRecvUid1, const std::vector<uint32_t> &vecRecvUid2,
				protocol::gtransfer::EOfflineMsgMode emMode, uint32_t Uri, const sox::Marshallable &obj)
			{
				server::gtransfer::PSS_GForward2 objForward2(Uri, obj);
				objForward2.m_stGForward2.SetSendId(uSenderId);
				objForward2.m_stGForward2.SetRecvUidVect1(vecRecvUid1);
				objForward2.m_stGForward2.SetRecvUidVect2(vecRecvUid2);
				objForward2.m_stGForward2.SetOfflineMode(emMode);
				objForward2.m_stGForward2.SetSendNum(uMaxNumOfOnlineReceiver);

				log(Info, "[%s] (GID=%d)", __FUNCTION__, uGid);

				RouteGT(uGid, objForward2.uri, objForward2);
			}
		};

		typedef CTGTransDispatcher<server::CServerSelectorImp>  CGTransDispatcher;

		typedef CTGTransDispatcher<server::CRServerSelectorImp>  CRGTransDispatcher;

		class CGTransDispatcherAwareW : public server::IServerSelectorWatcher
		{
			protected:
				CGTransDispatcher *m_pGTransDispatcher;
			
			public:
				CGTransDispatcherAwareW() : m_pGTransDispatcher(NULL) {};
			
				void SetGTransDispatcher(CGTransDispatcher *pGTransDispatcher)
				{
					m_pGTransDispatcher = pGTransDispatcher;
					m_pGTransDispatcher->watch(this);
				}
			
				CGTransDispatcher *GetGTransDispatcher()
				{
					return m_pGTransDispatcher;
				}
		};

		class CRGTransDispatcherAware
		{
			protected:
				CRGTransDispatcher *m_pGTransDispatcher;
			
			public:
				CRGTransDispatcherAware() : m_pGTransDispatcher(NULL) {};
			
				void SetGTransDispatcher(CRGTransDispatcher *pGTransDispatcher)
				{
					m_pGTransDispatcher = pGTransDispatcher;
				}
			
				CRGTransDispatcher *GetGTransDispatcher()
				{
					return m_pGTransDispatcher;
				}
		};

#define CONFIG_GTRANS_DISPATCHER(AppObj)\
		CGTransDispatcher __gtransdispatcher;\
		__gtransdispatcher.setWriter(&__writer);\
		__gtransdispatcher.setServer(&__server);\
		__gtransdispatcher.setDaemonClient(&__clientDaemon);\
		AppObj.SetGTransDispatcher(&__gtransdispatcher);

#define CONFIG_RGTRANS_DISPATCHER(AppObj)\
		CRGTransDispatcher __gtransdispatcher;\
		__gtransdispatcher.setWriter(&__writer);\
		__gtransdispatcher.setServer(&__server);\
		__gtransdispatcher.setDaemonClient(&__clientDaemon);\
		AppObj.SetGTransDispatcher(&__gtransdispatcher);

	}
}
