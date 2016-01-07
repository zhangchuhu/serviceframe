#ifndef __PSTORE_IM_CHAT2_H__
#define  __PSTORE_IM_CHAT2_H__

#include "core/base_svid.h"///*protocol::WUJI_SIMPLE_SVID*/定义放的地方
#include "packet.h"//sox::Marshallable的定义
#include "protocol/const.h"

using namespace std;

namespace server {
namespace store {
//消息记录系统删除聊天消息的协议
struct PCS_DelMsg: public sox::Marshallable {
	enum {
		uri = (1 << 8) | protocol::ACCESS_SVID
	};


	uint32_t m_uId; //请求者uid
	uint32_t m_uPeerId; //被删除消息者uid
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

	uint32_t m_uId; //请求者uid
	uint32_t m_uPeerId; //被删除消息者uid
	uint32_t m_uTaskId;
	uint32_t m_uResCode;
	virtual void marshal(sox::Pack &p) const {
		p << m_uId << m_uPeerId << m_uTaskId << m_uResCode;
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> m_uId >> m_uPeerId >> m_uTaskId >> m_uResCode;
	}
};

//给消息记录系统永久保存聊天消息使用的协议
struct PSS_ChatRecord: public sox::Marshallable {
	enum {
		uri = (2 << 8) | protocol::ACCESS_SVID
	};
	PSS_ChatRecord() {
	}

	uint32_t m_uId; //消息发送方的uid
	uint32_t m_uRecvId; //消息接收方的uid
	std::string m_strText; //消息体（二进制格式保存，需要根据客户端的包协议再次解析）
	uint32_t m_uSeqId; //消息序号（由UI生成的序列号，只能保证一个用户在一次登录在线过程中是唯一的）
	uint32_t m_uSendTime; //消息发送时间(UTC时间)
	uint32_t m_uSendMicrosecond;	//消息发送时间(微秒部分)


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

	uint32_t m_uId; //消息发送方的uid
	uint32_t m_uRecvId; //消息接收方的uid
			uint32_t m_uSeqId;			//消息序号（由UI生成的序列号，只能保证一个用户在一次登录在线过程中是唯一的）
			std::string m_strGuid;		//消息的guid
			uint32_t m_uSendTime;		//消息的发送时间

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uId << m_uRecvId << m_uSeqId << m_strGuid << m_uSendTime;
			}
			
			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uId >> m_uRecvId >>  m_uSeqId >> m_strGuid >> m_uSendTime;
			}
};

//需要全存储回应（带群ID版本），参照PSS_ChatRecord
struct PSS_ChatRecordGrp : public sox::Marshallable
{
	enum { uri = (4 << 8) | protocol::ACCESS_SVID };

	uint32_t m_uId; //消息发送方的uid
	uint32_t m_uRecvId; //消息接收方的uid
	std::string m_strText; //消息体（二进制格式保存，需要根据客户端的包协议再次解析）
	uint32_t m_uSeqId; //消息序号（由UI生成的序列号，只能保证一个用户在一次登录在线过程中是唯一的）
	uint32_t m_uSendTime; //消息发送时间(UTC时间)
	uint32_t m_uSendMicrosecond;	//消息发送时间(微秒部分)

	uint32_t m_uGrpId; //群ID
	uint32_t m_uKey; //附加值（主要用于附带bridgeid)

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

	uint32_t m_uId; //消息发送方的uid
	uint32_t m_uRecvId; //消息接收方的uid
	uint32_t m_uSeqId;			//消息序号（由UI生成的序列号，只能保证一个用户在一次登录在线过程中是唯一的）
	std::string m_strGuid;		//消息的guid
	uint32_t m_uSendTime;		//消息的发送时间

	uint32_t m_uKey; //附加值（主要用于附带bridgeid)

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
