 #pragma once

#include <vector>
#include <string>

#include "iproperty.h"
#include "protocol/const.h"
#include "core/base_svid.h"
//#include "./pverifycodepng.h"
#include "common/protocol/pgoffline.h"
#include "common/protocol/pimcommon.h"
#include "protocol/common/protocol/PGTransfer.h"

using namespace std;
using namespace protocol::gtransfer;

namespace protocol
{
	namespace goffline
	{
	//本文件描述的消息定义，是服务端和服务器交互的消息定义
		struct PSS_SetOfflineAction : public sox::Marshallable 
		{
			enum {uri = (4 << 8) | protocol::GOFFLINE_SVID};
			PSS_SetOfflineAction()
			{
				gid = 0;
				fid = 0;
				actionMod = 0;
			}

			CGofflineActionMultiUsersData m_objData;
			uint32_t gid;
			uint32_t fid;
			uint32_t actionMod;
			//if actionMod.b.TO_ADM_INFLD=1,need the fld usr list
			vector<uint32_t> uidList;

			virtual void marshal(sox::Pack &p) const
			{
				m_objData.marshal(p);
				p <<gid<<fid<<actionMod;
				marshal_container(p,uidList);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				m_objData.unmarshal(p);
				p >>gid>>fid>>actionMod;
				unmarshal_container(p, std::back_inserter(uidList));
			}
		};


		struct PSS_SetOfflineActionRes : public protocol::im::PCS_CommonHead 
		{
			enum {uri = (5 << 8) | protocol::GOFFLINE_SVID};
			PSS_SetOfflineActionRes(){}

			virtual void marshal(sox::Pack &p) const
			{
				protocol::im::PCS_CommonHead::marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				protocol::im::PCS_CommonHead::unmarshal(p);

			}
		};



		
	}
}
