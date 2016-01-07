#ifndef P_IM_CHAT_H
#define  P_IM_CHAT_H



//以下3个文件是必须的文件
// #include "common/core/base_svid.h"///*protocol::WUJI_SIMPLE_SVID*/定义放的地方
// #include "common/packet.h"//sox::Marshallable的定义
// #include "common/protocol/const.h"

#include "core/base_svid.h"///*protocol::WUJI_SIMPLE_SVID*/定义放的地方
#include "packet.h"//sox::Marshallable的定义
#include "protocol/const.h"

// #define PROTOCOL_IM_CHAT_NS_BEGIN	namespace protocol{ namespace imchat{
// #define PROTOCOL_IM_CHAT_NS_END	} }
// 
// #define USING_PROTOCOL_IM_CHAT_NS using namespace protocol::imchat;
// 
// 
// //如果接口文件放在server\protocol目录下，和放在server\我的目录下不知道有什么区别
// PROTOCOL_IM_CHAT_NS_BEGIN
// #define  WUJI_SIMPLE_SVID 24


using namespace std;

namespace protocol
{
	namespace im
	{
		
		// struct PCS_ChatMsg : public sox::Marshallable 
		// {
		// 	enum {uri = (6 << 8) | protocol::IMCHATD_SVID};
		// 	PCS_ChatMsg(){}
		// 	PCS_ChatMsg(uint32_t to, const std::string& msg)
		// 		: id(to), text(msg){}
		// 	
		// 	
		// 	uint32_t id; // 服务器发出去的时候是发送人ID，服务器接收的时候是接收人ID
		// 	std::string text;
		// 	
		// 	virtual void marshal(sox::Pack &p) const 
		// 	{
		// 		p << id << text;
		// 	}
		// 	
		// 	virtual void unmarshal(const sox::Unpack &p) 
		// 	{
		// 		p >> id >> text;
		// 	}
		// };
		
		
		
		struct PCS_ChatMsgInputStatus : public sox::Marshallable 
		{
			enum {uri = (1 << 8) | protocol::IMCHATD_SVID};
			PCS_ChatMsgInputStatus(){}
			PCS_ChatMsgInputStatus(uint32_t to)
				: id(to)
			{
				
			}
			uint32_t id; //dest id on sending , src id on recieving
			
			virtual void marshal(sox::Pack &p) const
			{
				p << id ;
			}
			
			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> id ;
			}
		};
		
		
		struct PCS_ChatMsgRes : public sox::Marshallable 
		{
			enum {uri = (2 << 8) | protocol::IMCHATD_SVID};
			PCS_ChatMsgRes(){}
			PCS_ChatMsgRes(uint32_t to, uint32_t uSeqId)
				: id(to),m_uSeqId(uSeqId)
			{
				
			}
			uint32_t id; //dest id on sending , src id on recieving
			uint32_t m_uSeqId;
			
			virtual void marshal(sox::Pack &p) const
			{
				p << id <<m_uSeqId;
			}
			
			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> id >>m_uSeqId;
			}
		};
		
		struct PCS_ChatMsg : public sox::Marshallable 
		{
			enum {uri = (3 << 8) | protocol::IMCHATD_SVID};
			PCS_ChatMsg(){}
			PCS_ChatMsg(uint32_t to, const std::string& msg,uint32_t uSeqId,uint32_t uSendTime,protocol::E_IMSTATUS status, bool bSenderBuddy)
				: id(to), text(msg),m_uSeqId(uSeqId),m_uSendTime(uSendTime), m_status(status), m_bSenderBuddy(bSenderBuddy)
			{

			}


			uint32_t id; //dest id on sending , src id on recieving
			std::string text;
			uint32_t m_uSeqId;
			uint32_t m_uSendTime;//UTC时间
			protocol::E_IMSTATUS m_status;	//登陆状态
			bool m_bSenderBuddy;				//是否是消息发送方的好友

			virtual void marshal(sox::Pack &p) const
			{
				p << id <<m_uSeqId<<m_uSendTime<<text;
				p.push_uint32(m_status);
				uint16_t senderBuddy = m_bSenderBuddy? 1: 0;
				p.push_uint16(senderBuddy);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> id >>m_uSeqId>>m_uSendTime>>text;
				m_status = static_cast<protocol::E_IMSTATUS>(p.pop_uint32());
				uint16_t senderBuddy = p.pop_uint16();
				m_bSenderBuddy = senderBuddy == 1 ? true: false;
			}
		};
		
		
		//这是直接从IMDB发送到client的消息
		struct PCS_StoreOfflineMsgRes : public sox::Marshallable 
		{
			PCS_StoreOfflineMsgRes(){}
			PCS_StoreOfflineMsgRes(uint32_t to, const std::string& msg, uint32_t err)
				: toid(to), text(msg), err_type(err){}
			
			enum {uri = (4 << 8) | protocol::IMCHATD_SVID};
			uint32_t toid;
			std::string text;
			uint32_t err_type;
			
			virtual void marshal(sox::Pack &p) const 
			{
				p << toid << text;
				p.push_uint32(err_type);
			}
			
			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> toid >> text;
				err_type = p.pop_uint32();
			}
		};
	}
}







//PROTOCOL_IM_CHAT_NS_END

#endif

