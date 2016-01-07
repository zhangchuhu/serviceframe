#ifndef PGMSG_CACHE_TYPES_H
#define PGMSG_CACHE_TYPES_H

#include <string>
#include "common/protocol/const.h"
#include "common/packet.h"
#include "common/protocol/pgtopicbase.h"
#include "common/protocol/PGTopicMsg.h"

//#define MAX_TOPIC_RECENT_MSG  30
//#define MAX_RECENT_TOPIC_NUM  100
//#define MIN_RECENT_TOPIC_NUM  90

namespace protocol
{
	namespace gmsgcache
	{
		typedef protocol::gtopic::GIDFIDTID CRelationID;

		struct CTopicInfo2 : public sox::Marshallable
		{
			CRelationID    m_Ids;            // group/folder/topic ID
			uint64_t       m_oldtime;        // eldest time stamp of topic msgs(microsecond level)
			uint64_t       m_newtime;        // latest time stamp of topic msgs(microsecond level)
			uint32_t       m_msgnum;         // msg number should not be bigger than 30

			CTopicInfo2(){}
			CTopicInfo2(CRelationID ids, uint64_t oldtime, uint64_t newtime, uint32_t msgnum)
				: m_msgnum(msgnum)
			{
				m_Ids = ids;
				m_oldtime = oldtime;
				m_newtime = newtime;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_oldtime << m_newtime << m_msgnum;
				m_Ids.marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_oldtime >> m_newtime >> m_msgnum;
				m_Ids.unmarshal(p);
			}
		};

		
		// store topic popular info
		struct STopicPopInfo : public sox::Marshallable
		{
			uint32_t       m_uTopicId;      // topic id
			uint32_t       m_uSumMsgNum;    // sum msg number
			uint32_t       m_uUpdateTime;   // latesttime

			STopicPopInfo(){}
			STopicPopInfo(uint32_t tid, uint32_t sumNum, uint32_t time)
				: m_uTopicId(tid), m_uSumMsgNum(sumNum), m_uUpdateTime(time)
			{}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uTopicId << m_uSumMsgNum << m_uUpdateTime;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uTopicId >> m_uSumMsgNum >> m_uUpdateTime;
			}
		};

		// store popular info of DCA
		struct SChatPopInfo : public sox::Marshallable
		{
			uint32_t	m_uSumMsgNum;    // sum msg number
			uint32_t	m_uUpdateTime;   // latesttime

			SChatPopInfo(){}
			SChatPopInfo(uint32_t sumNum, uint32_t time)
				: m_uSumMsgNum(sumNum), m_uUpdateTime(time)
			{}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uSumMsgNum << m_uUpdateTime;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uSumMsgNum >> m_uUpdateTime;
			}
		};

		struct CTopicInfo : public sox::Marshallable
		{
			CRelationID    m_Ids;            // group/folder/topic ID
			uint64_t       m_oldtime;        // eldest time stamp of topic msgs(microsecond level)
			uint64_t       m_newtime;        // latest time stamp of topic msgs(microsecond level)
			uint32_t       m_msgnum;         // msg number should not be bigger than 30
			uint32_t       m_updatecount;    // updated msg count should not be bigger than 30

			CTopicInfo(){}
			CTopicInfo(CRelationID ids, uint64_t oldtime, uint64_t newtime, uint32_t msgnum, uint32_t count)
				: m_msgnum(msgnum), m_updatecount(count)
			{
				m_Ids = ids;
				m_oldtime = oldtime;
				m_newtime = newtime;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_oldtime << m_newtime << m_msgnum << m_updatecount;
				m_Ids.marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_oldtime >> m_newtime >> m_msgnum >> m_updatecount;
				m_Ids.unmarshal(p);
			}
		};

		struct CGChatInfo : public sox::Marshallable
		{
			uint64_t       m_oldtime;        // eldest time stamp of topic msgs(microsecond level)
			uint64_t       m_newtime;        // latest time stamp of topic msgs(microsecond level)
			uint32_t       m_msgnum;         // msg number should not be bigger than 30
			uint32_t       m_updatecount;    // updated msg count should not be bigger than 30

			CGChatInfo(){}
			CGChatInfo(uint64_t oldtime, uint64_t newtime, uint32_t msgnum, uint32_t count)
				: m_oldtime(oldtime), m_newtime(newtime), m_msgnum(msgnum), m_updatecount(count)
			{
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_oldtime << m_newtime << m_msgnum << m_updatecount;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_oldtime >> m_newtime >> m_msgnum >> m_updatecount;
			}
		};

		//store uread topic number
		struct SUnreadTopicNum : public sox::Marshallable
		{
			uint32_t       m_uGroupId;          //group Id
			uint32_t       m_uFolderId;         // topic Id
			uint32_t       m_uLatestTime;       // sum msg number
			uint32_t       m_uUnreadTopicNum;   // unread topic number

			SUnreadTopicNum(){}
			SUnreadTopicNum(uint32_t groupId, uint32_t folderId, uint32_t time, uint32_t num)
				: m_uGroupId(groupId), m_uFolderId(folderId), m_uLatestTime(time), m_uUnreadTopicNum(num)
			{}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId << m_uFolderId << m_uLatestTime << m_uUnreadTopicNum;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uGroupId >> m_uFolderId >> m_uLatestTime >> m_uUnreadTopicNum;
			}
		};
