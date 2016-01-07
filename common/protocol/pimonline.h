#ifndef _PIMONLINE_H_
#define _PIMONLINE_H_

#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "iproperty.h"
#include "protocol/const.h"
#include "core/base_svid.h"
#include "pimcommon.h"

namespace protocol
{
	namespace imonline
	{
		//请求获取全部好友的在线状态
		struct PCS_GetBuddyImOnlineReq  : public protocol::im::CUidSet
		{
			enum {uri = (8 << 8) | protocol::IMONLINE2_SVID};
			PCS_GetBuddyImOnlineReq(){}
		};

		//返回全部好友的Uinfo
		struct PCS_GetBuddyImOnlineRes  : public sox::Marshallable 
		{
			enum {uri = (9 << 8) | protocol::IMONLINE2_SVID};
			PCS_GetBuddyImOnlineRes(){}

			std::map<uint32_t, protocol::E_IMSTATUS> m_mapStatus;
			virtual void marshal(sox::Pack &p) const
			{
				p.push_uint32(m_mapStatus.size());
				for(std::map<uint32_t, protocol::E_IMSTATUS>::const_iterator iter = m_mapStatus.begin(); iter != m_mapStatus.end(); ++iter)
				{
					p.push_uint32(iter->first);
					p.push_uint16(iter->second);
				}
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				uint32_t size = p.pop_uint32();
				for(uint32_t i =0; i < size; ++i)
				{
					uint32_t bid = p.pop_uint32();
					protocol::E_IMSTATUS status = static_cast<protocol::E_IMSTATUS>(p.pop_uint16());
					m_mapStatus.insert(std::make_pair(bid, status));
				}
			}
		};

		//请求所有群用户的状态

		struct PCS_GetGUserStats  : public PCS_GetBuddyImOnlineReq 
		{
			enum {uri = (12 << 8) | protocol::IMONLINE2_SVID};
			PCS_GetGUserStats(){}

			virtual void marshal(sox::Pack &p) const
			{
				PCS_GetBuddyImOnlineReq::marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				PCS_GetBuddyImOnlineReq::unmarshal(p);
			}
		};

		//返回所有群用户的状态

		struct PCS_GetGUserStatsRes  : public PCS_GetBuddyImOnlineRes 
		{
			enum {uri = (13 << 8) | protocol::IMONLINE2_SVID};
			PCS_GetGUserStatsRes(){}

			virtual void marshal(sox::Pack &p) const
			{
				PCS_GetBuddyImOnlineRes::marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				PCS_GetBuddyImOnlineRes::unmarshal(p);
			}
		};

	}
}

#endif
