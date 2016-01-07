/*!
 * @file PMobile.h
 * @brief 定义手机YY应用协议
 */
#ifndef PROTOTOCOL_MOBILE_H
#define PROTOTOCOL_MOBILE_H

#include "packet.h"
#include "common/core/base_svid.h"

namespace protocol
{
namespace mobile
{

/*!
 * @class PCS_UploadLocation
 * @brief 上传自己所在的经纬度
 */
struct PCS_UploadLocation: public sox::Marshallable
{
	enum {uri = (1 << 8) | protocol::MOBILE_APP_SVID};
	std::string m_strLongitude;
	std::string m_strLatitude;

	virtual void marshal(sox::Pack &p) const
	{
		p << m_strLongitude << m_strLatitude;
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> m_strLongitude >> m_strLatitude;
	}
};

/*!
 * @class PCS_UploadLocationRes
 * @brief PCS_UploadLocation的响应
 */
struct PCS_UploadLocationRes: public sox::Marshallable
{
	enum {uri = (2 << 8) | protocol::MOBILE_APP_SVID};

	virtual void marshal(sox::Pack &p) const
	{
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
	}
};


enum ESearchType
{
	ENUM_SEARCH_NEARBY_USER = 0,	/**< 搜索在附近的用户 */
	ENUM_SEARCH_SAME_GAME_USER = 1,	/**< 搜索在附近在玩相同游戏的用户 */
};

struct CSearchResult: public sox::Marshallable
{
	uint32_t m_uUID;			/**< 用户ID	*/
	uint32_t m_uDistance;   	/**< 单位: 米 */
	std::string m_strLongitude;	/**< 经度 */
	std::string m_strLatitude;	/**< 纬度 */
	uint32_t m_uUpdateTime;		/**< 经纬度更新时间(32位unix时间戳) */

	virtual void marshal(sox::Pack &p) const
	{
		p << m_uUID << m_uDistance << m_strLongitude << m_strLongitude;
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> m_uUID >> m_uDistance >> m_strLongitude >> m_strLongitude;
	}
};

typedef std::vector<CSearchResult> SearchResultSet;

/*!
 * @class PCS_SearchNearbyUsers
 * @brief 获取在我附近的用户
 */
struct PCS_SearchNearbyUsers: public sox::Marshallable
{
	enum {uri = (3 << 8) | protocol::MOBILE_APP_SVID};
	std::string m_strLongitude;	/**< 经度 */
	std::string m_strLatitude;	/**< 纬度 */
	ESearchType m_uCatalogID;	/**< 搜索类型 */
	std::string m_strContext;			/**< 根据搜索分类的不同, 代表不同的意思 */
	uint32_t m_uMaxDistance;	/**< 搜索多少范围以内的用户 */
	uint32_t m_uMaxUserNum;		/**< 返回的数量限制 */

	virtual void marshal(sox::Pack &p) const
	{
		p << m_strLongitude << m_strLatitude << (uint32_t) m_uCatalogID << m_strContext << m_uMaxDistance << m_uMaxUserNum;
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> m_strLongitude >> m_strLatitude;
		m_uCatalogID = (ESearchType) p.pop_uint32();
		p >> m_strContext >> m_uMaxDistance >> m_uMaxUserNum;
	}
};

/*!
 * @class PCS_SearchNearbyUsersRes
 * @brief PCS_SearchNearbyUsersRes的响应, 返以及经纬度
 */
struct PCS_SearchNearbyUsersRes: public sox::Marshallable
{
	enum {uri = (4 << 8) | protocol::MOBILE_APP_SVID};

	ESearchType m_uCatalogID;
	std::string m_strContext;
	SearchResultSet m_vecResultSet;

	virtual void marshal(sox::Pack &p) const
	{
		p << (uint32_t) m_uCatalogID << m_strContext;
		sox::marshal_container(p, m_vecResultSet);
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		m_uCatalogID = (ESearchType) p.pop_uint32();
		p >> m_strContext;
		sox::unmarshal_container(p, std::back_inserter(m_vecResultSet));
	}
};

/*!
 * @class PCS_GetBuddyLocation
 * @brief 获取好友所在位置
 */
struct PCS_GetBuddyLocation: public sox::Marshallable
{
	enum {uri = (5 << 8) | protocol::MOBILE_APP_SVID};
	
