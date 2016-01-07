#ifndef IGRPLISTDATATYPE_H_
#define IGRPLISTDATATYPE_H_

#include "common/packet.h" //sox::Marshallable

#include <vector>

namespace protocol{

namespace glist{

enum ERecvMode{
	ENUM_NOTIFY_ALL = 0, // notify all message. 
	ENUM_NEW_CARE_TOPIC, // only notify new topic and care topic. ==> default mode.
	ENUM_NOTIFY_NONE, // notify nothing.

    ENUM_RECV_MODE_DEFAULT = ENUM_NEW_CARE_TOPIC,//Justin fang -TODO: BUSYBOX-OPTIMIZE
    
	ENUM_BACKUP_1, // in case needs more option.
	ENUM_BACKUP_2,
	ENUM_BACKUP_3,
	ENUM_BACKUP_4,
	ENUM_BACKUP_5,
	ENUM_BACKUP_6,
};

//Justin fang -TODO: BUSYBOX-OPTIMIZE
//const int DEFAULT_RECV_MODE_VAL = ENUM_NEW_CARE_TOPIC;

/*
// Get alias of groups for a uid, so define this struct to combine
// groupId and its alias.
struct CGroupAliasWStr{
	uint32_t m_uGrpId;
	std::wstring m_wstrGrpAlias; // for UI.
};
*/

// for wstring<->string. UI should not care about this struct.
// Get alias of groups for a uid, so define this struct to combine
// groupId and its alias.
struct CGroupAlias: public sox::Marshallable{
	uint32_t m_uGrpId;
	std::string m_strGrpAlias;
	virtual void marshal(sox::Pack &p) const {
		p << m_uGrpId << m_strGrpAlias;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> m_uGrpId >> m_strGrpAlias;
	}
};

// Get receive modes for a uid, so define this struct to combine
// groupId and its receive mode.
struct CRecvMode: public sox::Marshallable{
	// uint32_t m_uGrpId;  
	uint32_t m_uFolderId; // may be folderId
	ERecvMode m_eRecvMode;

    //Justin fang -TODO: BUSYBOX-OPTIMIZE
    CRecvMode():
        m_uFolderId(0),
        m_eRecvMode(ENUM_RECV_MODE_DEFAULT)
        {}

    CRecvMode(uint32_t uFid):
        m_uFolderId(uFid),
        m_eRecvMode(ENUM_RECV_MODE_DEFAULT)
        {}
        
    CRecvMode(uint32_t uFid, ERecvMode eRecvMode):
        m_uFolderId(uFid),
        m_eRecvMode(eRecvMode)
        {}

    //Define the Key.
    bool operator < (const CRecvMode &objRecvMode) const
    {
        return (m_uFolderId < objRecvMode.m_uFolderId);
        
    }

	CRecvMode& operator = (const CRecvMode& objRecvMode)
    {
		m_uFolderId = objRecvMode.m_uFolderId;
		m_eRecvMode = objRecvMode.m_eRecvMode;
		return *this;
	}

	virtual void marshal(sox::Pack &p) const 
    {
		p <<  m_uFolderId << (uint32_t)m_eRecvMode;
	}
    
	virtual void unmarshal(const sox::Unpack &p) 
    {
		p >> m_uFolderId;
		uint32_t uRecvMode;
		p >> uRecvMode;
		m_eRecvMode = (ERecvMode)uRecvMode;
	}


};


// this struct help us to classify refused topics 
// according to groupId or folderId.
struct CGrpRefuseTopics: public sox::Marshallable{
	uint32_t m_uGrpId; // groupId or folderId
	std::vector<uint32_t> m_vecRefuTopics;

	virtual void marshal(sox::Pack &p) const {
		p << m_uGrpId;
		marshal_container(p, m_vecRefuTopics);
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> m_uGrpId;
		unmarshal_container(p, std::back_inserter(m_vecRefuTopics)); 
	}
};

enum ECareType{
	ENUM_USER_PUBLISH, // user publish the topic
	ENUM_USER_REPLY, // user reply the topic
	ENUM_USER_ADD // user select to care the topic
};

// this struct help us to classify cared topics 
// according to groupId or folderId.
struct CGrpCareTopics: public sox::Marshallable{
	uint32_t m_uGrpId; // groupId or folderId
	std::vector<uint32_t> m_vecCareTopics;

