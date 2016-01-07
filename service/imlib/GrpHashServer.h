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
#include <boost/thread/thread.hpp>
#include "server_common/protocol/PGTransferSrv.h"

//#define FNV_HASH
#ifdef FNV_HASH
    #include "ConsistentFnvHash.h"
#else
    #include "ConsistentHashing2.h"
#endif

namespace server
{
	namespace grp
	{

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//ͨ�÷�������ϣ������

		class CHashServerValueHelper2
		{
		public:

			virtual ~CHashServerValueHelper2() {};

			virtual bool AddServer(uint32_t uServerValue);

			virtual bool RemoveServer(uint32_t uServerValue);

			virtual void ClearServer();

			//�������ӣ��Ӽ���������һ��Valueֵ���������Ϊ��,���� NONE_SERVER_ID

			virtual uint32_t GetServerValueByFactor(uint32_t uFactor);

			//��������Value�ڼ������Ƿ����

			virtual bool IsServerValueExist(uint32_t uServerValue);

			//ȡȫ��������ID����

			virtual void GetServerValueSet(std::set<uint32_t> &setServerValue);

		private:
            #ifdef FNV_HASH
                UserConsistentFnvHash m_Hash;
            #else
                UserConsistentHash2 m_Hash;
            #endif
            
			std::set<uint32_t> m_setServerValue;
		};

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//��������ϣ����

		enum EHashServerValueType
		{
			ENUM_HASH_SERVERID = 0, //��serverId����ϣ���ֵ
			ENUM_HASH_SERVERIP //��ServerIp����ϣ���ֵ
		};

		//ͨ�õķ�������ϣ������

		template <typename THashServerValueHelper>
		class CTBaseDaemonClientW2 : public core::IDaemonClientAwareW
		{
			typedef typename std::map<std::string, THashServerValueHelper*>::iterator MAP_HASHSERVERS_ITER;

			std::map<std::string, THashServerValueHelper*> m_mapHashHelper; //���������� - ��ϣ��������
			std::map<std::string, EHashServerValueType> m_mapHashType; //���������� - ��ϣ���ֵ����
			std::map<uint32_t, sdaemon::FavorStrSV> m_mapServers; //ά�����ĵķ������б�

		protected:
			//------------------------------------------------------------------------------------------------
			//��ӷ��������͹�ϣ

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

			//��ȡָ�����������͹�ϣ����

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

				//������й�ϣ������
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
						uint32_t uServerValue = GetServerValue(*it); //�Ҳ���ע�����ͣ��ͷ���-1
						if (uServerValue != (uint32_t)(-1))
							iterHashHelper->second->AddServer(uServerValue);
						else
							log(Warn, "[CTBaseDaemonClientW::onRefreshSV] GetServerValue(%s) return -1!", it->sstr.c_str());

						AddMapServer(*it);
					}

					//��ӡ��������ͣ��¼
					if (sdaemon::RUNNING == it->serverStatus)
					{
						log(Info, "[CWatchServerDaemonClient::onRefreshSV] ServerId[%d] ServerName[%s] ServerIp[%s]",
							it->serverId, it->sstr.c_str(), __SVToIP(*it).c_str());
					}
				}

				OnUserInit(); //��ʼ������
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

