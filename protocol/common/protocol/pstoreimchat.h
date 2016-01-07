#ifndef __PSTORE_IM_CHAT2_H__
#define  __PSTORE_IM_CHAT2_H__

#include "core/base_svid.h"///*protocol::WUJI_SIMPLE_SVID*/����ŵĵط�
#include "packet.h"//sox::Marshallable�Ķ���
#include "protocol/const.h"

using namespace std;

namespace server {
namespace store {
//��Ϣ��¼ϵͳɾ��������Ϣ��Э��
struct PCS_DelMsg: public sox::Marshallable {
	enum {
		uri = (1 << 8) | protocol::ACCESS_SVID
	};


	uint32_t m_uId; //������uid
	uint32_t m_uPeerId; //��ɾ����Ϣ��uid
	uint32_t m_uTaskId;

	std::vector<std::string> msgUUIDList;


	virtual void marshal(sox::Pack &p) const {
		p << m_uId << m_uPeerId << m_uTaskId  ;
		marshal_container(p,msgUUIDList);
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> m_uId >> m_uPeerId >> m_uTaskId ;
		unmarshal_container(p,back_inserter(msgUUIDList));
	}
};

struct PCS_DelMsgRes: public sox::Marshallable {
	enum {
		uri = (1 << 8) | protocol::ACCESS_SVID
	};

	uint32_t m_uId; //������uid
	uint32_t m_uPeerId; //��ɾ����Ϣ��uid
	uint32_t m_uTaskId;
	uint32_t m_uResCode;
	virtual void marshal(sox::Pack &p) const {
		p << m_uId << m_uPeerId << m_uTaskId << m_uResCode;
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> m_uId >> m_uPeerId >> m_uTaskId >> m_uResCode;
	}
};

//����Ϣ��¼ϵͳ���ñ���������Ϣʹ�õ�Э��
struct PSS_ChatRecord: public sox::Marshallable {
	enum {
		uri = (2 << 8) | protocol::ACCESS_SVID
	};
	PSS_ChatRecord() {
	}

	uint32_t m_uId; //��Ϣ���ͷ���uid
	uint32_t m_uRecvId; //��Ϣ���շ���uid
	std::string m_strText; //��Ϣ�壨�����Ƹ�ʽ���棬��Ҫ���ݿͻ��˵İ�Э���ٴν�����
	uint32_t m_uSeqId; //��Ϣ��ţ���UI���ɵ����кţ�ֻ�ܱ�֤һ���û���һ�ε�¼���߹�������Ψһ�ģ�
	uint32_t m_uSendTime; //��Ϣ����ʱ��(UTCʱ��)
	uint32_t m_uSendMicrosecond;	//��Ϣ����ʱ��(΢�벿��)


	virtual void marshal(sox::Pack &p) const {
		p << m_uId << m_uRecvId << m_strText << m_uSeqId << m_uSendTime << m_uSendMicrosecond;
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> m_uId >> m_uRecvId >> m_strText >> m_uSeqId >> m_uSendTime >> m_uSendMicrosecond;
	}
};

struct PSS_ChatGuidRes: public sox::Marshallable {
	enum {
		uri = (3 << 8) | protocol::ACCESS_SVID
	};
	PSS_ChatGuidRes() {
	}

	uint32_t m_uId; //��Ϣ���ͷ���uid
	uint32_t m_uRecvId; //��Ϣ���շ���uid
			uint32_t m_uSeqId;			//��Ϣ��ţ���UI���ɵ����кţ�ֻ�ܱ�֤һ���û���һ�ε�¼���߹�������Ψһ�ģ�
			std::string m_strGuid;		//��Ϣ��guid
			uint32_t m_uSendTime;		//��Ϣ�ķ���ʱ��

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uId << m_uRecvId << m_uSeqId << m_strGuid << m_uSendTime;
			}
			
			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uId >> m_uRecvId >>  m_uSeqId >> m_strGuid >> m_uSendTime;
			}
};

//��Ҫȫ�洢��Ӧ����ȺID�汾��������PSS_ChatRecord
struct PSS_ChatRecordGrp : public sox::Marshallable
{
	enum { uri = (4 << 8) | protocol::ACCESS_SVID };

	uint32_t m_uId; //��Ϣ���ͷ���uid
	uint32_t m_uRecvId; //��Ϣ���շ���uid
	std::string m_strText; //��Ϣ�壨�����Ƹ�ʽ���棬��Ҫ���ݿͻ��˵İ�Э���ٴν�����
	uint32_t m_uSeqId; //��Ϣ��ţ���UI���ɵ����кţ�ֻ�ܱ�֤һ���û���һ�ε�¼���߹�������Ψһ�ģ�
	uint32_t m_uSendTime; //��Ϣ����ʱ��(UTCʱ��)
	uint32_t m_uSendMicrosecond;	//��Ϣ����ʱ��(΢�벿��)

	uint32_t m_uGrpId; //ȺID
	uint32_t m_uKey; //����ֵ����Ҫ���ڸ���bridgeid)

	virtual void marshal(sox::Pack &p) const {
		p << m_uId << m_uRecvId << m_strText << m_uSeqId << m_uSendTime << m_uSendMicrosecond << m_uGrpId << m_uKey;
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> m_uId >> m_uRecvId >> m_strText >> m_uSeqId >> m_uSendTime >> m_uSendMicrosecond >> m_uGrpId >> m_uKey;
	}
};

struct PSS_ChatGuidGrpRes: public sox::Marshallable
{
	enum { uri = (4 << 8) | protocol::ACCESS_SVID };

	uint32_t m_uId; //��Ϣ���ͷ���uid
	uint32_t m_uRecvId; //��Ϣ���շ���uid
	uint32_t m_uSeqId;			//��Ϣ��ţ���UI���ɵ����кţ�ֻ�ܱ�֤һ���û���һ�ε�¼���߹�������Ψһ�ģ�
	std::string m_strGuid;		//��Ϣ��guid
	uint32_t m_uSendTime;		//��Ϣ�ķ���ʱ��

	uint32_t m_uKey; //����ֵ����Ҫ���ڸ���bridgeid)

	virtual void marshal(sox::Pack &p) const
	{
		p << m_uId << m_uRecvId << m_uSeqId << m_strGuid << m_uSendTime << m_uKey;
	}

	virtual void unmarshal(const sox::Unpack &p) 
	{
		p >> m_uId >> m_uRecvId >>  m_uSeqId >> m_strGuid >> m_uSendTime >> m_uKey;
	}
};

}
}

#endif	//__PSTORE_IM_CHAT2_H__
