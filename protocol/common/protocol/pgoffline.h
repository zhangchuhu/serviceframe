 #pragma once

#include <vector>
#include <string>

#include "iproperty.h"
#include "protocol/const.h"
#include "core/base_svid.h"
//#include "./pverifycodepng.h"
//#include "common/protocol/iImV2.h"
#include "pimcommon.h"

using namespace std;

namespace protocol
{
	namespace goffline
	{
		// struct for gtransfer to send offline action to goffline
		struct CGofflineActionMultiUsersData:public sox::Marshallable 
		{

		public:
			CGofflineActionMultiUsersData()
			{

			}

			CGofflineActionMultiUsersData(uint32_t uSendUid,const std::vector<uint32_t>& vec,uint32_t uType,uint32_t uLength,const string& strBody)
			{
				m_uSendUid=uSendUid;//发送人
				m_vecRecverList=vec;//接收人
				m_uType=uType;//消息类型
				m_uLength=uLength;//消息长度
				m_strBody=strBody;//消息体

			}

			uint32_t m_uSendUid;//发送人
			std::vector<uint32_t> m_vecRecverList;//接收人
			uint32_t m_uType;//消息类型
			uint32_t m_uLength;//消息长度
			string m_strBody;//消息体

		public:
			virtual void marshal(sox::Pack &p) const 
			{
				p<<m_uSendUid<<m_uType<<m_uLength;
				p.push_varstr32(m_strBody.data(), m_strBody.length());
				sox::marshal_container(p, m_vecRecverList);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p>>m_uSendUid>>m_uType>>m_uLength;				
				m_strBody = p.pop_varstr32();
				sox::unmarshal_container(p, std::back_inserter(m_vecRecverList));
			}

		};

		//本文件描述的消息定义，是客户端和服务器交互的消息定义
		
		//取离线操作消息
		struct PCS_GetOfflineAction2 : public sox::Voidmable
		{
			enum {uri = (1 << 8) | protocol::GOFFLINE_SVID};
			
		};

		struct PCS_GetOfflineAction : public sox::Voidmable
		{
			enum {uri = (11 << 8) | protocol::GOFFLINE_SVID};

		};


		// struct for goffline to reply offline action to client 
		struct CGofflineActionData:public sox::Marshallable 
		{

		public:
			CGofflineActionData()
			{

			}

			CGofflineActionData(uint32_t uSendUid,uint32_t uRecieveUid,uint32_t uType,uint32_t uLength,const string& strBody)
			{
				m_uSendUid=uSendUid;//发送人
				m_uReceiveUid=uRecieveUid;//接收人
				m_uType=uType;//消息类型
				m_uLength=uLength;//消息长度
				m_strBody=strBody;//消息体

			}

			uint32_t m_uSendUid;//发送人
			uint32_t m_uReceiveUid;//接收人
			uint32_t m_uType;//消息类型
			uint32_t m_uLength;//消息长度
			string m_strBody;//消息体

		public:
			virtual void marshal(sox::Pack &p) const 
			{
				p<<m_uSendUid<<m_uReceiveUid<<m_uType<<m_uLength;//<<m_strBody;
				p.push_varstr32(m_strBody.data(), m_strBody.length());
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p>>m_uSendUid>>m_uReceiveUid>>m_uType>>m_uLength;//>>m_strBody;		
				m_strBody = p.pop_varstr32();		
			}

		};
		
		//PCS_GetOfflineAction的响应
		struct PCS_GetOfflineActionRes : public protocol::im::PCS_CommonHead 
		{
			enum {uri = (2 << 8) | protocol::GOFFLINE_SVID};
			PCS_GetOfflineActionRes(){}

			vector<CGofflineActionData> m_vecData;

			virtual void marshal(sox::Pack &p) const
			{
				protocol::im::PCS_CommonHead::marshal(p);
				marshal_container(p, m_vecData);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				protocol::im::PCS_CommonHead::unmarshal(p);
				unmarshal_container(p, std::back_inserter(m_vecData));
			}
		};

		// struct for goffline to reply offline action to client 
		struct CGofflineActionData2:public sox::Marshallable 
		{

		public:
			CGofflineActionData2()
			{

			}

			CGofflineActionData2(uint32_t uSendUid,uint32_t uRecieveUid,uint32_t uType,uint32_t uLength,const string& strBody)
			{
				m_uSendUid=uSendUid;//发送人
				m_uReceiveUid=uRecieveUid;//接收人
				m_uType=uType;//消息类型
				m_uLength=uLength;//消息长度
				m_strBody=strBody;//消息体

			}

			uint32_t m_uSendUid;//发送人
			uint32_t m_uReceiveUid;//接收人
			uint32_t m_uType;//消息类型
			uint32_t m_uLength;//消息长度
			string m_strBody;//消息体

		public:
			virtual void marshal(sox::Pack &p) const 
			{
				p<<m_uSendUid<<m_uReceiveUid<<m_uType<<m_uLength<<m_strBody;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p>>m_uSendUid>>m_uReceiveUid>>m_uType>>m_uLength>>m_strBody;				
			}

		};

		//PCS_GetOfflineAction的响应
		struct PCS_GetOfflineActionRes2 : public protocol::im::PCS_CommonHead 
		{
			enum {uri = (2 << 8) | protocol::GOFFLINE_SVID};
			PCS_GetOfflineActionRes2(){}

			vector<CGofflineActionData2> m_vecData;

			virtual void marshal(sox::Pack &p) const
			{
				protocol::im::PCS_CommonHead::marshal(p);
				marshal_container(p, m_vecData);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				protocol::im::PCS_CommonHead::unmarshal(p);
				unmarshal_container(p, std::back_inserter(m_vecData));
			}
		};
		//删除离线操作消息
		struct PCS_DelOfflineAction : public sox::Voidmable
		{
			enum {uri = (3 << 8) | protocol::GOFFLINE_SVID};

		};

		//2.0 -> 3.0的兼容存离线消息

		struct PCS_SaveOfflineAction : public sox::Marshallable
		{
			enum {uri = (6 << 8) | protocol::GOFFLINE_SVID};

			uint32_t	m_uSendUid;//发送人
			uint32_t	m_uRecverUid;//接收人
			uint32_t	m_uType;//消息类型
			uint32_t	m_uLength;//消息长度
			std::string	m_strBody;//消息体

			PCS_SaveOfflineAction() {};
			PCS_SaveOfflineAction(uint32_t uSendUid, uint32_t uRecverUid, uint32_t uType, uint32_t uLength, const std::string &strBody):
			m_uSendUid(uSendUid), m_uRecverUid(uRecverUid), m_uType(uType), m_uLength(uLength), m_strBody(strBody) {};

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uSendUid << m_uRecverUid << m_uType << m_uLength << m_strBody;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uSendUid >> m_uRecverUid >> m_uType >> m_uLength >> m_strBody;
			}
		};
	}
}

