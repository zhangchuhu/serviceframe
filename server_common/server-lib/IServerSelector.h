#pragma once 

#include "common/int_types.h"
#include "common/protocol/const.h"
#include "server_common/protocol/daemon.h"

#include "ConsistentHash.h"

#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>

namespace server
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	const uint32_t NET_TYPE_ANY = AUTO_DETECT;
	const uint32_t NET_TYPE_CTL = CTL;
	const uint32_t NET_TYPE_CNC = CNC;
	const uint32_t NET_TYPE_DOUBLE = (CNC | CTL);
	const uint32_t NET_TYPE_NONE = -1;

	class IServerContainer
	{
	public:

		//----------------------------------------------------------------------------------------------------------------------------
		//����Ϊ��ȡָ���ķ�����ID����

		virtual std::set<uint32_t> GetServerIds(const std::string &strSuffix) = 0;
		virtual std::set<uint32_t> GetServerIdsByNet(const std::string &strSuffix, uint32_t uNetType = NET_TYPE_ANY) = 0;
		virtual std::set<uint32_t> GetServerIdsByGroup(const std::string &strSuffix, uint32_t uGroupId) = 0;
		virtual std::set<uint32_t> GetServerIdsByNetGroup(const std::string &strSuffix, uint32_t uNetType, uint32_t uGroupId) = 0;
		virtual std::set<uint32_t> GetServerIdsByIp(const std::string &strSuffix, uint32_t uIp) = 0;

		//----------------------------------------------------------------------------------------------------------------------------
		//�����ṩIP-ID��ת��
		//����IP������ʱ��Ӧ��֤ÿ�����ͽ���ÿ̨����ֻ����һ��

		virtual uint32_t ServerIpToId(const std::string &strSuffix, uint32_t uServerIp, std::set<uint32_t> &setServerIds) = 0;
		virtual uint32_t ServerIdToIp(const std::string &strSuffix, uint32_t uServerId) = 0;
	};

	class IServerContainerAware
	{
	protected:
		IServerContainer *m_pServerContainer;

	public:
		IServerContainerAware() : m_pServerContainer(NULL) {};

		void SetServerContainer(IServerContainer *pServerContainer)
		{
			m_pServerContainer = pServerContainer;
		}

		IServerContainer *GetServerContainer()
		{
			return m_pServerContainer;
		}

	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//��������ϣ����

	enum EHashServerValueType
	{
		ENUM_HASH_SERVERID = 0, //��serverId����ϣ���ֵ
		ENUM_HASH_SERVERIP, //��ServerIp����ϣ���ֵ
		ENUM_HASH_SERVERIPID
	};

	//Watcher�ӿ�

	struct IServerSelectorWatcher
	{
		virtual ~IServerSelectorWatcher() {};

		virtual void OnServerInit() {}; //��������ʹ��ʱ��Watcher�ýӿڣ���Ҫע������
		virtual void OnServerAddjust(const std::string &strSuffix) {}; //����������ʱ��Watcher�ýӿڣ���Ҫע������
		virtual void OnServerAddjust2(const sdaemon::FavorStrSV & fs) {}; //����������ʱ��Watcher�ýӿڣ���Ҫע������
	};

	//Watcher Base

	struct IServerSelctorWatcherBase
	{
		virtual ~IServerSelctorWatcherBase() {};

		virtual void watch(IServerSelectorWatcher *w) = 0;
		virtual void revoke(IServerSelectorWatcher *w) = 0;
	};

	//ͨ�÷�������ӿ�

	class IServerSelector : public virtual IServerSelctorWatcherBase
	{
	public:

		virtual ~IServerSelector() {};

		//----------------------------------------------------------------------------------------------------------------------------
		//����Ϊע�����������

		virtual void RegisterServerSuffix(const std::string &strSuffix, EHashServerValueType emValueType = ENUM_HASH_SERVERID, EHashFuntcionType emFuncType = ENUM_FUNC_MD5) = 0;

		//----------------------------------------------------------------------------------------------------------------------------
		//����Ϊͨ��Key����һ���Թ�ϣ��ķ�����ID

		virtual uint32_t HashServer(uint32_t uKey, const std::string &strSuffix) = 0;
		virtual bool HashMultiServer(uint32_t uKey, uint32_t uValueCount, std::vector<uint32_t>& vecValues, const std::string &strSuffix) = 0;
		virtual uint32_t HashServerByNet(uint32_t uKey, const std::string &strSuffix, uint32_t uNetType = NET_TYPE_ANY) = 0;
		virtual uint32_t HashServerByGroup(uint32_t uKey, const std::string &strSuffix, uint32_t uGroupId) = 0;
		virtual uint32_t HashServerByNetGroup(uint32_t uKey, const std::string &strSuffix, uint32_t uNetType, uint32_t uGroupId) = 0;
		virtual uint32_t HashServerByIp(uint32_t uKey, const std::string &strSuffix, uint32_t uIp) = 0;

		//----------------------------------------------------------------------------------------------------------------------------
		//����Ϊ���ѡ��һ��������

		virtual uint32_t RandomServer(const std::string &strSuffix) = 0;
		virtual uint32_t RandomServerByNet(const std::string &strSuffix, uint32_t uNetType = NET_TYPE_ANY) = 0;
		virtual uint32_t RandomServerByGroup(const std::string &strSuffix, uint32_t uGroupId) = 0;
		virtual uint32_t RandomServerByNetGroup(const std::string &strSuffix, uint32_t uNetType, uint32_t uGroupId) = 0;
	};

	class IServerSelectorAware
	{
	protected:
		IServerSelector *m_pServerSelector;

	public:
		IServerSelectorAware() : m_pServerSelector(NULL) {};

		void SetServerSelector(IServerSelector *pServerSelector)
		{
			m_pServerSelector = pServerSelector;
		}

		IServerSelector *GetServerSelector()
		{
			return m_pServerSelector;
		}
	};

	class IServerSelectorAwareW : public IServerSelectorWatcher
	{
	protected:
		IServerSelector *m_pServerSelector;

	public:
		IServerSelectorAwareW() : m_pServerSelector(NULL) {};

		virtual void SetServerSelector(IServerSelector *pServerSelector)
		{
			m_pServerSelector = pServerSelector;
			m_pServerSelector->watch(this);
		}

		virtual IServerSelector *GetServerSelector()
		{
			return m_pServerSelector;
		}
	};

}
