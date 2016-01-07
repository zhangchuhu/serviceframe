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
		//���˻Ự�û���

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

		//���󴴽����˻Ự

		struct PCS_ApplyCreateMulChat2 : public sox::Marshallable
		{
			enum {uri = (101 << 8) | protocol::IMCHAT2D_SVID};

			uint64_t m_uKey;									//��UI��д����Ӧʱ��ԭ������
			SUserItem m_stInviter;								//������
			std::vector<SUserItem> m_vecInvites;				//����ʱ������û��б�

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

		//�����߻��յ����µĻ�Ӧ
		//�����Ƿ�ɹ������Է�����Ϊ׼����������������û������������жϣ������������ < 3���᷵��ʧ��

		struct PCS_ApplyCreateMulChatRes2 : public protocol::im::PCS_CommonHead
		{
			enum {uri = (102 << 8) | protocol::IMCHAT2D_SVID};

			uint64_t m_uKey;									//����KEY
			std::string m_strGuid;								//���������ɵ�guid
			std::vector<SUserItem> m_vecInvitesOnline;			//������������û�
			std::vector<SUserItem> m_vecInvitesOffline;			//������������û�

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

		//���������û�����Ự

		struct PCS_ApplyInviteMulChat2 : public sox::Marshallable
		{
			enum {uri = (103 << 8) | protocol::IMCHAT2D_SVID};

			std::string m_strGuid;								//�ỰΨһ��ʶ
			SUserItem m_stInviter;								//������
			std::vector<SUserItem> m_vecMembers;				//��ǰ��Ա�б�
			std::vector<SUserItem> m_vecInvites;				//������������û�

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

		//�����˻��յ��û�Ӧ
		//�����Ƿ�ɹ������Է�����Ϊ׼

		struct PCS_ApplyInviteMulChatRes2 : public protocol::im::PCS_CommonHead
		{
			enum {uri = (104 << 8) | protocol::IMCHAT2D_SVID};

			std::string m_strGuid;								//�ỰΨһ��ʶ
			std::vector<SUserItem> m_vecInvitesOnline;			//������������û�
			std::vector<SUserItem> m_vecInvitesOffline;			//������������û�

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

		//�����û�����ʱ����ǰ��Ա�ͱ�������û������յ�����Ϣ

		struct PCS_InviteMulChat2 : public sox::Marshallable
		{
			enum {uri = (105 << 8) | protocol::IMCHAT2D_SVID};

			std::string m_strGuid;								//�ỰΨһ��ʶ
			SUserItem m_stInviter;								//������
			std::vector<SUserItem> m_vecMembers;				//��ǰ��Ա�б��´���ʱ����Ա�б�Ϊ������
			std::vector<SUserItem> m_vecInvites;				//������ĳ�Ա�б�

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

		//�û��뿪�Ự
		struct PCS_LeaveMulChatToServer2 : public sox::Marshallable
		{
			enum {uri = (106 << 8) | protocol::IMCHAT2D_SVID};

			std::string m_strGuid;								//�ỰΨһ��ʶ
			SUserItem m_stLeaver;								//�뿪���û�
			std::vector<SUserItem> m_vecMembers;				//��ǰ��Ա�б�
			
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

			std::string m_strGuid;								//�ỰΨһ��ʶ
			SUserItem m_stLeaver;								//�뿪���û�

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

		//���͵��������Ķ��˻Ự��Ϣ
		//����ȫ����Ա�б�����protocol�յ�����Ϣ�󣬿��Ծݴ˻ָ����˻Ự

		struct PCS_MulChatMessage2 : public sox::Marshallable
		{
			enum {uri = (108 << 8) | protocol::IMCHAT2D_SVID};

			std::string	m_strGuid;								//�ỰΨһ��ʶ
			std::vector<SUserItem> m_vecMembers;				//���еĽ��ճ�Ա�б����������ˣ����ɷ������Է����˽���ǩ��
			uint32_t m_uFlag;									//��־������Ϊ0

			SUserItem m_stSender;

			std::string m_strText;								//��Ϣ����
			uint32_t m_uSeqId;									//��Ϣ��ˮ��
			uint32_t m_uSendTime;								//UTCʱ�䣨�ɷ�������д��
			uint32_t m_uSendMicrosecond;						//��Ϣ����ʱ��(΢�벿��)
			//protocol::im::PCS_ChatMsg m_ChatMsg;				//m_ChatMsg.id��ʾ�����ˣ��ɷ�������д��

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

		//�û��յ����˻Ự��Ϣ��Ļ�Ӧ
		//struct PCS_MulChatMessageMemberRes2 : public sox::Marshallable
		//{
		//	enum {uri = (109 << 8) | protocol::IMCHAT2D_SVID};

		//	std::string m_strGuid;								//�ỰΨһ��ʶ
		//	//protocol::im::PCS_ChatMsgRes m_ChatMsgRes;		//��Ϣ��Ӧ
		//	uint32_t m_uUid;									//����ʱ��ʾ�����ˣ�����ʱ��ʾ������
		//	uint32_t m_uSeqId;									//��Ϣ���к�

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

		//������Ϣʱ�����ָ���Ľ������Ѿ����ߣ����������������û��㲥֪ͨ
		struct PCS_MulChatMemberOffline2 : public sox::Marshallable
		{
			enum {uri = (109 << 8) | protocol::IMCHAT2D_SVID};

			std::string m_strGuid;								//�ỰΨһ��ʶ
			std::vector<SUserItem> m_vecMembersOffline;			//��Щ�û��Ѿ�������

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

		//���ûỰ����
		struct PCS_SetMulChatTopic2 : public sox::Marshallable
		{
			enum {uri = (110 << 8) | protocol::IMCHAT2D_SVID};

			std::string m_strGuid;								//�ỰΨһ��ʶ
			std::string m_strTopic;								//����
			std::vector<SUserItem> m_vecMembers;				//���еĽ��ճ�Ա�б����������ˣ����ɷ������Է����˽���ǩ��

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


		//���˷�����Ϣ�󣬷������᷵�ظ���Ϣ��GUID��s*1000000+us����ʾ��

		//struct PCS_MulChatMessageGuidRes2 : public sox::Marshallable
		//{
		//	enum {uri = (112 << 8) | protocol::IMCHAT2D_SVID};

		//	std::string m_strGuid;								//�ỰΨһ��ʶ
		//	uint32_t m_uSeqId;									//��Ϣ���к�

		//	uint32_t m_uSendTime;								//UTCʱ�䣨�ɷ�������д��
		//	uint32_t m_uSendMicrosecond;						//��Ϣ����ʱ��(΢�벿��)

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

		//	std::string m_strGuid;								//�ỰΨһ��ʶ
		//	uint32_t m_uSenderId;								//�û�
		//	protocol::E_IMSTATUS m_emStatus;					//״̬

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

		//	std::string m_strGuid;								//�ỰΨһ��ʶ
		//	std::vector<uint32_t> m_vecMembers;					//�Ự��Ա���������Լ���

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

		//	std::string m_strGuid;								  //�ỰΨһ��ʶ
		//	std::map<uint32_t, CVersionStatus> m_mapMembersStatus; //��Ա״̬���������Լ���

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