	virtual void marshal(sox::Pack &p) const {
		p << m_uGrpId;
		marshal_container(p, m_vecCareTopics);
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> m_uGrpId;
		unmarshal_container(p, std::back_inserter(m_vecCareTopics)); 
	}
};

// Get care topic, so define this struct to combine
// topicId and its care type.
struct CCareTopic: public sox::Marshallable{
	uint32_t m_uTopicId;
	ECareType m_eCareType;

    CCareTopic() :
        m_uTopicId(0),
        m_eCareType(ENUM_USER_PUBLISH)
        {}

    CCareTopic(uint32_t uTid) : 
        m_uTopicId(uTid), 
        m_eCareType(ENUM_USER_PUBLISH) 
        {}

    CCareTopic(uint32_t uTid, ECareType eCareType) : 
        m_uTopicId(uTid), 
        m_eCareType(eCareType) 
        {}
    
	CCareTopic& operator = (const CCareTopic& objCareTopic){
		m_uTopicId = objCareTopic.m_uTopicId;
		m_eCareType = objCareTopic.m_eCareType;
		return *this;
	}

	bool operator < (const CCareTopic& objCareTopic)const{
		return m_uTopicId < objCareTopic.m_uTopicId;
	}

	virtual void marshal(sox::Pack &p) const {
		p << m_uTopicId << (uint32_t)m_eCareType;
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> m_uTopicId;
		uint32_t uCareType;
		p >> uCareType;
		m_eCareType = (ECareType)uCareType;
	}
};

enum ETopicProperty
{
    E_TOPIC_USER_PUBLISH = 0, // user publish the topic
    E_TOPIC_USER_REPLY, // user reply the topic
    E_TOPIC_USER_ADD, // user select to care the topic

    E_TOPIC_USER_CARE_DEFAULT = E_TOPIC_USER_PUBLISH,//Justin fang -TODO: BUSYBOX-LCACHE
    E_TOPIC_USER_CARE_END = E_TOPIC_USER_ADD,//Please add care type above it. //Justin fang -TODO: BUSYBOX-LCACHE TM
    E_TOPIC_USER_REFUSE = 0xFF,// refuse topic

    E_TOPIC_USER_REFUSE_END = 0x1FF,
    E_TOPIC_USER_INVALID
};

struct STopicsProperty : public sox::Marshallable
{
    uint32_t m_uTopicId; //Topic ID
    ETopicProperty m_eTopicType;//Topic type: care or refuse.
    
    STopicsProperty()
    {
        m_uTopicId = 0;
        m_eTopicType = E_TOPIC_USER_PUBLISH;
    }

    STopicsProperty(uint32_t uTopicId)
    {
        m_uTopicId = uTopicId;
        m_eTopicType = E_TOPIC_USER_PUBLISH;
    }
    
    STopicsProperty(uint32_t uTopicId, ETopicProperty eTopicProperty)
    {
        m_uTopicId = uTopicId;
        m_eTopicType = eTopicProperty;
    }
    
    
    STopicsProperty& operator = (const STopicsProperty &objTopicType)
    {
        m_uTopicId = objTopicType.m_uTopicId;
        m_eTopicType = objTopicType.m_eTopicType;
        return *this;
    }

    bool operator < (const STopicsProperty &objTopicType) const
    {
        return m_uTopicId < objTopicType.m_uTopicId;
    }

    virtual void marshal(sox::Pack &p) const 
    {
        p << m_uTopicId << (uint32_t)m_eTopicType;
    }

