#ifndef __IMIDUPDATE_H__
#define __IMIDUPDATE_H__



//以下3个文件是必须的文件
// #include "common/core/base_svid.h"///*protocol::WUJI_SIMPLE_SVID*/定义放的地方
// #include "common/packet.h"//sox::Marshallable的定义
// #include "common/protocol/const.h"

#include "core/base_svid.h"///*protocol::WUJI_SIMPLE_SVID*/定义放的地方
#include "packet.h"//sox::Marshallable的定义
#include "protocol/const.h"


using namespace std;

namespace protocol
{
	namespace im
	{
		struct PSS_ImIdUpdateMsg : public sox::Marshallable 
		{
			enum {uri = (1 << 8) | protocol::IMIDUPDATE_SVID};

			PSS_ImIdUpdateMsg(){}
			PSS_ImIdUpdateMsg(uint32_t uUid, uint32_t uImId)
				: m_uUid(uUid),m_uImId(uImId)
			{}

			uint32_t m_uUid; //user id
			uint32_t m_uImId; //im id
			
			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid << m_uImId ;
			}
			
			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uUid >> m_uImId ;
			}
		};
		
	}
}

#endif