/*
		//topit id and update time
		struct STidTime : public sox::Marshallable
		{
			uint32_t m_uTopicId;
			uint32_t m_uTime;

			STidTime(){}
			STidTime(uint32_t tid, uint32_t time)
				: m_uTopicId(tid), m_uTime(time)
			{}
			virtual void marshal(sox::Pack &p) const
			{
				p << m_uTopicId << m_uTime;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uTopicId >> m_uTime;
			}
		};
*/
		// msg record of:  1) default chat area 
		//				   2) topics
		struct CServerTopicMsg : public sox::Marshallable
		{
			bool         m_update;       // true identify the msg has been updated by admin
			uint32_t     m_senderUid;    // send uid
			uint32_t     m_sendTime;     // send time
			uint64_t     m_timestamp;    // timestamp microsecond level
			protocol::gmsgcache::GTopicTextChat  m_msgText;

			CServerTopicMsg(){}
			CServerTopicMsg(bool update, uint32_t uid, uint32_t sendtime, uint64_t timestamp, const protocol::gmsgcache::GTopicTextChat& txt)
				: m_update(update), m_senderUid(uid), m_sendTime(sendtime), m_timestamp(timestamp)
			{
				m_msgText = txt;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_update << m_senderUid << m_sendTime << m_timestamp;
				m_msgText.marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_update >> m_senderUid >> m_sendTime >> m_timestamp;
				m_msgText.unmarshal(p);
			}
		};
		
		// old definition for compatibility
		struct CServerTopicMsg2 : public sox::Marshallable
		{
			uint32_t     m_senderUid;    // send uid
			uint32_t     m_sendTime;     // send time
			uint64_t     m_timestamp;    // timestamp microsecond level
			protocol::gmsgcache::GTopicTextChat  m_msgText;

			CServerTopicMsg2(){}
			CServerTopicMsg2(uint32_t uid, uint32_t sendtime, uint64_t timestamp, const protocol::gmsgcache::GTopicTextChat& txt)
				: m_senderUid(uid), m_sendTime(sendtime), m_timestamp(timestamp)
			{
				m_msgText = txt;
			}
			
			CServerTopicMsg2& operator=(const CServerTopicMsg& input)
			{
				m_senderUid = input.m_senderUid;
				m_sendTime = input.m_sendTime;
				m_timestamp = input.m_timestamp;
				
				m_msgText = input.m_msgText;

				return *this;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_senderUid << m_sendTime << m_timestamp;
				m_msgText.marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >>  m_senderUid >> m_sendTime >> m_timestamp;
				m_msgText.unmarshal(p);
			}
		};
	}
}

#endif
