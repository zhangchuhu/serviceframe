#ifndef PROTOCOL_IM_ONLINE_BRIDGE_H_INCLUDE
#define PROTOCOL_IM_ONLINE_BRIDGE_H_INCLUDE

#include "int_types.h"
#include "core/base_svid.h"
#include "protocol/const.h"
#include "packet.h"

#include <sys/time.h>
#include <vector>

namespace server
{
	namespace imonbridge
	{
		struct PSS_OnBridgeBroadcast : public sox::Marshallable
		{
			enum {uri = (1 << 8 | protocol::IMONBRIDGE_SVID)};

			PSS_OnBridgeBroadcast() 
				: m_uUid(0), m_uFromOnlineGrp(-1), m_uOriUri(0){}
			PSS_OnBridgeBroadcast(uint32_t id, uint32_t fromGrp, uint32_t uri, const std::string& str)
				: m_uUid(id), m_uFromOnlineGrp(fromGrp), m_uOriUri(uri), m_strOri(str) {}

			uint32_t     m_uUid;
			uint32_t     m_uFromOnlineGrp;
			uint32_t     m_uOriUri;
			std::string  m_strOri;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid << m_uFromOnlineGrp << m_uOriUri << m_strOri;
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid >> m_uFromOnlineGrp >> m_uOriUri >> m_strOri;
			}
		};
	}
}


#endif
