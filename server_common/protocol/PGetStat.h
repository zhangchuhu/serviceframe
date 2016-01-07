#pragma once 

#include "common/packet.h"
#include "common/core/base_svid.h"
#include "common/protocol/const.h"

namespace server
{
	namespace backstat
	{
		struct PSS_GetStatReq:public sox::Marshallable
		{
			enum {uri = (1 << 8 | protocol::BACKSTAT_SVID)};
			
			std::set<std::string> m_reqStrs;
			
			void addAttribute(const std::string& attributeDesc)
			{
			  m_reqStrs.insert(attributeDesc);
			}
			
			virtual void marshal(sox::Pack &p) const
			{
				marshal_container(p, m_reqStrs);
			}
			
			virtual void unmarshal(const sox::Unpack &p)
			{
				unmarshal_container(p, std::inserter(m_reqStrs,m_reqStrs.begin()));
			}

		};

		struct PSS_GetStatRes:public sox::Marshallable
		{
			enum {uri = (2 << 8 | protocol::BACKSTAT_SVID)};
			
			uint32_t m_serverId;
			std::map<std::string,uint32_t> m_UintMap;
			std::map<std::string,std::string> m_StringMap;
			
			virtual void marshal(sox::Pack &p) const
			{
				p << m_serverId;
				marshal_container(p, m_UintMap);
				marshal_container(p, m_StringMap);
			}
			
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_serverId;
				unmarshal_container(p, std::inserter(m_UintMap,m_UintMap.begin()));
				unmarshal_container(p, std::inserter(m_StringMap,m_StringMap.begin()));
			}

		};
		
	}
}
