 #pragma once

#include <vector>
#include <string>

#include "iproperty.h"
#include "protocol/const.h"
#include "core/base_svid.h"

#include "common/protocol/pgofflinemsg.h"
#include "common/protocol/pimcommon.h"

using namespace std;

namespace protocol
{
	namespace gofflinemsg
	{
	//本文件描述的消息定义，是服务端和服务器交互的消息定义
		struct PSS_SaveGOffLineMsg : public sox::Marshallable 
		{
			enum {uri = (5 << 8) | protocol::GOFFLINEMSG_SVID};
			PSS_SaveGOffLineMsg(){}

			CGofflineMsgData m_objData;
			uint32_t m_uSeqId;//客户端发送点对点消息时候提供的流水号


			virtual void marshal(sox::Pack &p) const
			{
				m_objData.marshal(p);
				p << m_uSeqId;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				m_objData.unmarshal(p);
				p >> m_uSeqId;
			}
		};
		
	}
}
