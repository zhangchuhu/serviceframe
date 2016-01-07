#pragma once 

#include "common/int_types.h"
#include "common/packet.h"
#include "common/protocol/pimcommon.h"
#include "common/protocol/const.h"

#include "core/base_svid.h"

#include <iostream>
#include <string>
#include <vector>

#include "pimchat.h"
#include "common/protocol/PMulChatDef.h"
#include "PMulChat.h"

#include "pimtrans.h"

namespace protocol
{
	namespace im
	{
		//多人会话用户项

		//struct SUserItem : public sox::Marshallable
		//{
		//	uint32_t m_uUid;
		//	std::string m_strNick;

		//	SUserItem() : m_uUid(0), m_strNick("") {};
		//	SUserItem(uint32_t uUid) : m_uUid(uUid), m_strNick("") {};
		//	SUserItem(uint32_t uUid, const std::string &strNick) : m_uUid(uUid), m_strNick(strNick) {};

		//	virtual void marshal(sox::Pack &p) const 
		//	{
		//		p << m_uUid << m_strNick;
		//	}

		//	virtual void unmarshal(const sox::Unpack &p) 
		//	{
		//		p >> m_uUid >> m_strNick;
		//	}

		//	bool operator == (const SUserItem &stUser)
		//	{
		//		return (m_uUid == stUser.m_uUid);
		//	}
		//};

		//-----------------------------------------------------------------------------------------------

		//请求创建多人会话

		struct PCS_ApplyCreateMulChat2 : public sox::Marshallable
		{
			enum {uri = (101 << 8) | protocol::IMCHAT2D_SVID};

			uint64_t m_uKey;									//由UI填写，响应时会原样返回
			SUserItem m_stInviter;								//邀请人
			std::vector<SUserItem> m_vecInvites;				//创建时邀请的用户列表

			PCS_ApplyCreateMulChat2() {}

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uKey;
				m_stInviter.marshal(p);
				sox::marshal_container(p, m_vecInvites);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uKey;
				m_stInviter.unmarshal(p);
				sox::unmarshal_container(p, std::back_inserter(m_vecInvites));
			}
		};

		//创建者会收到如下的回应
		//操作是否成功，请以返回码为准，服务器会对在线用户的数量进行判断，如果在线数量 < 3，会返回失败

		struct PCS_ApplyCreateMulChatRes2 : public protocol::im::PCS_CommonHead
		{
			enum {uri = (102 << 8) | protocol::IMCHAT2D_SVID};

			uint64_t m_uKey;									//返回KEY
			std::string m_strGuid;								//服务器生成的guid
			std::vector<SUserItem> m_vecInvitesOnline;			//被邀请的在线用户
			std::vector<SUserItem> m_vecInvitesOffline;			//被邀请的离线用户

			PCS_ApplyCreateMulChatRes2() {}

			virtual void marshal(sox::Pack &p) const 
			{
				protocol::im::PCS_CommonHead::marshal(p);
				p << m_uKey << m_strGuid;
				sox::marshal_container(p, m_vecInvitesOnline);
				sox::marshal_container(p, m_vecInvitesOffline);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				protocol::im::PCS_CommonHead::unmarshal(p);
				p >> m_uKey >> m_strGuid;
				sox::unmarshal_container(p, std::back_inserter(m_vecInvitesOnline));
				sox::unmarshal_container(p, std::back_inserter(m_vecInvitesOffline));
			}
		};

		//请求邀请用户加入会话

		struct PCS_ApplyInviteMulChat2 : public sox::Marshallable
		{
			enum {uri = (103 << 8) | protocol::IMCHAT2D_SVID};

			std::string m_strGuid;								//会话唯一标识
			SUserItem m_stInviter;								//邀请人
			std::vector<SUserItem> m_vecMembers;				//当前成员列表
			std::vector<SUserItem> m_vecInvites;				//被邀请的在线用户

