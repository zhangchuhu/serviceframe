#pragma once 

#include <map>
#include <string>
#include <time.h>

#include "core/base_svid.h"
#include "protocol/const.h"
#include "common/protocol/pimcommon.h"
#include "common/protocol/pimdb.h"

namespace server
{
	namespace im
	{

		//imcache��uinfo���ֲ�֣���Ϊimcache��imuinfo������ֵ�Э��
		struct PSS_GetUserPoints : public sox::Marshallable
		{
			enum {uri = (101 << 8) | protocol::IMUINFO_SVID};

			uint32_t m_uUId;		// 查看人Id
			uint32_t m_uPeekUId;	// 被查看人Id
			uint32_t m_uImlinkdId;	// 用户数据传输链路入口

			PSS_GetUserPoints() : m_uUId(-1), m_uPeekUId(-1), m_uImlinkdId(-1)
			{
			}

			PSS_GetUserPoints(uint32_t uUId, uint32_t uPeekUId, uint32_t uImlinkdId) 
				:	m_uUId(uUId), m_uPeekUId(uPeekUId), m_uImlinkdId(uImlinkdId)
			{
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUId << m_uPeekUId << m_uImlinkdId;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUId >> m_uPeekUId >> m_uImlinkdId;
			}
		};

		//imcache��uinfo���ֲ�֣���Ϊimuinfo��imcache���ػ��ֵ�Э��
		struct PSS_GetUserPointsRes : public sox::Marshallable 
		{
			enum {uri = (102 << 8) | protocol::IMCACHE_SVID};

			PSS_GetUserPointsRes()
				:	m_uUId(-1), m_uPeekUId(-1), m_uImlinkdId(-1), m_uPoints(0)
			{
			}

			PSS_GetUserPointsRes(uint32_t uUId, uint32_t uPeekUId, 
				uint32_t uImlinkdId, uint32_t uPoints)
				:	m_uUId(uUId), m_uPeekUId(uPeekUId), 
				m_uImlinkdId(uImlinkdId), 
				m_uPoints(uPoints)
			{
			}

			uint32_t m_uUId;
			uint32_t m_uPeekUId;
			uint32_t m_uImlinkdId;
			uint32_t m_uPoints;

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uUId << m_uPeekUId << m_uImlinkdId << m_uPoints;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uUId >> m_uPeekUId >> m_uImlinkdId >> m_uPoints;
			}
		};

	}
}