	std::string m_strLongitude; /**< 经度 */
	std::string m_strLatitude;  /**< 纬度 */
	std::vector<uint32_t> m_vecUIDs;

	virtual void marshal(sox::Pack &p) const
	{
		p << m_strLongitude << m_strLatitude;
		sox::marshal_container(p, m_vecUIDs);
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> m_strLongitude >> m_strLatitude;
		sox::unmarshal_container(p, std::back_inserter(m_vecUIDs));
	}
};

/*!
 * @class PCS_GetBuddyLocationRes
 * @brief PCS_GetBuddyLocation的响应, 返回好友的位置信息
 */
struct PCS_GetBuddyLocationRes: public sox::Marshallable
{
	enum {uri = (6 << 8) | protocol::MOBILE_APP_SVID};
	SearchResultSet m_vecResultSet;

	virtual void marshal(sox::Pack &p) const
	{
		sox::marshal_container(p, m_vecResultSet);
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		sox::unmarshal_container(p, std::back_inserter(m_vecResultSet));
	}
};

/*!
 * @class PCS_UploadMyMobile
 * @brief 上传自己的手机号码(hash值)
 */
struct PCS_UploadMyMobile: public sox::Marshallable
{
	enum {uri = (7 << 8) | protocol::MOBILE_APP_SVID};
	std::string m_strMobile;

	virtual void marshal(sox::Pack &p) const
	{
		p << m_strMobile;
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> m_strMobile;
	}
};

enum EMobileRelation
{
	ENUM_NOT_CONTACT		= 0,	/**< 双方通讯录没有交集 */
	ENUM_IN_MY_CONTACT		= 1,	/**< 对方号码在我的通讯录内 */
	ENUM_IN_OTHER_CONTACT	= 2,	/**< 我的号码在对方通讯录内 */
	ENUM_IN_BOTH_CONTACT	= ENUM_IN_MY_CONTACT | ENUM_IN_OTHER_CONTACT,	/**< 双方都有对方的号码 */
};

struct CMobileRelation: public sox::Marshallable
{
	uint32_t m_uBuddyID;
	EMobileRelation m_enRelation;

	virtual void marshal(sox::Pack &p) const
	{
		p << m_uBuddyID;
		p.push_uint8((uint8_t) m_enRelation);
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> m_uBuddyID;
		m_enRelation = (EMobileRelation) p.pop_uint8();
	}
};

struct PCS_UploadMyMobileRes: public sox::Voidmable
{
	enum {uri = (8 << 8) | protocol::MOBILE_APP_SVID};
};

/*!
 * @class PCS_UploadContactMobile
 * @brief 上传自己的手机通讯录(联系人的手机号码的hash值)
 */
struct PCS_UploadContactMobile: public sox::Marshallable
{
	enum {uri = (9 << 8) | protocol::MOBILE_APP_SVID};
	std::vector<std::string> m_vecMobileList;

	virtual void marshal(sox::Pack &p) const
	{
		sox::marshal_container(p, m_vecMobileList);
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		sox::unmarshal_container(p, std::back_inserter(m_vecMobileList));
	}
};

struct PCS_UploadContactMobileRes: public sox::Voidmable
{
	enum {uri = (10 << 8) | protocol::MOBILE_APP_SVID};
};

/*!
 * @class PCS_SearchMobileBuddy
 * @brief 搜索手机号码与自己通讯录中号码相同的用户
 */
struct PCS_SearchMobileBuddy: public sox::Voidmable
{
	enum {uri = (11 << 8) | protocol::MOBILE_APP_SVID};
};

struct PCS_SearchMobileBuddyRes: public sox::Marshallable
{
	enum {uri = (12 << 8) | protocol::MOBILE_APP_SVID};
	std::vector<CMobileRelation> m_vecBuddyRelation;