			PCS_ApplyInviteMulChat2() {}

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_strGuid;
				m_stInviter.marshal(p);
				sox::marshal_container(p, m_vecMembers);
				sox::marshal_container(p, m_vecInvites);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_strGuid;
				m_stInviter.unmarshal(p);
				sox::unmarshal_container(p, std::back_inserter(m_vecMembers));
				sox::unmarshal_container(p, std::back_inserter(m_vecInvites));
			}
		};

		//邀请人会收到该回应
		//操作是否成功，请以返回码为准

		struct PCS_ApplyInviteMulChatRes2 : public protocol::im::PCS_CommonHead
		{
			enum {uri = (104 << 8) | protocol::IMCHAT2D_SVID};

			std::string m_strGuid;								//会话唯一标识
			std::vector<SUserItem> m_vecInvitesOnline;			//被邀请的在线用户
			std::vector<SUserItem> m_vecInvitesOffline;			//被邀请的离线用户

			PCS_ApplyInviteMulChatRes2() {}

			virtual void marshal(sox::Pack &p) const 
			{
				protocol::im::PCS_CommonHead::marshal(p);
				p << m_strGuid;
				sox::marshal_container(p, m_vecInvitesOnline);
				sox::marshal_container(p, m_vecInvitesOffline);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				protocol::im::PCS_CommonHead::unmarshal(p);
				p >> m_strGuid;
				sox::unmarshal_container(p, std::back_inserter(m_vecInvitesOnline));
				sox::unmarshal_container(p, std::back_inserter(m_vecInvitesOffline));
			}
		};

		//邀请用户加入时，当前成员和被邀请的用户都会收到该消息

		struct PCS_InviteMulChat2 : public sox::Marshallable
		{
			enum {uri = (105 << 8) | protocol::IMCHAT2D_SVID};

			std::string m_strGuid;								//会话唯一标识
			SUserItem m_stInviter;								//邀请人
			std::vector<SUserItem> m_vecMembers;				//当前成员列表，新创建时，成员列表为创建人
			std::vector<SUserItem> m_vecInvites;				//被邀请的成员列表

			PCS_InviteMulChat2() {}

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_strGuid;
				m_stInviter.marshal(p);
				sox::marshal_container(p, m_vecMembers);
				sox::marshal_container(p, m_vecInvites);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_strGuid;
				m_stInviter.unmarshal(p);
				sox::unmarshal_container(p, std::back_inserter(m_vecMembers));
				sox::unmarshal_container(p, std::back_inserter(m_vecInvites));
			}
		};

		//用户离开会话
		struct PCS_LeaveMulChatToServer2 : public sox::Marshallable
		{
			enum {uri = (106 << 8) | protocol::IMCHAT2D_SVID};

			std::string m_strGuid;								//会话唯一标识
			SUserItem m_stLeaver;								//离开的用户
			std::vector<SUserItem> m_vecMembers;				//当前成员列表
			
			PCS_LeaveMulChatToServer2() {}

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_strGuid;
				m_stLeaver.marshal(p);
				sox::marshal_container(p, m_vecMembers);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_strGuid;
				m_stLeaver.unmarshal(p);
				sox::unmarshal_container(p, std::back_inserter(m_vecMembers));
			}
		};

		struct PCS_LeaveMulChatToClient2 : public sox::Marshallable
		{
			enum {uri = (107 << 8) | protocol::IMCHAT2D_SVID};

			std::string m_strGuid;								//会话唯一标识
			SUserItem m_stLeaver;								//离开的用户

			PCS_LeaveMulChatToClient2() {}

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_strGuid;
				m_stLeaver.marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_strGuid;
				m_stLeaver.unmarshal(p);
			}
		};

		//-----------------------------------------------------------------------------------------------

		//发送到服务器的多人会话消息
		//带上全部成员列表，用于protocol收到该消息后，可以据此恢复多人会话

		struct PCS_MulChatMessage2 : public sox::Marshallable
		{
			enum {uri = (108 << 8) | protocol::IMCHAT2D_SVID};

			std::string	m_strGuid;								//会话唯一标识
			std::vector<SUserItem> m_vecMembers;				//所有的接收成员列表（包括发送人），由服务器对发送人进行签别
			uint32_t m_uFlag;									//标志，保留为0

			SUserItem m_stSender;

			std::string m_strText;								//消息内容
			uint32_t m_uSeqId;									//消息流水号
			uint32_t m_uSendTime;								//UTC时间（由服务器填写）
			uint32_t m_uSendMicrosecond;						//消息发送时间(微秒部分)
			//protocol::im::PCS_ChatMsg m_ChatMsg;				//m_ChatMsg.id表示发送人（由服务器填写）

			PCS_MulChatMessage2() : m_uFlag(0) {}

			void SetMessage(const std::string &strText, uint32_t uSeqId, uint32_t uSendTime)
			{
				m_strText = strText;
				m_uSeqId = uSeqId;
				m_uSendTime = uSendTime;
			}

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_strGuid;
				sox::marshal_container(p, m_vecMembers);
				p << m_uFlag;

				m_stSender.marshal(p);

				p << m_strText << m_uSeqId << m_uSendTime << m_uSendMicrosecond;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_strGuid;
				sox::unmarshal_container(p, std::back_inserter(m_vecMembers));
				p >> m_uFlag;

				m_stSender.unmarshal(p);

				p >> m_strText >> m_uSeqId >> m_uSendTime >> m_uSendMicrosecond;
			}
		};

		//用户收到多人会话消息后的回应
		//struct PCS_MulChatMessageMemberRes2 : public sox::Marshallable
		//{
		//	enum {uri = (109 << 8) | protocol::IMCHAT2D_SVID};

		//	std::string m_strGuid;								//会话唯一标识
		//	//protocol::im::PCS_ChatMsgRes m_ChatMsgRes;		//消息响应
		//	uint32_t m_uUid;									//发送时表示接收人，接收时表示发送人
		//	uint32_t m_uSeqId;									//消息序列号

		//	PCS_MulChatMessageMemberRes2() {}

		//	void SetMessageRes(uint32_t uUid, uint32_t uSeqId)
		//	{
		//		m_uUid = uUid;
		//		m_uSeqId = uSeqId;
		//	}

		//	virtual void marshal(sox::Pack &p) const 
		//	{
		//		p << m_strGuid << m_uUid << m_uSeqId;
		//	}

		//	virtual void unmarshal(const sox::Unpack &p) 
		//	{
		//		p >> m_strGuid >> m_uUid >> m_uSeqId;
		//	}
		//};

		//发送消息时，如果指定的接收人已经离线，服务器会向在线用户广播通知
		struct PCS_MulChatMemberOffline2 : public sox::Marshallable
		{
			enum {uri = (109 << 8) | protocol::IMCHAT2D_SVID};

			std::string m_strGuid;								//会话唯一标识
			std::vector<SUserItem> m_vecMembersOffline;			//这些用户已经下线了

			PCS_MulChatMemberOffline2() {}

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_strGuid;
				sox::marshal_container(p, m_vecMembersOffline);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_strGuid;
				sox::unmarshal_container(p, std::back_inserter(m_vecMembersOffline));
			}
		};

		//设置会话主题
		struct PCS_SetMulChatTopic2 : public sox::Marshallable
		{
			enum {uri = (110 << 8) | protocol::IMCHAT2D_SVID};

			std::string m_strGuid;								//会话唯一标识
			std::string m_strTopic;								//主题
			std::vector<SUserItem> m_vecMembers;				//所有的接收成员列表（包括发送人），由服务器对发送人进行签别

			PCS_SetMulChatTopic2() {}

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_strGuid << m_strTopic;
				sox::marshal_container(p, m_vecMembers);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_strGuid >> m_strTopic;
				sox::unmarshal_container(p, std::back_inserter(m_vecMembers));
			}
		};


		//本人发送消息后，服务器会返回该消息的GUID（s*1000000+us来表示）

		//struct PCS_MulChatMessageGuidRes2 : public sox::Marshallable
		//{
		//	enum {uri = (112 << 8) | protocol::IMCHAT2D_SVID};

		//	std::string m_strGuid;								//会话唯一标识
		//	uint32_t m_uSeqId;									//消息序列号

		//	uint32_t m_uSendTime;								//UTC时间（由服务器填写）
		//	uint32_t m_uSendMicrosecond;						//消息发送时间(微秒部分)

		//	PCS_MulChatMessageGuidRes2() {}

		//	virtual void marshal(sox::Pack &p) const 
		//	{
		//		p << m_strGuid << m_uSeqId << m_uSendTime << m_uSendMicrosecond;
		//	}

		//	virtual void unmarshal(const sox::Unpack &p) 
		//	{
		//		p >> m_strGuid >> m_uSeqId >> m_uSendTime >> m_uSendMicrosecond;
		//	}
		//};

		//-----------------------------------------------------------------------------------------------

		//struct PCS_MulChatChangeStatus2 : public sox::Marshallable
		//{
		//	enum {uri = (113 << 8) | protocol::IMCHAT2D_SVID};

		//	std::string m_strGuid;								//会话唯一标识
		//	uint32_t m_uSenderId;								//用户
		//	protocol::E_IMSTATUS m_emStatus;					//状态

		//	PCS_MulChatChangeStatus2() {}

		//	virtual void marshal(sox::Pack &p) const 
		//	{
		//		p << m_strGuid << m_uSenderId;
		//		p.push_uint8((uint8_t)m_emStatus);
		//	}

		//	virtual void unmarshal(const sox::Unpack &p) 
		//	{
		//		p >> m_strGuid >> m_uSenderId;
		//		m_emStatus = (protocol::E_IMSTATUS)p.pop_uint8();
		//	}
		//};

		//struct PCS_MulChatMemberStatus2 : public sox::Marshallable
		//{
		//	enum {uri = (114 << 8) | protocol::IMCHAT2D_SVID};

		//	std::string m_strGuid;								//会话唯一标识
		//	std::vector<uint32_t> m_vecMembers;					//会话成员（不包括自己）

		//	PCS_MulChatMemberStatus2() {}

		//	virtual void marshal(sox::Pack &p) const 
		//	{
		//		p << m_strGuid;
		//		sox::marshal_container(p, m_vecMembers);
		//	}

		//	virtual void unmarshal(const sox::Unpack &p) 
		//	{
		//		p >> m_strGuid;
		//		sox::unmarshal_container(p, std::back_inserter(m_vecMembers));
		//	}
		//};

		//struct PCS_MulChatMemberStatusRes2 : public sox::Marshallable
		//{
		//	enum {uri = (115 << 8) | protocol::IMCHAT2D_SVID};

		//	std::string m_strGuid;								  //会话唯一标识
		//	std::map<uint32_t, CVersionStatus> m_mapMembersStatus; //成员状态（不包括自己）

		//	PCS_MulChatMemberStatusRes2() {}

		//	virtual void marshal(sox::Pack &p) const 
		//	{
		//		p << m_strGuid;
		//		sox::marshal_container(p, m_mapMembersStatus);
		//	}

		//	virtual void unmarshal(const sox::Unpack &p) 
		//	{
		//		p >> m_strGuid;
		//		sox::unmarshal_container(p, std::inserter(m_mapMembersStatus, m_mapMembersStatus.begin()));
		//	}
		//};
	}
}
