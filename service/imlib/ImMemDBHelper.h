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
		static const time_t EXPIRATION_TIME = 0; //永不过期


		//////////////////////////////////////////////////////////////////////////////////
		//用户在线信息，仅用于本帮助类Memcached存储

		struct CUserOnlineInfo : public sox::Marshallable
		{
			//由于并不用于网络发送，所以没有定义uri

			uint32_t m_Uid;
			uint32_t m_uImLinkdId;
			protocol::E_IMSTATUS m_emStatus;

			CUserOnlineInfo() 
			{
				m_Uid = -1;
				m_uImLinkdId = -1;
				m_emStatus = protocol::IMOFFLINE; //预防memcached获取数据失败时，返回了未赋值的状态
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
		//IM系统的memcached DB操作封装类

		class CImMemDBHelper :
			public mc::MemcachedFactoryAware
		{
			boost::mutex m_mutex;
			//boost::mutex::scoped_lock s1(m_mutexLinkd);

		public:
			CImMemDBHelper() {};

			//设置用户在线状态、所在ImLinkdId

			bool SetUserOnlineInfo(uint32_t Uid, const protocol::E_IMSTATUS &emStatus, uint32_t uImLinkdId);
			bool SetUserOnlineInfo(uint32_t Uid, CUserOnlineInfo &stUserOnline);

			//获取用户在线状态、所在ImLinkdId

			bool GetUserOnlineInfo(uint32_t Uid, protocol::E_IMSTATUS &emStatus, uint32_t &uImLinkdId);
			bool GetUserOnlineInfo(uint32_t Uid, CUserOnlineInfo &stUserOnline);
			bool GetUserOnlineInfo(uint32_t Uid, uint32_t &uImLinkdId);

			//擦除用户在线状态

			bool EraseUserOnlineInfo(uint32_t Uid);

			//重置Online memcached

			bool ClearAllOnlineInfo();

			//批量设置用户在线状态、所在ImLinkdId

			bool BatchSetUserOnlineInfo(std::vector<CUserOnlineInfo> &vecUserOnline);

			//批量获取用户在线状态、所在ImLinkdId

			bool BatchGetUserOnlineInfo(const std::vector<uint32_t> &vecUid, std::vector<CUserOnlineInfo> &vecUserOnline);

			//批量擦除用户在线状态、所在ImLinkdId

			bool BatchEraseUserOnlineInfo(const std::vector<uint32_t> &vecUid);

		private:

			//设置/获取缓存的用户在线信息（非线程安全）

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
