 #pragma once

#include <vector>
#include <string>

#include "iproperty.h"
#include "protocol/const.h"
#include "core/base_svid.h"
//#include "./pverifycodepng.h"
//#include "common/protocol/iImV2.h"
#include "pimdb.h"
#include "pimcommon.h"

using namespace std;

namespace protocol
{
	namespace im
	{
	//本文件描述的消息定义，是客户端和服务器交互的消息定义

		//从imgetimid服务能够拿到的数据		
		struct 		PCS_GetMyImid : public sox::Voidmable
		{
			enum {uri = (1 << 8) | protocol::IMGETIMID_SVID};

		};		

		
		struct 		PCS_GetMyImidRes : public protocol::im::PCS_CommonHead
		{
			enum {uri = (2 << 8) | protocol::IMGETIMID_SVID};
			PCS_GetMyImidRes(){}

			uint32_t m_uImId;

			virtual void marshal(sox::Pack &p) const 
			{
				PCS_CommonHead::marshal(p);
				p << m_uImId;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				PCS_CommonHead::unmarshal(p);
				p >> m_uImId;
			}
		};		



		struct PCS_GetImidByList : public sox::Marshallable 
		{
			enum {uri = (3 << 8) | protocol::IMGETIMID_SVID};
			PCS_GetImidByList(){}

			vector<uint32_t> m_vecUid;//好友的UID列表

			virtual void marshal(sox::Pack &p) const
			{
				marshal_container(p, m_vecUid);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				unmarshal_container(p, std::back_inserter(m_vecUid));
			}
		};

		struct PCS_GetImidByListRes : public PCS_CommonHead
		{
			enum {uri = (4 << 8) | protocol::IMGETIMID_SVID};
			PCS_GetImidByListRes(){}

			map<uint32_t, uint32_t> m_mapUidToImid;

			virtual void marshal(sox::Pack &p) const
			{
                marshal_container(p, m_mapUidToImid);
				
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
                unmarshal_container(p, std::inserter(m_mapUidToImid, m_mapUidToImid.begin()));

			}
		};
		
        //new for buddy imid
        struct PCS_GetBuddyImId: public PCS_GetBuddyImIdReq
        {
            enum {uri = (202 << 8) | protocol::IMGETIMID_SVID};
        };
	} //end im
}
