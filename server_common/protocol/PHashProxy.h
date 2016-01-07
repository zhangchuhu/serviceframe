#pragma once

#include "common/packet.h"
#include "common/core/base_svid.h"
#include "common/protocol/const.h"

#include <string>
#include <vector>
#include <deque>

using namespace protocol;

namespace server
{
	namespace hashproxy
	{
		struct PSS_ProxyRequest: public sox::Marshallable
		{
			enum {uri = (11 << 8) | HASHPROXY_SVID};

			uint32_t m_uTimeout;		//超时时间
			uint32_t m_uUsrId;			//用户id
			uint32_t m_uLinkId;			//用户所在imlink id

			std::vector<uint32_t> m_vecKey;
			std::deque<std::string> m_vecSuffix;		//目标server的suffix, 先发给第一个suffix；如果超时，发给下一个

			//上下文
			uint32_t m_uUri;
			std::string m_strPacket;

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uTimeout << m_uUsrId << m_uLinkId;
                marshal_container(p, m_vecKey);
				marshal_container(p, m_vecSuffix);
				p << m_uUri << m_strPacket;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uTimeout >> m_uUsrId >> m_uLinkId;
                unmarshal_container(p, std::back_inserter(m_vecKey));
				unmarshal_container(p, std::back_inserter(m_vecSuffix));
				p >> m_uUri >> m_strPacket;
			}
		};
	}
}