    virtual void unmarshal(const sox::Unpack &p) 
    {
        p >> m_uTopicId;
        uint32_t uTopicType;
        p >> uTopicType;
        m_eTopicType = (ETopicProperty)uTopicType;
    }
};

//Justin fang -TODO: BUSYBOX-DCHAT
enum EDChatMode
{
    E_DCHAT_NO_ACTION = 0,  //Neither Care or Refuse
    E_DCHAT_CARE,           //Care the D-Chat, Default Mode.
    E_DCHAT_REFUSE,         //Refuse the D-Chat.

    E_DCHAT_DEFAULT = E_DCHAT_CARE,   //The Default Mode.     
    E_DCHAT_VALID_END = E_DCHAT_REFUSE, //Mark the last valid one.
    E_DCHAT_INVALID         //Invalid D-Chat Mode.
};

//Default Chat Mode: FID+Mode
struct SDChatMode: public sox::Marshallable
{
	uint32_t m_uFolderId;
	EDChatMode m_eDChatMode;

    SDChatMode() : 
        m_uFolderId(0),
        m_eDChatMode(E_DCHAT_DEFAULT)    
        {}

    SDChatMode(uint32_t uFid) :
        m_uFolderId(uFid),
        m_eDChatMode(E_DCHAT_DEFAULT)
        {}
        
    SDChatMode(uint32_t uFid, EDChatMode eDChatMode) :
        m_uFolderId(uFid),
        m_eDChatMode(eDChatMode)
        {}
    
	SDChatMode& operator = (const SDChatMode &objDChatMode)
	{
		m_uFolderId = objDChatMode.m_uFolderId;
		m_eDChatMode = objDChatMode.m_eDChatMode;
		return *this;
	}

    //Define the Key.
    bool operator < (const SDChatMode &objDChatMode) const
    {
        return (m_uFolderId < objDChatMode.m_uFolderId);
    }

	virtual void marshal(sox::Pack &p) const 
	{
		p <<m_uFolderId <<(uint32_t)m_eDChatMode;
	}
    
	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> m_uFolderId;
        
		uint32_t uDChatMode;
		p >> uDChatMode;
		m_eDChatMode = (EDChatMode)uDChatMode;
	}
    
};

//Justin fang -TODO:BUSYBOX-BETA6
enum EMsgRcvMode
{
    E_MSG_RCV_POP = 0,
    E_MSG_RCV_BEAT,
    E_MSG_RCV_SUM,

    E_MSG_RCV_DEFAULT = E_MSG_RCV_BEAT,
    E_MSG_RCV_INVALID = 0xff
};

struct CMsgRcvMode: public sox::Marshallable
{
    //member list.
    uint32_t     m_uGFid;
    EMsgRcvMode  m_eMsgRcvMode;

    //construct && deconstruct
    CMsgRcvMode():
        m_uGFid(0),
        m_eMsgRcvMode(E_MSG_RCV_DEFAULT)
        {}

    CMsgRcvMode(uint32_t uGFid):
        m_uGFid(uGFid),
        m_eMsgRcvMode(E_MSG_RCV_DEFAULT)
        {}
        
    CMsgRcvMode(uint32_t uGFid, EMsgRcvMode eMsgRcvMode):
        m_uGFid(uGFid),
        m_eMsgRcvMode(eMsgRcvMode)
        {}

    //Define the Key function.
    bool operator < (const CMsgRcvMode &objMsgRcvMode) const
    {
        return (m_uGFid < objMsgRcvMode.m_uGFid);
    }

    CMsgRcvMode& operator = (const CMsgRcvMode& objMsgRcvMode)
    {
        m_uGFid = objMsgRcvMode.m_uGFid;
        m_eMsgRcvMode = objMsgRcvMode.m_eMsgRcvMode;
        return *this;
    }

    //marshal && unmarshal
    virtual void marshal(sox::Pack &p) const 
    {
        p<<m_uGFid<<(uint32_t)m_eMsgRcvMode;
    }
    
    virtual void unmarshal(const sox::Unpack &p) 
    {
        uint32_t uMsgRcvMode;
        
        p >> m_uGFid;
        p >> uMsgRcvMode;
        m_eMsgRcvMode = (EMsgRcvMode)uMsgRcvMode;
    }
};


}
}

#endif
