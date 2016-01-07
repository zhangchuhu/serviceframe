#pragma once

#include "ImHashServer.h"

#include "common/boost/thread/mutex.hpp"

namespace server
{
	namespace im
	{

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//ͨ�÷�������ϣ������

		//���̰߳汾

		class CRHashServerValueHelper : public CHashServerValueHelper
		{
		public:

			virtual ~CRHashServerValueHelper() {};

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
			boost::mutex m_mutexValue;
		};

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//��������ϣ����

		//���̰߳汾

		class CRBaseDaemonClientW : public CTBaseDaemonClientW<CRHashServerValueHelper>
		{
			boost::mutex m_mutexServers;

		protected:

			virtual void ClearMapServers()
			{
				boost::mutex::scoped_lock sl(m_mutexServers);

				CTBaseDaemonClientW<CRHashServerValueHelper>::ClearMapServers();
			}

			virtual void AddMapServer(const sdaemon::FavorStrSV & fs)
			{
				boost::mutex::scoped_lock sl(m_mutexServers);

				CTBaseDaemonClientW<CRHashServerValueHelper>::AddMapServer(fs);
			}

			virtual void RemoveMapServer(const sdaemon::FavorStrSV & fs)
			{
				boost::mutex::scoped_lock sl(m_mutexServers);

				CTBaseDaemonClientW<CRHashServerValueHelper>::RemoveMapServer(fs);
			}

			virtual uint32_t GetServerIdByValue(uint32_t uServerValue, const std::string &strServerName, EHashServerValueType emHashType)
			{
				boost::mutex::scoped_lock sl(m_mutexServers);

				return CTBaseDaemonClientW<CRHashServerValueHelper>::GetServerIdByValue(uServerValue, strServerName, emHashType);
			}
		};

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//gtransfer��������ϣ������
		//��������������gtransfer����ʹ��

		//���̰߳汾

		//typedef CTGTransDaemonClientW<CRBaseDaemonClientW>  CRGTransDaemonClientW;

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}
