#ifndef IMLIB_MEMDBHELPER_H_
#define IMLIB_MEMDBHELPER_H_

#include "server_common/dblib/Memcached.h" //include Memcached
#include "server_common/dblib/IMemcachedFactory.h" //include MemcachedFactoryAware

#include "common/protocol/const.h" //include protocol::E_IMSTATUS
#include "common/boost/thread/mutex.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <set>

namespace server
{
	namespace im
	{
		static const std::string IMONLINE_PREFIX = "io-";
		static const std::string IMONLINE_MEMDB = "imonline_memdb";
		static const time_t EXPIRATION_TIME = 0; //��������


		//////////////////////////////////////////////////////////////////////////////////
		//�û�������Ϣ�������ڱ�������Memcached�洢

		struct CUserOnlineInfo : public sox::Marshallable
		{
			//���ڲ����������緢�ͣ�����û�ж���uri

			uint32_t m_Uid;
			uint32_t m_uImLinkdId;
			protocol::E_IMSTATUS m_emStatus;

			CUserOnlineInfo() 
			{
				m_Uid = -1;
				m_uImLinkdId = -1;
				m_emStatus = protocol::IMOFFLINE; //Ԥ��memcached��ȡ����ʧ��ʱ��������δ��ֵ��״̬
			};
			CUserOnlineInfo(uint32_t Uid, uint32_t uImLinkdId, protocol::E_IMSTATUS emStatus)
			{
				m_Uid = Uid;
				m_uImLinkdId = uImLinkdId;
				m_emStatus = emStatus;
			}

			bool IsOnline()
			{
				return (protocol::IMONLINE == m_emStatus);
			}
			bool IsHide()
			{
				return (protocol::IMHIDE == m_emStatus);
			}
			bool IsNotOffline()
			{
				return ( !IsOffline() );
			}
			bool IsOffline()
			{
				return (protocol::IMOFFLINE == m_emStatus);
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_Uid;
				p << m_uImLinkdId;
				p.push_uint8(m_emStatus);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_Uid;
				p >> m_uImLinkdId;
				m_emStatus = (protocol::E_IMSTATUS)p.pop_uint8();
			}
		};

		//////////////////////////////////////////////////////////////////////////////////
		//IMϵͳ��memcached DB������װ��

		class CImMemDBHelper :
			public mc::MemcachedFactoryAware
		{
			boost::mutex m_mutex;
			//boost::mutex::scoped_lock s1(m_mutexLinkd);

		public:
			CImMemDBHelper() {};

			//�����û�����״̬������ImLinkdId

			bool SetUserOnlineInfo(uint32_t Uid, const protocol::E_IMSTATUS &emStatus, uint32_t uImLinkdId);
			bool SetUserOnlineInfo(uint32_t Uid, CUserOnlineInfo &stUserOnline);

			//��ȡ�û�����״̬������ImLinkdId

			bool GetUserOnlineInfo(uint32_t Uid, protocol::E_IMSTATUS &emStatus, uint32_t &uImLinkdId);
			bool GetUserOnlineInfo(uint32_t Uid, CUserOnlineInfo &stUserOnline);
			bool GetUserOnlineInfo(uint32_t Uid, uint32_t &uImLinkdId);

			//�����û�����״̬

			bool EraseUserOnlineInfo(uint32_t Uid);

			//����Online memcached

			bool ClearAllOnlineInfo();

			//���������û�����״̬������ImLinkdId

			bool BatchSetUserOnlineInfo(std::vector<CUserOnlineInfo> &vecUserOnline);

			//������ȡ�û�����״̬������ImLinkdId

			bool BatchGetUserOnlineInfo(const std::vector<uint32_t> &vecUid, std::vector<CUserOnlineInfo> &vecUserOnline);

			//���������û�����״̬������ImLinkdId

			bool BatchEraseUserOnlineInfo(const std::vector<uint32_t> &vecUid);

		private:

			//����/��ȡ������û�������Ϣ�����̰߳�ȫ��

			bool __SetUserOnlineToCache(uint32_t Uid, CUserOnlineInfo &stUserOnline);
			bool __GetUserOnlineFromCache(uint32_t Uid, CUserOnlineInfo &stUserOnline);
			bool __EraseUserOnlineInfo(uint32_t Uid);
			bool __ClearAllOnlineInfo();
			bool __BatchSetUserOnlineInfo(std::vector<CUserOnlineInfo> &vecUserOnline);
			bool __BatchGetUserOnlineInfo(const std::vector<uint32_t> &vecUid, std::vector<CUserOnlineInfo> &vecUserOnline);
			bool __BatchEraseUserOnlineInfo(const std::vector<uint32_t> &vecUid);

			std::string __Uid2Key(uint32_t Uid);
		};

	} //end im
} //end server

#endif
