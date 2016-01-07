#ifndef SERVER_PIMBRIDGE_H
#define SERVER_PIMBRIDGE_H

#include <vector>
#include <string>
#include "int_types.h"
#include "core/base_svid.h"
#include "protocol/const.h"
#include "packet.h"

namespace server{
	namespace imbridge{

		//// 用户信息，uid，linkdid
		//struct CUserUidLinkd : public sox::Marshallable
		//{
		//	uint32_t m_uid;
		//	uint32_t m_linkd;

		//	CUserUidLinkd(uint32_t uid, uint32_t linkd):
		//	m_uid(uid), m_linkd(linkd)
		//	{

		//	}

		//	virtual void marshal(sox::Pack &p) const
		//	{
		//		p << m_uid << m_linkd;
		//	}

		//	virtual void unmarshal(const sox::Unpack &p) 
		//	{
		//		p >> m_uid >> m_linkd;
		//	}
		//};

		//其它服务器请求imbridge转发的协议
		struct PSS_ForwardBridge : public sox::Marshallable
		{
			enum {uri = (12 << 8) | protocol::IMBRIDGE_SVID};

			PSS_ForwardBridge() {};
			PSS_ForwardBridge(uint32_t uri, uint32_t uid, uint32_t linkd, const std::string &strOriPacket) :
			m_ouri(uri), m_toUid(uid), m_toLinkd(linkd), m_strOriPacket(strOriPacket)
			{
			}
			
			uint32_t m_ouri; 
			uint32_t m_toUid;
			uint32_t m_toLinkd;
			std::string   m_strOriPacket; //原包数据

			virtual void marshal(sox::Pack &p) const
			{
				p << m_ouri << m_toUid << m_toLinkd;
				p << m_strOriPacket;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_ouri >> m_toUid >> m_toLinkd;
				p >> m_strOriPacket;
			}
		};

        struct PSS_ForwardToServer: public sox::Marshallable
        {
            enum {uri = (13 << 8) | protocol::IMBRIDGE_SVID};
            uint32_t m_uSvrId;
            uint32_t m_uUri;
            std::string m_strCtx;

            virtual void marshal(sox::Pack &p) const
            {
                p << m_uSvrId << m_uUri << m_strCtx;
            }

            virtual void unmarshal(const sox::Unpack &p)
            {
                p >> m_uSvrId >> m_uUri >> m_strCtx;
            }
        };

        struct PSS_ForwardToServerEx: public sox::Marshallable
        {
            enum {uri = (14 << 8) | protocol::IMBRIDGE_SVID};
            uint32_t m_uSvrId;
            uint32_t m_uUri;
            std::string m_strCtx;

            virtual void marshal(sox::Pack &p) const
            {
                p << m_uSvrId << m_uUri;
                p.push_varstr32(m_strCtx.data(), m_strCtx.length());
            }

            virtual void unmarshal(const sox::Unpack &p)
            {
                p >> m_uSvrId >> m_uUri;
                m_strCtx = p.pop_varstr32();
            }
        };
	}
}

#endif

