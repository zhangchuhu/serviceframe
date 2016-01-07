#ifndef P_IM_OFFLINEMSG_H_SERVER_FILE
#define  P_IM_OFFLINEMSG_H_SERVER_FILE





#include "core/base_svid.h"///*protocol::WUJI_SIMPLE_SVID*/定义放的地方
#include "packet.h"//sox::Marshallable的定义
#include "protocol/const.h"


namespace protocol
{
	namespace im
	{

		//客户端向服务器上传离线消息
		struct PCS_OLMsgUpload : public sox::Marshallable 
		{
			enum {uri = (1 << 8) | protocol::IMOFFLINEMSG_SVID};
			PCS_OLMsgUpload (){}
			PCS_OLMsgUpload(uint32_t from,uint32_t to, const std::string& msg,uint32_t uSeqId )
				: m_uSrcId(from),m_uDstId(to),m_strMsgText(msg),m_uSeqId(uSeqId)
			{

			}


			uint32_t m_uSrcId; //
			uint32_t m_uDstId;
			std::string m_strMsgText;
			uint32_t m_uSeqId;


			virtual void marshal(sox::Pack &p) const
			{

				p << m_uSrcId<<m_uDstId<<m_uSeqId<<m_strMsgText;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{

				p >> m_uSrcId>>m_uDstId >>m_uSeqId>>m_strMsgText;
			}
		};


	
	}
}


#endif 
 
