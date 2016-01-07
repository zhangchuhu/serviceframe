#ifndef IGCHATMSG_CACHE_H
#define IGCHATMSG_CACHE_H

#include <vector>
#include "common/protocol/PGTopicMsg.h"
#include "common/protocol/PGMsgCacheType.h"

namespace protocol
{
	namespace gmsgcache
	{
		// default chat msg for client
		struct CClientChatMsg
		{
			bool       m_update;                  // true identify msg has been updated by admin
			uint32_t   m_uid;                     // sender		
			uint32_t   m_sendTime;                // send time	
			uint64_t   m_timestamp;               // timestamp, microsecond level, identify this msg
			protocol::gmsgcache::GTopicTextChat m_text;      // chat text

			CClientChatMsg(){}
			CClientChatMsg(bool update, uint32_t uid, uint32_t time, uint64_t timestamp, protocol::gmsgcache::GTopicTextChat& text)
				: m_update(update), m_uid(uid), m_sendTime(time), m_timestamp(timestamp)
			{
				m_text = text;
			}
		};

		// default chat msgs for client
		struct CClientChatMsgRecord
		{
			uint32_t                      m_groupid;
			uint32_t                      m_folderid;
			uint32_t                      m_update;     // true: ui need to update its msgs, false: need not
			uint64_t                      m_eldesttime; // eldest timestamp of msgs of ui
			uint64_t                      m_latesttime; // latest timestamp of msgs of ui
			std::vector<CClientChatMsg>   m_vecMsg;     // msgs from server

			CClientChatMsgRecord() {}
			CClientChatMsgRecord(uint32_t gid, 
										  uint32_t fid, 
										  const uint32_t update, 
										  const uint64_t oldtime, 
										  const uint64_t newtime, 
										  const std::vector<CClientChatMsg>& vecMsg)
			{
				m_groupid = gid;
				m_folderid = fid;
				m_update = update;
				m_eldesttime = oldtime;
				m_latesttime = newtime;
				m_vecMsg = vecMsg;
			}
		};

		//------ new client structure since yy3.0beta6
		// Chat msg entity
		struct CClientLatestMsg
		{
			uint32_t   m_uid;                     			// sender		
			uint32_t   m_sendTime;                			// send time	
			uint64_t   m_timestamp;               			// timestamp, microsecond level, identify this msg
			protocol::gmsgcache::GTopicTextChat m_text;     // chat text

			CClientLatestMsg(){}
			CClientLatestMsg(uint32_t uid, uint32_t time, uint64_t timestamp, protocol::gmsgcache::GTopicTextChat& text)
				: m_uid(uid), m_sendTime(time), m_timestamp(timestamp)
			{
				m_text = text;
			}
		};

		// msg record of chat msg
		struct CClientLatestMsgRecord
		{
			uint32_t                      	m_groupid;
			uint32_t                      	m_folderid;
			std::vector<CClientLatestMsg>   m_vecMsg;     // msgs from server

			CClientLatestMsgRecord() {}
			CClientLatestMsgRecord(uint32_t gid, uint32_t fid, const std::vector<CClientLatestMsg>& vecMsg)
				: m_groupid(gid), m_folderid(fid), m_vecMsg(vecMsg)
			{}
		};
//////////////////////////////////////////////////////////////////////////


		class IGChatMsgCacheWatcher
		{
		public:
			// get recent msgs of default chat area
			// deprecated since yy3.0 beta6, replaced by OnGChatLatestMsgRes
			virtual void OnGChatMsgRecentRes(const CClientChatMsgRecord& Record) = 0;

			// since yy3.0 beta6: get latest msgs of chat msgs
			virtual void OnGChatLatestMsgRes(const CClientLatestMsgRecord& Record) = 0;

			// chat msg arrive
			virtual void OnGChatMsgArrive(const uint32_t& senderUid,
									const uint32_t& sendTime,
									const uint64_t& timestamp,
									const uint32_t& groupId,
									const uint32_t& folderId,
									const protocol::gmsgcache::GTopicTextChat& text) = 0;

			//  chat msg timeout
			virtual void OnGChatMsgTimeout(const uint32_t& groupId,
									const uint32_t& folderId,
									const uint32_t& seqId,
									const protocol::gmsgcache::GTopicTextChat& text) = 0;

			// chat msg response arrive
			virtual void OnChatMsgRes(const uint32_t& groupId,
									const uint32_t& folderId,
									const uint32_t& seqId,
									const uint32_t& sendtime,
									const uint64_t& timestamp) = 0;

			// notify ui the result of getting default chat area latest update time and msg count
			virtual void onGetGChatPopInfo(const uint32_t& uGrpID, const uint32_t& uFldrID, 
				const protocol::gmsgcache::SChatPopInfo& ChatPopInfo)=0;

			// Notify ui if chat msg auth failed, a.k.a. usr is banned to post.
			// it probablely happens when 
			// 1) protocol authenticate user as banned to post.
			// 		UI should give a hint telling the user that he had been banned.
			// 2) protocol hasn't got banned-to-post list yet. So the msg is sent to server.
			// 		UI should give a hint telling the user he had been banned, and update "banned sign" with member list.
			virtual void OnGChatMsgBanned(const uint32_t& groupId,
									const uint32_t& folderId) = 0;

			virtual void OnGCHatMsgBannedTemp(const uint32_t& groupId,const uint32_t& folderId,
									const uint32_t& banTime){};

			// Notify UI if recv group/folder activity(members' last reply time) list.
			// UI should traversal the mapUid2LastReplyTime to find out last reply time of each member.
			// UI also need to convert the mapUid2LastReplyTime 'uint32_t' value (seconds since 1970-1-1)  
			// to "2010-11-30" style before final posts to user.
			virtual void OnGetGrpFldActivityListRes(const uint32_t& groupId,
									const uint32_t& folderId,
									const std::map<uint32_t, uint32_t>& mapUid2LastReplyTime){};// = 0
			// *TODO*: UI should change the def to pure virtual method after implement it!!
		};

		class IOpGChatMsgCache
		{
		public:

			virtual void watch(IGChatMsgCacheWatcher *w) = 0;

			virtual void revoke(IGChatMsgCacheWatcher *w) = 0;
		};

		class IGChatMsgCache : public virtual IOpGChatMsgCache
		{
		public:

			virtual ~IGChatMsgCache() {}

			// get recent msgs of default chat area, deprecated since beta6
			virtual void GetGChatMsgRecent(uint32_t uGroupId, uint32_t uFolderId, const CGChatInfo& ChatInfo) = 0;
 		
			// Since yy3.0 beta6. The routine get latest chat msgs from gmsgcache.
			//  
			// e.g., pass unread msg num (calculated with popInfo) to get the latest unread msgs.
			virtual void GetLatestChatMsgList(uint32_t uGroupId, uint32_t uFolderId, uint32_t uMsgNum) = 0;

			// send chat msg
			virtual void SendChatMsg(uint32_t groupId, uint32_t folderId, uint32_t seqId,
									const protocol::gmsgcache::GTopicTextChat& text) = 0;

			//get pop info
			virtual void GetGChatPopInfo(uint32_t uGrpID, uint32_t uFldrID) = 0;

			// get activity list of group or folder
			virtual void GetGrpFldActivityList(uint32_t uGroupId, uint32_t uFolderId) = 0;

			virtual void SetOwner(uint32_t uOwnerUID) = 0;
		};		
	}
}

#endif
