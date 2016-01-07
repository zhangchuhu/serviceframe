/*
Note: In protocol, all data with std::string type must be coded with UTF8
*/
#ifndef ISEARCHRECORD_H 
#define ISEARCHRECORD_H

#include "common/packet.h" // for Marshallable
#include "common/protocol/PGTopicMsg.h"

namespace protocol {
	namespace search {
		// ������Ϣ��¼
		struct SChatMsgInfo : public sox::Marshallable 
		{
			uint32_t  fromUid; //dest id on sending , src id on recieving
			uint32_t  toUid;
			std::string strText;
			uint32_t  uSendTime;  //UTCʱ��
			std::string msgUUID;
			
			virtual void marshal(sox::Pack &p) const
			{
			 	p << fromUid << toUid << strText << uSendTime << msgUUID;
			}
			
			virtual void unmarshal(const sox::Unpack &p) 
			{
			 	p >> fromUid >> toUid >> strText >> uSendTime >> msgUUID;
			}
		};

		//���������¼
		struct STopicMsgInfo : public sox::Marshallable
		{
			uint32_t     m_senderUid;    // ������uid
			uint32_t     m_sendTime;     // ����ʱ�� 
			std::string  m_guid;         // ���Ե�guid
			protocol::gmsgcache::GTopicTextChat  m_msgText;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_senderUid << m_sendTime << m_guid;
				m_msgText.marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >>  m_senderUid >> m_sendTime >> m_guid;
				m_msgText.unmarshal(p);
			}
		};

	}
}
#endif