	virtual void marshal(sox::Pack &p) const
	{
		sox::marshal_container(p, m_vecBuddyRelation);
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		sox::unmarshal_container(p, std::back_inserter(m_vecBuddyRelation));
	}
};

enum EMessageType
{
	ENUM_TINYBLOG_POST = 0,
	ENUM_TINYBLOG_REPLY = 1,
	ENUM_TINYBLOG_FORWARD = 2,
};

struct CBroadcastIndex: public sox::Marshallable
{
	uint64_t m_uMsgID;		/**< 消息的唯一ID */
	EMessageType m_enType;	/**< 消息类型 */
	uint32_t m_uSenderID;	/**< 发起者ID */
	uint32_t m_uCreateTime;	/**< 广播创建的时间(UTC) */
	uint32_t m_uReplyCount;		/**< 回复次数 */
	uint32_t m_uForwardCount;	/**< 转发次数 */
	uint64_t m_uParentMsgID;	/**< (类型为转发, 回复时用到) */
	uint64_t m_uReqID;

	virtual void marshal(sox::Pack &p) const
	{
		p << m_uMsgID;
		p.push_uint32((uint32_t) m_enType);
		p << m_uSenderID << m_uCreateTime << m_uReplyCount << m_uForwardCount << m_uParentMsgID << m_uReqID;
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> m_uMsgID;
		m_enType = (EMessageType) p.pop_uint32();
		p >> m_uSenderID >> m_uCreateTime >> m_uReplyCount >> m_uForwardCount >> m_uParentMsgID >> m_uReqID;
	}
};

struct CBroadcastMessage: public sox::Marshallable
{
	uint64_t m_uMsgID;	/**< 消息ID */
	std::string m_strContext;	/**< 消息内容 */

	virtual void marshal(sox::Pack &p) const
	{
		p << m_uMsgID << m_strContext;
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> m_uMsgID >> m_strContext;
	}
};

/*!
 * @class PCS_SendBroadcast
 * @brief 发送广播
 */
struct PCS_SendBroadcast: public sox::Marshallable
{
	enum {uri = (13 << 8) | protocol::MOBILE_APP_SVID};
	EMessageType m_enType;	/**< 广播类型 */
	uint64_t m_uReqID;	/**< 由客户端生成 */
	uint64_t m_uParentMsgID;      /**< 原来的消息ID(转发, 回复时才用到) */
	uint32_t m_uSenderID;   /**< 原来的发送者(转发, 回复时才用到) */
	std::string m_strContext;   /**< 二进制的消息正文 */

	virtual void marshal(sox::Pack &p) const
	{
		p.push_uint32((uint32_t) m_enType);
		p << m_uReqID << m_uParentMsgID << m_uSenderID << m_strContext;
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		m_enType = (EMessageType) p.pop_uint32();
		p >> m_uReqID >> m_uParentMsgID >> m_uSenderID >> m_strContext;
	}
};

struct PCS_SendBroadcastRes: public sox::Marshallable
{
	enum {uri = (14 << 8) | protocol::MOBILE_APP_SVID};
	EMessageType m_enType;
	uint64_t m_uMsgID;
	uint64_t m_uReqID;

	virtual void marshal(sox::Pack &p) const
	{
		p.push_uint32((uint32_t) m_enType);
		p << m_uMsgID << m_uReqID;
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		m_enType = (EMessageType) p.pop_uint32();
		p >> m_uMsgID >> m_uReqID;
	}
};

/*!
 * @class PCS_GetBroadcastIndex
 * @brief 获取自己的广播索引列表
 */
struct PCS_GetBroadcastIndex: public sox::Marshallable
{
	enum {uri = (15 << 8) | protocol::MOBILE_APP_SVID};
	uint64_t m_uBeginPos;	//  
	uint64_t m_uMaxNum;		//
    std::vector<uint32_t> m_vecBuddyList;

	virtual void marshal(sox::Pack &p) const
	{
		p << m_uBeginPos << m_uMaxNum << m_vecBuddyList;
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> m_uBeginPos >> m_uMaxNum >> m_vecBuddyList;
	}
};

struct PCS_GetBroadcastIndexRes: public sox::Marshallable
{
	enum {uri = (16 << 8) | protocol::MOBILE_APP_SVID};
	std::vector<CBroadcastIndex> m_vecIndexList;