					OnUserAddjust(fs.sstr); //����������ʱ������������Ĵ���
				}

				//��ӡ��������ͣ��¼
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
						pHashHelper->RemoveServer(uServerValue); //ȷ��-1Ҳ��ɾ��
					}

					RemoveMapServer(fs);

					OnUserAddjust(fs.sstr); //����������ʱ������������Ĵ���
				}

				//��ӡ��������ͣ��¼
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

			virtual void OnUserInit() {} //onRefreshSVʱ����
			virtual void OnUserAddjust(const std::string strServerName) {} //onAddSV onServerRemovedSVʱ����

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

			//���ݷ�����VALUEֵ -> ������ID ������VALUEֵ�� IP ���� ID��
			//������ָ��VALUE����ֵ�������޷��ڷ������м���
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

			//ͨ�����������ݽṹ���������ĵķ�����HASH���ֵ
			//���ñ�����ʱ�����뱣֤�����˷���������

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
			//IP��ַת��Ϊ�ַ���

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
		//gtransfer��������ϣ������
		//��������������gtransfer����ʹ��

		template <typename TBaseDaemonClientW>
		class CTGTransDaemonClientW : public TBaseDaemonClientW, //may CBaseDaemonClientW,CRBaseDaemonClientW
			virtual public core::IRouteTarget
		{
		public:
			CTGTransDaemonClientW()
			{
				//ע��gtrans��������ϣ�����ý������ΪIP
				TBaseDaemonClientW::AddServerName(SUFFIX_GTRANS, ENUM_HASH_SERVERIP);
				//ע��gchat��������ϣ�����ý������ΪIP
				TBaseDaemonClientW::AddServerName(SUFFIX_GCHAT, ENUM_HASH_SERVERIP);				
				//ע��gauth��������ϣ�����ý������ΪIP
				TBaseDaemonClientW::AddServerName(SUFFIX_GAUTH, ENUM_HASH_SERVERIP);
				//ע��gauth��������ϣ�����ý������ΪIP
				TBaseDaemonClientW::AddServerName(SUFFIX_GMSGCACHE, ENUM_HASH_SERVERIP);
			}

			//���ܣ�����ֱ�ӷ���hash(gid) -> gtransfer
			//������uGidΪȺ/��ID
			bool RouteGT(uint32_t uGid, uint32_t Uri, const sox::Marshallable &obj)
			{
				CHashServerValueHelper2 *pHashHelper = TBaseDaemonClientW::GetHashHelper(SUFFIX_GTRANS);
				if (pHashHelper == NULL)
					return false;

				//�������Ӽ�����������ֵ������ΪIP
				uint32_t uServerValue = pHashHelper->GetServerValueByFactor(uGid);

				//���ݷ��������ֵ�����serverId
				uint32_t uServerId = TBaseDaemonClientW::GetServerIdByValue(uServerValue, SUFFIX_GTRANS, ENUM_HASH_SERVERIP);

				//���͵��������gtransferId
				if (uServerId != (uint32_t)(-1))
					routeS(uServerId, Uri, obj);
				return true;
			}

			bool RouteGTReliable(uint32_t uGid, uint32_t Uri, const sox::Marshallable &obj)
			{
				CHashServerValueHelper2 *pHashHelper = TBaseDaemonClientW::GetHashHelper(SUFFIX_GTRANS);
				if (pHashHelper == NULL)
					return false;

				//�������Ӽ�����������ֵ������ΪIP
				uint32_t uServerValue = pHashHelper->GetServerValueByFactor(uGid);

				//���ݷ��������ֵ�����serverId
				uint32_t uServerId = TBaseDaemonClientW::GetServerIdByValue(uServerValue, SUFFIX_GTRANS, ENUM_HASH_SERVERIP);

				//���͵��������gtransferId
				if (uServerId != (uint32_t)(-1))
					RouteReliable(uServerId, Uri, obj);
				return true;
			}

			bool RouteGT(std::string &strFrom, uint32_t uGid, uint32_t Uri, const std::string &strPacket)
			{
				CHashServerValueHelper2 *pHashHelper = TBaseDaemonClientW::GetHashHelper(SUFFIX_GTRANS);
				if (pHashHelper == NULL)
					return false;

				//�������Ӽ�����������ֵ������ΪIP
				uint32_t uServerValue = pHashHelper->GetServerValueByFactor(uGid);

				//���ݷ��������ֵ�����serverId
				uint32_t uServerId = TBaseDaemonClientW::GetServerIdByValue(uServerValue, SUFFIX_GTRANS, ENUM_HASH_SERVERIP);

				//���͵��������gtransferId
				if (uServerId != (uint32_t)(-1))
					routeS(strFrom, uServerId, Uri, strPacket);

				return true;
			}

			//���ܣ�����ֱ�ӷ���hash(gid) -> gchat
			//������uGidΪȺID
			bool RouteGC(uint32_t uGid, uint32_t Uri, const sox::Marshallable &obj)
			{
				CHashServerValueHelper2 *pHashHelper = TBaseDaemonClientW::GetHashHelper(SUFFIX_GCHAT);
				if (pHashHelper == NULL)
					return false;

				//�������Ӽ�����������ֵ������ΪIP
				uint32_t uServerValue = pHashHelper->GetServerValueByFactor(uGid);

				//���ݷ��������ֵ�����serverId
				uint32_t uServerId = TBaseDaemonClientW::GetServerIdByValue(uServerValue, SUFFIX_GCHAT, ENUM_HASH_SERVERIP);

				//���͵��������gchatId
				if (uServerId != (uint32_t)(-1))
					routeS(uServerId, Uri, obj);
				return true;
			}

			bool RouteGC(std::string &strFrom, uint32_t uGid, uint32_t Uri, const std::string &strPacket)
			{
				CHashServerValueHelper2 *pHashHelper = TBaseDaemonClientW::GetHashHelper(SUFFIX_GCHAT);
				if (pHashHelper == NULL)
					return false;

				//�������Ӽ�����������ֵ������ΪIP
				uint32_t uServerValue = pHashHelper->GetServerValueByFactor(uGid);

				//���ݷ��������ֵ�����serverId
				uint32_t uServerId = TBaseDaemonClientW::GetServerIdByValue(uServerValue, SUFFIX_GCHAT, ENUM_HASH_SERVERIP);

				//���͵��������gchatId
				if (uServerId != (uint32_t)(-1))
					routeS(strFrom, uServerId, Uri, strPacket);

				return true;
			}

			bool RouteGCReliable(uint32_t uGid, uint32_t Uri, const sox::Marshallable &obj)
			{
				CHashServerValueHelper2 *pHashHelper = TBaseDaemonClientW::GetHashHelper(SUFFIX_GCHAT);
				if (pHashHelper == NULL)
					return false;

				//�������Ӽ�����������ֵ������ΪIP
				uint32_t uServerValue = pHashHelper->GetServerValueByFactor(uGid);

				//���ݷ��������ֵ�����serverId
				uint32_t uServerId = TBaseDaemonClientW::GetServerIdByValue(uServerValue, SUFFIX_GCHAT, ENUM_HASH_SERVERIP);

				//���͵��������gchatId
				if (uServerId != (uint32_t)(-1))
					RouteReliable(uServerId, Uri, obj);
				return true;
			}

			//���ܣ�����ֱ�ӷ���hash(gid) -> gauth
			//������uGidΪȺ/��ID
			bool RouteGA(uint32_t uGid, uint32_t Uri, const sox::Marshallable &obj)
			{
				CHashServerValueHelper2 *pHashHelper = TBaseDaemonClientW::GetHashHelper(SUFFIX_GAUTH);
				if (pHashHelper == NULL)
					return false;

				//�������Ӽ�����������ֵ������ΪIP
				uint32_t uServerValue = pHashHelper->GetServerValueByFactor(uGid);

				//���ݷ��������ֵ�����serverId
				uint32_t uServerId = TBaseDaemonClientW::GetServerIdByValue(uServerValue, SUFFIX_GAUTH, ENUM_HASH_SERVERIP);

				//���͵��������gauthid
				if (uServerId != (uint32_t)(-1))
					routeS(uServerId, Uri, obj);
				return true;
			}

			bool RouteGAReliable(uint32_t uGid, uint32_t Uri, const sox::Marshallable &obj)
			{
				CHashServerValueHelper2 *pHashHelper = TBaseDaemonClientW::GetHashHelper(SUFFIX_GAUTH);
				if (pHashHelper == NULL)
					return false;

				//�������Ӽ�����������ֵ������ΪIP
				uint32_t uServerValue = pHashHelper->GetServerValueByFactor(uGid);

				//���ݷ��������ֵ�����serverId
				uint32_t uServerId = TBaseDaemonClientW::GetServerIdByValue(uServerValue, SUFFIX_GAUTH, ENUM_HASH_SERVERIP);

				//���͵��������gtransferId
				if (uServerId != (uint32_t)(-1))
					RouteReliable(uServerId, Uri, obj);
				return true;
			}

			//���ܣ�����ֱ�ӷ���hash(gid) -> gmsgcache
			//������uGidΪȺ/��ID
			bool RouteGM(uint32_t uGid, uint32_t Uri, const sox::Marshallable &obj)
			{
				CHashServerValueHelper2 *pHashHelper = TBaseDaemonClientW::GetHashHelper(SUFFIX_GMSGCACHE);
				if (pHashHelper == NULL)
					return false;

				//�������Ӽ�����������ֵ������ΪIP
				uint32_t uServerValue = pHashHelper->GetServerValueByFactor(uGid);

				//���ݷ��������ֵ�����serverId
				uint32_t uServerId = TBaseDaemonClientW::GetServerIdByValue(uServerValue, SUFFIX_GMSGCACHE, ENUM_HASH_SERVERIP);

				//���͵��������gmsgcacheid
				if (uServerId != (uint32_t)(-1))
					routeS(uServerId, Uri, obj);
				return true;
			}

			bool RouteGMReliable(uint32_t uGid, uint32_t Uri, const sox::Marshallable &obj)
			{
				CHashServerValueHelper2 *pHashHelper = TBaseDaemonClientW::GetHashHelper(SUFFIX_GMSGCACHE);
				if (pHashHelper == NULL)
					return false;

				//�������Ӽ�����������ֵ������ΪIP
				uint32_t uServerValue = pHashHelper->GetServerValueByFactor(uGid);

				//���ݷ��������ֵ�����serverId
				uint32_t uServerId = TBaseDaemonClientW::GetServerIdByValue(uServerValue, SUFFIX_GMSGCACHE, ENUM_HASH_SERVERIP);

				//���͵��������gmsgcacheid
				if (uServerId != (uint32_t)(-1))
					RouteReliable(uServerId, Uri, obj);
				return true;
			}

			//���ܣ����㲥������hash(gid) -> gtransfer
			//������uGidΪȺID
			bool GBroadcast(uint32_t uSenderId, uint32_t uGid, protocol::gtransfer::EOfflineMsgMode emMode,
				uint32_t Uri, const sox::Marshallable &obj)
			{
				server::gtransfer::PSS_GBroadcast objBroadcast(Uri, obj);
				objBroadcast.m_stGBoardcast.SetSendId(uSenderId);
				objBroadcast.m_stGBoardcast.SetOfflineMode(emMode);
				objBroadcast.m_stGBoardcast.SetGid(uGid); //fixed by liny
				objBroadcast.m_stGBoardcast.SetKey(uGid);

				log(Info, "[%s] (GID=%d)", __FUNCTION__, uGid);

				//not send to offline, send to gchat for broadcast
				if(protocol::gtransfer::ENUM_NOT_SAVE_OFFLINE == emMode)
				{
					return RouteGC(uGid, objBroadcast.uri, objBroadcast);
				}
				else
				{
					return RouteGT(uGid, objBroadcast.uri, objBroadcast);
				}
			}

			//���ܣ����㲥������hash(gid) -> gtransfer
			//������uHashKeyΪhash��key��������Ⱥid��uBroadcastGidΪ�㲥�ķ�Χ��������Ⱥ������id
			bool GBroadcast(uint32_t uSenderId, uint32_t uHashKey, uint32_t uBroadcastGid, protocol::gtransfer::EOfflineMsgMode emMode,
				uint32_t Uri, const sox::Marshallable &obj)
			{
				server::gtransfer::PSS_GBroadcast objBroadcast(Uri, obj);
				objBroadcast.m_stGBoardcast.SetSendId(uSenderId);
				objBroadcast.m_stGBoardcast.SetOfflineMode(emMode);
				objBroadcast.m_stGBoardcast.SetGid(uBroadcastGid);
				objBroadcast.m_stGBoardcast.SetKey(uHashKey);

				log(Info, "[%s] (HashKey=%d, uBroadcastGid=%d)", __FUNCTION__, uHashKey, uBroadcastGid);

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

			//���ܣ����㲥������hash(gid) -> gtransfer
			//������uHashKeyΪhash��key��������Ⱥid��uBroadcastGidΪ�㲥�ķ�Χ��������Ⱥ������id
			bool GBroadcast(uint32_t uSenderId, uint32_t uHashKey, uint32_t uBroadcastGid, protocol::gtransfer::EOfflineMsgMode emMode,
				uint32_t Uri, const std::string &strPacket)
			{
				server::gtransfer::PSS_GBroadcast objBroadcast(Uri, strPacket);
				objBroadcast.m_stGBoardcast.SetSendId(uSenderId);
				objBroadcast.m_stGBoardcast.SetOfflineMode(emMode);
				objBroadcast.m_stGBoardcast.SetGid(uBroadcastGid);
				objBroadcast.m_stGBoardcast.SetKey(uHashKey);

				log(Info, "[%s] (HashKey=%d, uBroadcastGid=%d)", __FUNCTION__, uHashKey, uBroadcastGid);

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

			//���ܣ���ת��������hash(gid) -> gtransfer
			//������uGidΪȺID�����uGid=-1�������ѡ��һ��gtransfer
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

			//���ܣ���ת��������hash(gid) -> gtransfer
			//������uGidΪȺID�����uGid=-1�������ѡ��һ��gtransfer
			//��vecRecvUid1�����ѡ��uMaxNumOfOnlineReceiver�����߽����߽���ת�������û�����ߵģ�
			//����ת����vecRecvUid2��������û�
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
			}
		};

		typedef CTGTransDaemonClientW<CBaseDaemonClientW2>  CGTransDaemonClientW;

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//gtransfer��������ϣ������
		//��������������gtransfer����ʹ��

		//���̰߳汾

		typedef CTGTransDaemonClientW<CRBaseDaemonClientW2>  CRGTransDaemonClientW;

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//��˫�߲���汾
		template <typename TBaseDaemonClientW>
		class CTGCheckDaemonClientW : public TBaseDaemonClientW, //may CBaseDaemonClientW,CRBaseDaemonClientW
			virtual public core::IRouteTarget
		{
		public:
			CTGCheckDaemonClientW()
			{
				//ע��gcheck��������ϣ�����ý������ΪID
				TBaseDaemonClientW::AddServerName(SUFFIX_GCHECK, ENUM_HASH_SERVERID);
			}

			//���ܣ�����ֱ�ӷ���hash(gid) -> gcheck
			//������uGidΪȺ/��ID
			bool RouteGChkReliable(uint32_t uGid, uint32_t Uri, const sox::Marshallable &obj)
			{
				CHashServerValueHelper2 *pHashHelper = TBaseDaemonClientW::GetHashHelper(SUFFIX_GCHECK);
				if (pHashHelper == NULL)
					return false;

				//�������Ӽ�����������ֵ������ΪIP
				uint32_t uServerId = pHashHelper->GetServerValueByFactor(uGid);

				//���ݷ��������ֵ�����serverId
				//uint32_t uServerId = TBaseDaemonClientW::GetServerIdByValue(uServerValue, SUFFIX_GCHECK, ENUM_HASH_SERVERID);

				//���͵��������gtransferId
				if (uServerId != (uint32_t)(-1))
					RouteReliable(uServerId, Uri, obj);
				return true;
			}

		};

		//�ṩ�����̷߳�����
		//typedef CTGCheckDaemonClientW<CBaseDaemonClientW>  CGCheckDaemonClientW;
	}
}
