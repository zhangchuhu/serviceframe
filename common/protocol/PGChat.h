#pragma once

#include <vector>
#include <string>
#include "iproperty.h"
#include "protocol/const.h"
#include "core/base_svid.h"
#include "common/packet.h"
#include "common/protocol/PGTopicMsg.h"

using namespace protocol::gmsgcache;

namespace protocol
{
	namespace gchat
	{
		// topic msg
		struct PCS_GTopicMsg : public sox::Marshallable
		{
			enum {uri = (1 << 8) | protocol::GCHAT_SVID};

			//gchat fills m_sendTime, m_guid, m_imlinkdId

			uint32_t     m_topicId;      // topic id
			uint32_t     m_groupId;      // group id
			uint32_t     m_folderId;     // folder id
			uint32_t     m_seqId;        // sequence id
			uint32_t     m_imlinkdId;    // imlinkdid, for PCS_GTopicMsgRes, is filled by gchat

			uint32_t     m_senderUid;    // send uid
			uint32_t     m_sendTime;     // send time
			uint64_t     m_timestamp;    // timestamp microsecond level
			protocol::gmsgcache::GTopicTextChat  m_msgText;

			PCS_GTopicMsg(){}
			PCS_GTopicMsg(uint32_t topicId, uint32_t groupId, uint32_t folderId, uint32_t seqId, const GTopicTextChat& text)
				: m_topicId(topicId), m_groupId(groupId), m_folderId(folderId), m_seqId(seqId)
			{
				m_msgText = text;
				m_imlinkdId = 0;
				m_senderUid = 0;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_topicId << m_groupId << m_folderId << m_seqId << m_imlinkdId << m_senderUid << m_sendTime << m_timestamp;
				m_msgText.marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_topicId >> m_groupId >> m_folderId >> m_seqId >> m_imlinkdId >> m_senderUid >> m_sendTime >> m_timestamp;
				m_msgText.unmarshal(p);
			}
		};

		// response of topic msg
		struct PCS_GTopicMsgRes : public sox::Marshallable 
		{
			enum {uri = (2 << 8) | protocol::GCHAT_SVID};
			
			uint32_t     m_topicId;      // topic id
			uint32_t     m_groupId;      // group id
			uint32_t     m_folderId;     // folder id
			uint32_t     m_seqId;        // sequence id

			uint32_t     m_sendtime;     // sendtime which is written by gchat or gtransfer			
			uint64_t     m_timestamp;    // timestamp which is written by gchat or gtransfer

			PCS_GTopicMsgRes() : m_topicId(-1), m_groupId(-1), m_folderId(-1), m_seqId(-1)
			{
			}
			PCS_GTopicMsgRes(uint32_t topicId, uint32_t groupId, uint32_t folderId, uint32_t seqId, uint32_t sendtime, uint64_t time)
				: m_topicId(topicId), m_groupId(groupId), m_folderId(folderId), m_seqId(seqId), m_sendtime(sendtime), m_timestamp(time)
			{}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_topicId << m_groupId << m_folderId << m_seqId << m_sendtime << m_timestamp;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_topicId >> m_groupId >> m_folderId >> m_seqId >> m_sendtime >> m_timestamp;
			}
		};

		// chat msg
		struct PCS_GChatMsg : public sox::Marshallable
		{
			enum {uri = (3 << 8) | protocol::GCHAT_SVID};

			// gchat fills in m_sendTime, m_timestamp, m_imlinkdId
			uint32_t     m_groupId;      // group id
			uint32_t     m_folderId;     // folder id
			uint32_t     m_seqId;        // sequence id
			uint32_t     m_imlinkdId;    // imlinkdid, for PCS_GTopicMsgRes
			uint32_t     m_senderUid;    // send uid
			uint32_t     m_sendTime;     // send time
			uint64_t     m_timestamp;    // timestamp microsecond level
			protocol::gmsgcache::GTopicTextChat  m_msgText;
			bool         m_bSync;
			uint64_t     m_uStatTimeStamp;

			PCS_GChatMsg()
			{
				m_imlinkdId = 0;
				m_senderUid = 0;
				m_bSync = false;
				m_uStatTimeStamp = 0;
			}
			PCS_GChatMsg(uint32_t groupId, uint32_t folderId, uint32_t seqId, const GTopicTextChat& text)
				: m_groupId(groupId), m_folderId(folderId), m_seqId(seqId)
			{
				m_msgText = text;
				m_imlinkdId = 0;
				m_senderUid = 0;
				m_bSync = false;
				m_uStatTimeStamp = 0;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_groupId << m_folderId << m_seqId << m_imlinkdId << m_senderUid << m_sendTime << m_timestamp;
				m_msgText.marshal(p);
				p.push_uint8(m_bSync);
				p << m_uStatTimeStamp;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_groupId >> m_folderId >> m_seqId >> m_imlinkdId >> m_senderUid >> m_sendTime >> m_timestamp;
				m_msgText.unmarshal(p);
				if (p.size() != 0)
				{
					m_bSync = (bool)p.pop_uint8();
				}
				if (p.size() != 0)
				{
					p >> m_uStatTimeStamp;
				}
			}
		};

		// response of chat msg
		struct PCS_GChatMsgRes : public sox::Marshallable 
		{
			enum {uri = (4 << 8) | protocol::GCHAT_SVID};
			
			uint32_t     m_groupId;      // group id
			uint32_t     m_folderId;     // folder id
			uint32_t     m_seqId;        // sequence id

			uint32_t     m_sendtime;     // sendtime which is written by gchat or gtransfer			
			uint64_t     m_timestamp;    // timestamp which is written by gchat or gtransfer

