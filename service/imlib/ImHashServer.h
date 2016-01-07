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
		//ͨ�÷�������ϣ������

		class CHashServerValueHelper
		{
		public:

			virtual ~CHashServerValueHelper() {};

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
			UserConsistentHash m_Hash;
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
		class CTBaseDaemonClientW : public core::IDaemonClientAwareW
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

			//��ȡָ�����������͹�ϣ����

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
							log(Info, "[CTBaseDaemonClientW::onRefreshSV] GetServerValue(%s) return -1, (just look here)", it->sstr.c_str());

						AddMapServer(*it);
					}

					//��ӡ��������ͣ��¼
					if (it->serverStatus == sdaemon::RUNNING)
					{
						log(Info, "[CWatchServerDaemonClient::onRefreshSV] ServerId[%d] ServerName[%s] ServerIp[%s]", 
							it->serverId, it->sstr.c_str(), __SVToIP(*it).c_str());
					}
				}

				OnUserInit(); //��ʹ������
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

					CHashServerValueHelper *pHashHelper = iterHashHelper->second;
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


		typedef CTBaseDaemonClientW<CHashServerValueHelper>  CBaseDaemonClientW;


		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//gtransfer��������ϣ������
		//��������������gtransfer����ʹ��

		//template <typename TBaseDaemonClientW>
		//class CTGTransDaemonClientW : public TBaseDaemonClientW, //may CBaseDaemonClientW,CRBaseDaemonClientW
		//									 public core::IRouteTarget
		//{
		//public:
		//	CTGTransDaemonClientW()
		//	{
		//		//ע��gtrans��������ϣ�����ý������ΪIP
		//		TBaseDaemonClientW::AddServerName(SUFFIX_GTRANS, ENUM_HASH_SERVERIP);
		//	}

		//	//���ܣ�����ֱ�ӷ���hash(gid) -> gtransfer
		//	//������uGidΪȺ/��ID
		//	bool RouteGT(uint32_t uGid, uint32_t Uri, const sox::Marshallable &obj)
		//	{
		//		CHashServerValueHelper *pHashHelper = TBaseDaemonClientW::GetHashHelper(SUFFIX_GTRANS);
		//		if (pHashHelper == NULL)
		//			return false;

		//		//�������Ӽ�����������ֵ������ΪIP
		//		uint32_t uServerValue = pHashHelper->GetServerValueByFactor(uGid);

		//		//���ݷ��������ֵ�����serverId
		//		uint32_t uServerId = TBaseDaemonClientW::GetServerIdByValue(uServerValue, SUFFIX_GTRANS, ENUM_HASH_SERVERIP);

		//		//���͵��������gtransferId
		//		if (uServerId != (uint32_t)(-1))
		//			routeS(uServerId, Uri, obj);
		//		return true;
		//	}

		//	bool RouteGT(std::string &strFrom, uint32_t uGid, uint32_t Uri, const std::string &strPacket)
		//	{
		//		CHashServerValueHelper *pHashHelper = TBaseDaemonClientW::GetHashHelper(SUFFIX_GTRANS);
		//		if (pHashHelper == NULL)
		//			return false;

		//		//�������Ӽ�����������ֵ������ΪIP
		//		uint32_t uServerValue = pHashHelper->GetServerValueByFactor(uGid);

		//		//���ݷ��������ֵ�����serverId
		//		uint32_t uServerId = TBaseDaemonClientW::GetServerIdByValue(uServerValue, SUFFIX_GTRANS, ENUM_HASH_SERVERIP);

		//		//���͵��������gtransferId
		//		if (uServerId != (uint32_t)(-1))
		//			routeS(strFrom, uServerId, Uri, strPacket);

		//		return true;
		//	}

		//	//���ܣ����㲥������hash(gid) -> gtransfer
		//	//������uGidΪȺ/��ID
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

		//	//���ܣ���ת��������hash(gid) -> gtransfer
		//	//������uGidΪȺ/��ID�����uGid=-1�������ѡ��һ��gtransfer
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

		//	//���ܣ���ת��������hash(gid) -> gtransfer
		//	//������uGidΪȺ/��ID�����uGid=-1�������ѡ��һ��gtransfer
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
