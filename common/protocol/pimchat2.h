#ifndef P_IM_CHAT2_H
#define  P_IM_CHAT2_H



//以下3个文件是必须的文件
// #include "common/core/base_svid.h"///*protocol::WUJI_SIMPLE_SVID*/定义放的地方
// #include "common/packet.h"//sox::Marshallable的定义
// #include "common/protocol/const.h"

#include "core/base_svid.h"///*protocol::WUJI_SIMPLE_SVID*/定义放的地方
#include "packet.h"//sox::Marshallable的定义
#include "protocol/const.h"
#include "common/protocol/Im_Const.h"

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
		struct PCS_ChatMsgInputStatus2 : public sox::Marshallable 
		{
			enum {uri = (1 << 8) | protocol::IMCHAT2D_SVID};
			PCS_ChatMsgInputStatus2(){}
			PCS_ChatMsgInputStatus2(uint32_t to)
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
		
		
		struct PCS_ChatMsgRes2 : public sox::Marshallable 
		{
			enum {uri = (2 << 8) | protocol::IMCHAT2D_SVID};
			PCS_ChatMsgRes2(){}
			PCS_ChatMsgRes2(uint32_t to, uint32_t uSeqId)
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
		
		struct PCS_ChatMsg2 : public sox::Marshallable 
		{
			enum {uri = (3 << 8) | protocol::IMCHAT2D_SVID};
			PCS_ChatMsg2(){}
			PCS_ChatMsg2(uint32_t to, const std::string& msg,uint32_t uSeqId,uint32_t uSendTime,protocol::E_IMSTATUS status, bool bSenderBuddy)
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
		struct PCS_StoreOfflineMsgRes2 : public sox::Marshallable 
		{
			PCS_StoreOfflineMsgRes2(){}
			PCS_StoreOfflineMsgRes2(uint32_t to, const std::string& msg, uint32_t err)
				: toid(to), text(msg), err_type(err){}
			
			enum {uri = (4 << 8) | protocol::IMCHAT2D_SVID};
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

		struct PSS_ChatMsgFromOffline : public sox::Marshallable 
		{
			enum {uri = (5 << 8) | protocol::IMCHAT2D_SVID};
			PSS_ChatMsgFromOffline(){}
			PSS_ChatMsgFromOffline(uint32_t uFromUid, uint32_t to, const std::string& msg,uint32_t uSeqId,uint32_t uSendTime,protocol::E_IMSTATUS status, bool bSenderBuddy)
				: id(to), from(uFromUid), text(msg),m_uSeqId(uSeqId),m_uSendTime(uSendTime), m_status(status), m_bSenderBuddy(bSenderBuddy)
			{

			}


			uint32_t id; //goal uid
			uint32_t from;//source uid
			std::string text;
			uint32_t m_uSeqId;
			uint32_t m_uSendTime;//UTC时间
			protocol::E_IMSTATUS m_status;	//登陆状态
			bool m_bSenderBuddy;				//是否是消息发送方的好友

			virtual void marshal(sox::Pack &p) const
			{
				p << id << from << m_uSeqId<<m_uSendTime<<text;
				p.push_uint32(m_status);
				uint16_t senderBuddy = m_bSenderBuddy? 1: 0;
				p.push_uint16(senderBuddy);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> id >>from >> m_uSeqId>>m_uSendTime>>text;
				m_status = static_cast<protocol::E_IMSTATUS>(p.pop_uint32());
				uint16_t senderBuddy = p.pop_uint16();
				m_bSenderBuddy = senderBuddy == 1 ? true: false;
			}
		};

		// 聊天消息的响应，在memcache中未找到用户，转发给imMakeUp，由其查询数据库，并发往用户
		struct PSS_MsgResRouteToMakeUp : public sox::Marshallable
		{
			enum {uri = (6 << 8) | protocol::IMCHAT2D_SVID};
			PSS_MsgResRouteToMakeUp(){}
			PSS_MsgResRouteToMakeUp(uint32_t id, uint32_t toId, uint32_t uSeqId)
				: m_id(id), m_toId(toId), m_uSeqId(uSeqId)
			{

			}
			uint32_t m_id;         // 响应消息的发送方
			uint32_t m_toId;       // 响应消息的接收方
			uint32_t m_uSeqId;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_id << m_toId <<m_uSeqId;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_id >> m_toId >>m_uSeqId;
			}
		};

		// ui重传消息
		struct PCS_ChatMsgRetry : public PCS_ChatMsg2
		{
			enum {uri = (7 << 8) | protocol::IMCHAT2D_SVID};

			PCS_ChatMsgRetry() {}
			PCS_ChatMsgRetry(uint32_t to, const std::string& msg,uint32_t uSeqId,uint32_t uSendTime,protocol::E_IMSTATUS status, bool bSenderBuddy)
				: PCS_ChatMsg2(to, msg, uSeqId, uSendTime, status, bSenderBuddy)
			{
			}

			uint8_t  m_flag;

			virtual void marshal(sox::Pack &p) const
			{
				p.push_uint8(m_flag);
				PCS_ChatMsg2::marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				m_flag = p.pop_uint8();
				PCS_ChatMsg2::unmarshal(p);
			}
		};

		// 
		struct PCS_ChatMsgResRetry : public PCS_ChatMsgRes2
		{
			enum {uri = (8 << 8) | protocol::IMCHAT2D_SVID};

			uint8_t  m_flag;

			virtual void marshal(sox::Pack &p) const
			{
				p.push_uint8(m_flag);
				PCS_ChatMsgRes2::marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				m_flag = p.pop_uint8();
				PCS_ChatMsgRes2::unmarshal(p);
			}
		};

		//返回guid
		struct PCS_GuidRes  : public sox::Marshallable
		{
			enum {uri = (9 << 8) | protocol::IMCHAT2D_SVID};

			uint32_t m_uId;					//To chat message receiver, the m_uId is sender's uid.    To chat message sender, the m_uId is receiver' uid.
			uint32_t m_uSeqId;
			std::string m_strGuid;
			uint32_t m_uSendTime;			//消息发送时间
			//uint32_t m_uBak;				//保留字


			virtual void marshal(sox::Pack &p) const
			{
				p << m_uId << m_uSeqId << m_strGuid << m_uSendTime;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uId >> m_uSeqId >> m_strGuid >> m_uSendTime;
			}
		};

        struct PCS_SyncTime: public sox::Marshallable
        {
            enum {uri = (11 << 8) | protocol::IMCHAT2D_SVID};
            uint32_t m_uCltTime;


            virtual void marshal(sox::Pack &p) const
            {
                p << m_uCltTime;
            }
    
            virtual void unmarshal(const sox::Unpack &p)
            {
                p >> m_uCltTime;
            }
        };

        struct PCS_SyncTimeRes: public sox::Marshallable
        {
            enum {uri = (12 << 8) | protocol::IMCHAT2D_SVID};
            uint32_t m_uCltTime;
            uint32_t m_uSvrTime;

            virtual void marshal(sox::Pack &p) const
            {
                p << m_uCltTime << m_uSvrTime;
            }

            virtual void unmarshal(const sox::Unpack &p)
            {
                p >> m_uCltTime >> m_uSvrTime;
            }
        };


		enum ENUM_CHAT_PROTOCOL
		{
			ENUM_CHAT_PROTOCOL_TCP = 0,
			ENUM_CHAT_PROTOCOL_UDP = 1,
			ENUM_CHAT_PROTOCOL_MULUDP = 2 //多路UDP
		};

        enum ENUM_CHAT_TEXT_TYPE
        {
            ENUM_CHAT_TEXT_IM = 0,
			ENUM_CHAT_TEXT_GROUP = 1,
			ENUM_CHAT_TEXT_AUTO = 2	//自动回复
        };

		enum ENUM_CHAT_REPAIR_TYPE
		{
			ENUM_CHAT_REPAIR_NONE = 0,
			ENUM_CHAT_REPAIR_PROTOCOL,
			ENUM_CHAT_REPAIR_IMONLINE
		};

		struct PCS_MultiRoute
		{
			uint32_t m_uFlags;

			PCS_MultiRoute()
				: m_uFlags(0)
			{
				SetRouteNumber(ENUM_FRONT_TCP);
			}

			void SetTextType(ENUM_CHAT_TEXT_TYPE enType)
			{
				m_uFlags &= 0xff00ffff;
				m_uFlags |= ((uint32_t)enType << 16) & 0xff0000;
			}

			void SetNumber(uint32_t uNum)
			{
				m_uFlags &= 0xffff00ff;
				m_uFlags |= (uNum << 8) & 0xff00;
			}

			void SetRepairFlag(ENUM_CHAT_REPAIR_TYPE emType)
			{
				m_uFlags &= 0x0fffffff;
				m_uFlags |= ((uint32_t)emType << 28) & 0xf0000000;
			}

			ENUM_CHAT_PROTOCOL GetProtocol()
			{
				return (ENUM_CHAT_PROTOCOL) (m_uFlags & 0xff);
			}

			// 补偿类型
			ENUM_CHAT_REPAIR_TYPE GetRepairType()
			{
				return (ENUM_CHAT_REPAIR_TYPE)((m_uFlags >> 28) & 0x0f);
			}

			uint32_t GetNumber()
			{
				return (m_uFlags >> 8) & 0xff;
			}

			//次高4位表示消息的路数
			void SetRouteNumber(EImFrontEndNo emFront)
			{
					m_uFlags &= 0xf0ffffff;
					m_uFlags |= ((uint32_t)emFront << 24) & 0x0f000000;
			}

			EImFrontEndNo GetRouteNumber()
			{
					return (EImFrontEndNo)((m_uFlags >> 24) & 0x0f);
			}

			void SetProtocol(ENUM_CHAT_PROTOCOL enProto)
			{
				m_uFlags &= 0xffffff00;
				m_uFlags |= ((uint32_t) enProto & 0xff);

				if (enProto == ENUM_CHAT_PROTOCOL_TCP)
				{
					SetRouteNumber(ENUM_FRONT_TCP);
				}
				else if (enProto == ENUM_CHAT_PROTOCOL_UDP)
				{
					SetRouteNumber(ENUM_FRONT_UDP);
				}
				else if (enProto == ENUM_CHAT_PROTOCOL_MULUDP) //多路UDP默认设置为第3路
				{
					SetRouteNumber(ENUM_FRONT_MUL_UDP1);
				}
			}

			ENUM_CHAT_TEXT_TYPE GetTextType()
			{
				return (ENUM_CHAT_TEXT_TYPE) ((m_uFlags >> 16) & 0xff);
			}
		};

        struct PCS_MultiRouteChatMsg: public PCS_MultiRoute, public PCS_ChatMsg2
        {
            enum {uri = (13 << 8) | protocol::IMCHAT2D_SVID};
            
            virtual void marshal(sox::Pack &p) const
            {
                p << m_uFlags;
                PCS_ChatMsg2::marshal(p);
            }

            virtual void unmarshal(const sox::Unpack &p)
            {
                p >> m_uFlags;
                PCS_ChatMsg2::unmarshal(p);
            }
        };

		//聊天消息响应的扩展字段
		struct ChatMsgResExtend : public sox::Marshallable
		{
			uint32_t gid;
			virtual void marshal(sox::Pack& pk) const
			{
				pk << gid;
			}
			virtual void unmarshal(const sox::Unpack& up)
			{
				up >> gid;
			}
			std::string toString() const
			{
				sox::PackBuffer bf;
				sox::Pack pk(bf);
				this->marshal(pk);
				return std::string(pk.data(), pk.size());
			}
			static ChatMsgResExtend fromString(const std::string &str)
			{
				sox::Unpack up(str.data(), str.size());
				ChatMsgResExtend ex;
				ex.unmarshal(up);
				return ex;
			}
		};

        struct PCS_MultiRouteChatMsgRes: public PCS_MultiRoute, public PCS_ChatMsgRes2
        {
            enum {uri = (14 << 8) | protocol::IMCHAT2D_SVID};
			string text;
            virtual void marshal(sox::Pack &p) const
            {
                p << m_uFlags;
                PCS_ChatMsgRes2::marshal(p);
				p << text;
            }

			//注意新旧协议兼容
            virtual void unmarshal(const sox::Unpack &p)
            {
			//	log(Info, "res unpack size:%u", p.size());
                p >> m_uFlags;
                PCS_ChatMsgRes2::unmarshal(p);
				if(0 != p.size())
				{
					try
					{
						p >> text;
					}
					catch(...)
					{
						text = "";
					}
				}
				else
					text = "";
            }
        };


        struct PSS_MultiRouteChatMsg: public PCS_MultiRouteChatMsg
        {
            enum {uri = (15 << 8) | protocol::IMCHAT2D_SVID};
        };


        struct PSS_MultiRouteChatMsgRes: public PCS_MultiRouteChatMsgRes
        {
            enum {uri = (16 << 8) | protocol::IMCHAT2D_SVID};
        };
		struct PCS_WindowShock: public sox::Marshallable
		{
			enum {uri = (17 << 8) | protocol::IMCHAT2D_SVID};
			uint32_t m_uFromId;
			std::string m_strNickName;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uFromId << m_strNickName;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uFromId >> m_strNickName;
			}
		};


		// 手机用户，消息接收者通知发送者消息已读

		struct PCS_MsgHasRead : public sox::Marshallable
		{
			enum {uri = (20 << 8) | protocol::IMCHAT2D_SVID};

			virtual void marshal(sox::Pack &p) const
			{

			}

			virtual void unmarshal(const sox::Unpack &p)
			{

			}
		};
	}
}







//PROTOCOL_IM_CHAT_NS_END

#endif