			PCS_GChatMsgRes() : m_groupId(-1), m_folderId(-1), m_seqId(-1)
			{
			}
			PCS_GChatMsgRes(uint32_t groupId, uint32_t folderId, uint32_t seqId, uint32_t sendtime, uint64_t time)
				: m_groupId(groupId), m_folderId(folderId), m_seqId(seqId), m_sendtime(sendtime), m_timestamp(time)
			{}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_groupId << m_folderId << m_seqId << m_sendtime << m_timestamp;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_groupId >> m_folderId >> m_seqId >> m_sendtime >> m_timestamp;
			}
		};
		
		enum EAuthResult // enumerations of auth results 
		{
			ENUM_USR_IS_BANNED_TO_POST = 0,
			ENUM_UNKNOWN_AUTH_RESULT
		};
		// response of chat msg, send from GAuth if auth failed
		struct PCS_GChatMsgAuthRes : public sox::Marshallable 
		{
			enum {uri = (5 << 8) | protocol::GCHAT_SVID};
			
			uint32_t     m_groupId;      // group id
			uint32_t     m_folderId;     // folder id
			uint32_t     m_seqId;        // sequence id
			EAuthResult  m_authRes;		 // auth result

			PCS_GChatMsgAuthRes() : m_groupId(-1), m_folderId(-1), m_seqId(-1), m_authRes(ENUM_UNKNOWN_AUTH_RESULT)
			{}

			PCS_GChatMsgAuthRes(uint32_t groupId, uint32_t folderId, uint32_t seqId, EAuthResult authRes)
				: m_groupId(groupId), m_folderId(folderId), m_seqId(seqId), m_authRes(authRes) 
			{}

			PCS_GChatMsgAuthRes(PCS_GChatMsg& obj, EAuthResult authRes)
				: m_groupId(obj.m_groupId), m_folderId(obj.m_folderId), m_seqId(obj.m_seqId), m_authRes(authRes) 
			{}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_groupId << m_folderId << m_seqId;
				p.push_uint8(m_authRes);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_groupId >> m_folderId >> m_seqId;
				m_authRes = (EAuthResult)p.pop_uint8();
			}
		};

		// protocol -> GAuth
		// request group/folder activity(latest reply time) list
		struct PCS_GetGrpFldActivityList : public sox::Marshallable 
		{
			enum {uri = (6 << 8) | protocol::GCHAT_SVID};
			
			uint32_t     m_groupId;    
			uint32_t     m_folderId;

			PCS_GetGrpFldActivityList() : m_groupId(-1), m_folderId(-1)
			{
			}
			PCS_GetGrpFldActivityList(uint32_t groupId, uint32_t folderId)
				: m_groupId(groupId), m_folderId(folderId)
			{}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_groupId << m_folderId;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_groupId >> m_folderId;
			}
		};

		// GAuth -> protocol
		// response of group/folder activity(latest reply time) list
		struct PCS_GetGrpFldActivityListRes : public sox::Marshallable 
		{
			enum {uri = (7 << 8) | protocol::GCHAT_SVID};
			
			uint32_t     m_groupId;      // group id
			uint32_t     m_folderId;     // folder id

			// key(uid) => latest reply time
			std::map<uint32_t, uint32_t> m_mapUid2LastReplyTime;

			PCS_GetGrpFldActivityListRes() : m_groupId(-1), m_folderId(-1){}
			PCS_GetGrpFldActivityListRes(uint32_t groupId, uint32_t folderId,
										 const std::map<uint32_t, uint32_t>& mapUid2LastReplyTime)
				: m_groupId(groupId), m_folderId(folderId), m_mapUid2LastReplyTime(mapUid2LastReplyTime)
			{}

			PCS_GetGrpFldActivityListRes(const PCS_GetGrpFldActivityList& objReq, 
										 const std::map<uint32_t, uint32_t>& mapUid2LastReplyTime)
				: m_groupId(objReq.m_groupId), m_folderId(objReq.m_folderId), m_mapUid2LastReplyTime(mapUid2LastReplyTime)
			{}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_groupId << m_folderId;
				sox::marshal_container(p, m_mapUid2LastReplyTime);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_groupId >> m_folderId;
				sox::unmarshal_container(p, std::inserter(m_mapUid2LastReplyTime, m_mapUid2LastReplyTime.begin()));
			}
		};

		// response of chat msg, send from GAuth if auth failed
		struct PCS_GChatMsgBanRes : public sox::Marshallable 
		{
			enum {uri = (8 << 8) | protocol::GCHAT_SVID};

			uint32_t     m_groupId;      // group id
			uint32_t     m_folderId;     // folder id
			uint32_t     m_seqId;        // sequence id
			uint32_t     m_banTime;      // ban time

			PCS_GChatMsgBanRes() : m_groupId(-1), m_folderId(-1), m_seqId(-1), m_banTime(0)
			{}

			PCS_GChatMsgBanRes(uint32_t groupId, uint32_t folderId, uint32_t seqId, uint32_t banTime)
				: m_groupId(groupId), m_folderId(folderId), m_seqId(seqId), m_banTime(banTime)
			{}

			PCS_GChatMsgBanRes(PCS_GChatMsg& obj, uint32_t banTime)
				: m_groupId(obj.m_groupId), m_folderId(obj.m_folderId), m_seqId(obj.m_seqId), m_banTime(banTime) 
			{}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_groupId << m_folderId << m_seqId << m_banTime;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_groupId >> m_folderId >> m_seqId >> m_banTime;
			}
		};
	}
}


