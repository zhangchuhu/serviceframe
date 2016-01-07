#ifndef IM_SERVER_PROTOCOL_IMONLINE_INFO_H
#define IM_SERVER_PROTOCOL_IMONLINE_INFO_H

#include "int_types.h"
#include "core/base_svid.h"
#include "protocol/const.h"
#include "packet.h"

namespace server
{
	namespace imonlineinfo
	{
		enum EImUserLoginType
		{
			ENUM_IMLOGIN_NOMAL=0,    // 普通登录方式（就近接入）
			ENUM_IMLOGIN_FORCE       // 强制登陆方式
		};

		struct PSS_UserLoginData : public sox::Marshallable
		{
			enum {uri = (172 << 8) | protocol::IMLINKD_SVID};

			PSS_UserLoginData() {}
			PSS_UserLoginData(EImUserLoginType emType, uint32_t uUid, uint32_t uUserIp, uint32_t uLinkdIp)
				: m_emType(emType), m_uUid(uUid), m_uUserIp(uUserIp), m_uLinkdIp(uLinkdIp)
			{
			}

			EImUserLoginType m_emType;
			uint32_t         m_uUid;
			uint32_t         m_uUserIp;
			uint32_t         m_uLinkdIp;

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uUid << m_uUserIp << m_uLinkdIp;
				p.push_uint16(m_emType);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uUid >> m_uUserIp >> m_uLinkdIp;
				m_emType = (EImUserLoginType)p.pop_uint16();
			}
		};

		//
		struct PSS_UserLoginInfo : public sox::Marshallable
		{
			enum {uri = (173 << 8) | protocol::IMLINKD_SVID};
			
			PSS_UserLoginInfo() {}
			PSS_UserLoginInfo(uint32_t uUid, uint32_t uUserIp, protocol::E_IMSTATUS emSt)
				: m_uUid(uUid), m_uUserIp(uUserIp), m_emStatus(emSt)
			{
			}

			uint32_t                   m_uUid;
			uint32_t                   m_uUserIp;     // network bit-order
			protocol::E_IMSTATUS       m_emStatus;
		
			virtual void marshal(sox::Pack &p) const 
			{
				uint32_t uLen = sizeof(uint32_t) * 3 + sizeof(uint16_t);
				p << uLen << m_uUid << m_uUserIp;
				p.push_uint16(m_emStatus);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				uint32_t uLen;
				p >> uLen >> m_uUid >> m_uUserIp;
				m_emStatus = (protocol::E_IMSTATUS)p.pop_uint16();
			}
		};

		// 常用ip登陆协议，非客户端登陆的类型

		struct PSS_UserLoginInfoEx : public sox::Marshallable
		{
			enum {uri = (174 << 8) | protocol::IMLINKD_SVID};

			uint32_t        m_uUid;
			uint32_t        m_uIp;            // 登陆ip
			uint32_t        m_uTime;          // 登陆时间
			std::string     m_strType;        // 登陆类型
			
			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uUid << m_uIp << m_uTime << m_strType;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uUid >> m_uIp >> m_uTime >> m_strType;
			}
		};
	}
}

#endif
