#pragma once

#include "common/packet.h" // for Marshallable

namespace protocol
{
	namespace gsearch
	{
		struct CGSearchRecordData : public sox::Marshallable
		{
			CGSearchRecordData() : m_uGID(0), m_uGOwner(0), m_uGAuthMode(0), m_strGName("")
			{
			}
					
			virtual void marshal(sox::Pack &p) const {
				p << m_uGID << m_uGOwner << m_uGAuthMode << m_strGName;
			}
					
			virtual void unmarshal(const sox::Unpack &p) {
				p >> m_uGID >> m_uGOwner >> m_uGAuthMode >> m_strGName;
			}
					
			uint32_t m_uGID;//group id
			uint32_t m_uGOwner;//group owner
			uint32_t m_uGAuthMode;//authentication mode
			std::string m_strGName;//group name
		};

	}
}

