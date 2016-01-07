#pragma once
#include <vector>


#include "core/base_svid.h"
#include "packet.h"//sox::Marshallable�Ķ���
#include "protocol/const.h"
#include "server_common/protocol/psession.h"


namespace server
{
	namespace session
	{
		// Ƶ��������־,��Э��
		struct PMsgToLog2 :  public sox::Marshallable
		{
			enum {uri = (1 << 8) | protocol::SESSION_SAVE_LOG_SVID};

			uint32_t		topmost_sid;	// ����Ƶ��id
			SesisonOPType	type;			// ��������
			uint32_t		admin_uid;		// ����Ա
			std::string		admin_nick;		// ����Ա�ǳ�
			uint32_t		id;				// ����������user or session��
			std::string		id_nick;		// �����������ǳ�
			std::string		op_event;		// �¼�����
			std::string		remark;			// ��ע
			uint32_t m_uTime;//����ʱ��(���Ͷ˸����

			virtual void marshal(sox::Pack &pk) const
			{
				pk << topmost_sid;
				pk.push_uint8(type);
				pk << admin_uid << admin_nick
					<< id << id_nick
					<< op_event << remark<<m_uTime;

			}
			virtual void unmarshal(const sox::Unpack &upk)
			{
				upk >> topmost_sid;
				type = (SesisonOPType)upk.pop_uint8();
				upk >> admin_uid >> admin_nick
					>> id >> id_nick
					>> op_event >> remark>>m_uTime;
			}
		};


		//session����������־����
		struct PCS_UserBehavior : public sox::Marshallable 
		{
			enum {uri = (2 << 8) | protocol::SESSION_SAVE_LOG_SVID};
			PCS_UserBehavior ()
			{
				m_uSeqId=0;
				m_uExtraData=0;

			}
			std::vector<PMsgToLog2> m_vecData;
			uint32_t m_uSeqId;//��ˮ��
			uint32_t m_uExtraData;//�����ˮ�Ų�������Ψһ�İ�������ʹ����������������֡�


			virtual void marshal(sox::Pack &p) const
			{
				marshal_container(p, m_vecData);
				p<<m_uSeqId<<m_uExtraData;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				unmarshal_container(p, std::back_inserter(m_vecData));
				p>>m_uSeqId>>m_uExtraData;
			}
		};

		//PCS_UserBehavior����Ӧ
		struct PCS_UserBehaviorRes : public sox::Marshallable 
		{
			enum {uri = (3 << 8) | protocol::SESSION_SAVE_LOG_SVID};
			PCS_UserBehaviorRes ()
			{
				m_uSeqId=0;
				m_uExtraData=0;

			}
			
			uint32_t m_uSeqId;//��ˮ��
			uint32_t m_uExtraData;//�����ˮ�Ų�������Ψһ�İ�������ʹ����������������֡�


			virtual void marshal(sox::Pack &p) const
			{
				p<<m_uSeqId<<m_uExtraData;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p>>m_uSeqId>>m_uExtraData;
			}
		};
	}
}

