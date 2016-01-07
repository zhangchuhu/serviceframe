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

		//imcache中uinfo积分拆分，此为imcache向imuinfo请求积分的协议
		struct PSS_GetUserPoints : public sox::Marshallable
		{
			enum {uri = (101 << 8) | protocol::IMUINFO_SVID};

			uint32_t m_uUId;		// 鏌ョ湅浜篒d
			uint32_t m_uPeekUId;	// 琚煡鐪嬩汉Id
			uint32_t m_uImlinkdId;	// 鐢ㄦ埛鏁版嵁浼犺緭閾捐矾鍏ュ彛

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

		//imcache中uinfo积分拆分，此为imuinfo向imcache返回积分的协议
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