	virtual void marshal(sox::Pack &p) const
	{
		marshal_container(p, m_vecIndexList);
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		unmarshal_container(p, std::back_inserter(m_vecIndexList));
	}
};

/*!
 * @class PCS_GetBroadcastMessage
 * @brief 获取广播消息
 */
struct PCS_GetBroadcastMessage: public sox::Marshallable
{
	enum {uri = (17 << 8) | protocol::MOBILE_APP_SVID};
	std::vector<uint64_t> m_vecMsgID;

	virtual void marshal(sox::Pack &p) const
	{
		marshal_container(p, m_vecMsgID);
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		unmarshal_container(p, std::back_inserter(m_vecMsgID));
	}
};

struct PCS_GetBroadcastMessageRes: public sox::Marshallable
{
	enum {uri = (18 << 8) | protocol::MOBILE_APP_SVID};
	std::vector<CBroadcastMessage> m_vecMsgList;

	virtual void marshal(sox::Pack &p) const
	{
		marshal_container(p, m_vecMsgList);
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		unmarshal_container(p, std::back_inserter(m_vecMsgList));
	}
};

struct PCS_GetBroadcastReplyIndex: public sox::Marshallable
{
	enum {uri = (19 << 8) | protocol::MOBILE_APP_SVID};
	uint64_t m_uMsgID;
	uint32_t m_uBeginPos;	//
	uint64_t m_uMaxNum;		//

	virtual void marshal(sox::Pack &p) const
	{
		p << m_uMsgID << m_uBeginPos << m_uMaxNum;
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> m_uMsgID >> m_uBeginPos >> m_uMaxNum;
	}
};

struct PCS_GetBroadcastReplyIndexRes: public sox::Marshallable
{
	enum {uri = (20 << 8) | protocol::MOBILE_APP_SVID};
	std::vector<CBroadcastIndex> m_vecIndexList;

	virtual void marshal(sox::Pack &p) const
	{
		marshal_container(p, m_vecIndexList);
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		unmarshal_container(p, std::back_inserter(m_vecIndexList));
	}
};

struct PCS_BroadcastReplyNotify: public sox::Marshallable
{
	enum {uri = (21 << 8) | protocol::MOBILE_APP_SVID};
	uint64_t m_uParentMsgID;
	uint64_t m_uMsgID;
	uint32_t m_uSenderID;

	virtual void marshal(sox::Pack &p) const
	{
		p << m_uParentMsgID << m_uMsgID << m_uSenderID;
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> m_uParentMsgID >> m_uMsgID >> m_uSenderID;
	}
};

struct PCS_BroadcastReplyNotifyForBuddy: public PCS_BroadcastReplyNotify
{
	enum {uri = (23 << 8) | protocol::MOBILE_APP_SVID};

	virtual void marshal(sox::Pack &p) const {
		protocol::mobile::PCS_BroadcastReplyNotify::marshal(p);
	}

	virtual void unmarshal(const sox::Unpack &p) {
		protocol::mobile::PCS_BroadcastReplyNotify::unmarshal(p);
	}
};

// transmit
struct PCS_PublishNewBroadcast : public sox::Marshallable
{
    enum {uri = (22 << 8) | protocol::MOBILE_APP_SVID};
    EMessageType m_enType;
    uint64_t m_uMsgID;
    uint32_t m_uSender;
    
    virtual void marshal(sox::Pack &p) const
    {
		p.push_uint32((uint32_t) m_enType);
        p << m_uMsgID << m_uSender;
    }

    virtual void unmarshal(const sox::Unpack &p)
    {
        m_enType = (EMessageType) p.pop_uint32();
        p >> m_uMsgID >> m_uSender;
    }
};

enum EOsType {
	IOS	= 1,
	ANDROID = 2,
};

struct PCS_DeviceInfo : public sox::Marshallable
{
	uint32_t m_uUID;
	std::string	m_strDeviceID;
	uint32_t m_uUpdateTime;
	EOsType	m_eType;

