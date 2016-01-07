#ifndef _PIMONLINECACHEDB_H___
#define _PIMONLINECACHEDB_H___

#include "int_types.h"
#include "core/base_svid.h"
#include "protocol/const.h"
#include "packet.h"

#include <vector>

namespace server
{
	namespace imonlinecache
	{
		struct COnlineInfo : public sox::Marshallable
		{
			uint32_t m_uid;
			uint32_t m_linkdId;
			protocol::E_IMSTATUS m_status;

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uid << m_linkdId;
				p.push_uint8(m_status);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uid >> m_linkdId;
				m_status = (protocol::E_IMSTATUS)p.pop_uint8();
			}
		};


		/*
		* imonline启动时，要求imonline_cached更新所有用户状态
		*/
		struct PSS_NotifySyncAllUserStatus : public sox::Marshallable
		{
			enum {uri = (4 << 8 | protocol::IMONLINECACHE_SVID)};
			virtual void marshal(sox::Pack &p) const
			{
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
			}
		};
	}
}

#endif