	virtual void marshal(sox::Pack &p) const
	{
		p << m_uUID << m_strDeviceID << m_uUpdateTime;
		p.push_uint32((uint32_t) m_eType);
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> m_uUID >> m_strDeviceID >> m_uUpdateTime;
		m_eType = (EOsType) p.pop_uint32();
	}
};

struct PCS_SetDeviceInfo : public PCS_DeviceInfo
{
	enum {uri = (24 << 8) | protocol::MOBILE_APP_SVID};

	virtual void marshal(sox::Pack &p) const
	{
		PCS_DeviceInfo::marshal(p);
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		PCS_DeviceInfo::unmarshal(p);
	}
};

struct PCS_SetDeviceInfoRes : public sox::Marshallable
{
	enum {uri = (25 << 8) | protocol::MOBILE_APP_SVID};
};

struct PCS_GetDeviceInfo : public sox::Marshallable
{
	enum {uri = (26 << 8) | protocol::MOBILE_APP_SVID};
	uint32_t m_uUID;

	virtual void marshal(sox::Pack &p) const
	{
		p << m_uUID;
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> m_uUID;
	}
};

struct PCS_GetDeviceInfoRes : public PCS_DeviceInfo
{
	enum {uri = (27 << 8) | protocol::MOBILE_APP_SVID};

	virtual void marshal(sox::Pack &p) const
	{
		PCS_DeviceInfo::marshal(p);
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		PCS_DeviceInfo::unmarshal(p);
	}
};


struct PCS_GetBatchMsgIDReq : public sox::Marshallable
{
	enum {uri = (28 << 8) | protocol::MOBILE_APP_SVID};
	uint64_t m_uBeginID;
	uint64_t m_uEndID;
	uint64_t m_uCount;
	std::vector<uint32_t> m_setUids;

	virtual void marshal(sox::Pack &p) const
	{
		p << m_uBeginID << m_uEndID << m_uCount << m_setUids;
		marshal_container(p, m_setUids);
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> m_uBeginID >> m_uEndID >> m_uCount >> m_setUids;
		unmarshal_container(p, std::back_inserter(m_setUids));
	}
};

struct PCS_GetBatchMsgIDRes : public sox::Marshallable
{
	enum {uri = (29 << 8) | protocol::MOBILE_APP_SVID};
	std::vector<uint64_t> m_vecMsgID;

	virtual void marshal(sox::Pack &p) const
	{
		marshal_container(p, m_vecMsgID);
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		unmarshal_container(p, std::back_inserter(m_vecMsgID));
	}
};

struct PCS_GetBroadcastIndexByMsgIDs : public sox::Marshallable
{
	enum {uri = (30 << 8) | protocol::MOBILE_APP_SVID};
	std::vector<uint64_t> m_vecMsgID;

	virtual void marshal(sox::Pack &p) const
	{
		marshal_container(p, m_vecMsgID);
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		unmarshal_container(p, std::back_inserter(m_vecMsgID));
	}
};

struct PCS_GetBroadcastIndexByMsgIDsRes : public PCS_GetBroadcastIndexRes
{
	enum {uri = (31 << 8) | protocol::MOBILE_APP_SVID};

	virtual void marshal(sox::Pack &p) const
	{
		PCS_GetBroadcastIndexRes::marshal(p);
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		PCS_GetBroadcastIndexRes::unmarshal(p);
	}
};

struct PCS_VoiceCallCap : public sox::Marshallable
{
    enum { uri = (40 << 8) | protocol::MOBILE_APP_SVID };
    uint32_t sender;
    bool speex_mod4;
    bool signal_frame;
    virtual void marshal(sox::Pack &p) const
    {
        p << sender;
        p.push_uint8(speex_mod4);
        p.push_uint8(signal_frame);
    }

    virtual void unmarshal(const sox::Unpack &p)
    {
        p >> sender;
        speex_mod4 = (bool)p.pop_uint8();
        signal_frame = (bool)p.pop_uint8();
    }
};

struct PCS_VoiceCallCapRes : public PCS_VoiceCallCap
{
    enum { uri = (41 << 8) | protocol::MOBILE_APP_SVID };
};


}	//mobile
}	//protocol
#endif	//PROTOTOCOL_MOBILE_H
