#ifndef IGTOPICMSG_CACHE_H
#define IGTOPICMSG_CACHE_H

#include <vector>
#include "common/protocol/PGTopicMsg.h"
#include "common/protocol/PGMsgCacheType.h"

namespace protocol
{
	namespace gmsgcache
	{
		// 主题聊天记录
		struct CClientTopicMsg
		{
			bool       m_update;                  // true identify msg has been updated by admin
			uint32_t   m_uid;                     // 发言者			
			uint32_t   m_sendTime;                // 发言时间			
			uint64_t   m_timestamp;               // timestamp, microsecond level
			protocol::gmsgcache::GTopicTextChat m_text;      // 聊天内容

			CClientTopicMsg(){}
			CClientTopicMsg(bool update, uint32_t uid, uint32_t time, uint64_t timestamp, protocol::gmsgcache::GTopicTextChat& text)
				: m_update(update), m_uid(uid), m_sendTime(time), m_timestamp(timestamp)
			{
				m_text = text;
			}
		};

		// 最近主题聊天消息
		struct CClientMsgRecord
		{
			CRelationID                   m_Id;       // 群/组/主题 ID
			uint32_t                      m_update;     // 是否需要更新msg，1为是，0为否
			uint64_t                      m_eldesttime; // 已读最旧主体回帖时间戳
			uint64_t                      m_latesttime; // 已读最新主体回帖时间戳
			std::vector<CClientTopicMsg>  m_vecMsg;   // 记录

			CClientMsgRecord() {}
			CClientMsgRecord(CRelationID id, const uint32_t update, const uint64_t oldtime, const uint64_t newtime, const std::vector<CClientTopicMsg>& vecMsg)
			{
				m_Id = id;
				m_update = update;
				m_eldesttime = oldtime;
				m_latesttime = newtime;
				m_vecMsg = vecMsg;
			}
		};

//////////////////////////////////////////////////////////////////////////


		class IGTopicMsgCacheWatcher
		{
		public:
			// 以群id为单位，取回的主题聊天记录
			virtual void OnGTopicMsgRecentRes(const std::vector<CClientMsgRecord>& vecRecord) = 0;

			// 主题聊天消息到达
			virtual void OnGTopicMsgArrive(const uint32_t& senderUid,         // 发言者
									const uint32_t& sendTime,                 // 发言时间
									const uint64_t& timestamp,                // timestamp, microsecond level
									const uint32_t& groupId,                  // 组id
									const uint32_t& folderId,                 // 群id
									const uint32_t& topicId,                  // 主题id
									const protocol::gmsgcache::GTopicTextChat& text) = 0;

			// topic msg timeout
			virtual void OnGTopicMsgTimeout(const uint32_t& groupId,// 组id
									const uint32_t& folderId,                 // 群id
									const uint32_t& topicId,                  // 主题id
									const uint32_t& seqId,				  // 主题id
									const protocol::gmsgcache::GTopicTextChat& text) = 0;

			// 主题聊天消息响应到达
			virtual void OnTopicMsgRes(const uint32_t& groupId,               // 群id
									const uint32_t& folderId,                 // 组id
									const uint32_t& topicId,                  // 主题id
									const uint32_t& seqId,
									const uint32_t& sendtime,
									const uint64_t& timestamp) = 0;           // timestamp, microsecond level

			// 未读主题聊天消息条数到达
			//virtual void OnGTopicUnreadMsgNumRes(const std::vector<CTopicInfo>& vecTopics) = 0;

			// 最近主题消息到达
			virtual void OnGTopicRecentTopicRes(const uint32_t& groupId, const uint32_t& folderId, const uint32_t& sumnumber, const std::vector<uint32_t>& vecTopics) = 0;

			// 未读主题数目消息到达
			//virtual void OnGTopicUnreadTopicNumRes(const uint32_t& groupId, const std::vector<SUnreadTopicNum>& vecFolder) = 0; 
				
			// notify ui the result of gettting the sticky topic ids
			// gid - fid- vec< uint32_t >
			//                   tid
			virtual void onGetPinnedTopicList(const uint32_t& uGrpID, const uint32_t& uFldID, 
											  const std::vector<uint32_t>& vecTIDs)=0; 

			// notify ui the result of getting topic latest reply time and count
			virtual void onGetGTopicPopInfo(const uint32_t& uGrpID, const uint32_t& uFldrID, 
				const std::vector<protocol::gmsgcache::STopicPopInfo>& vecTopicPopInfos)=0;

		};

		class IOpGTopicMsgCache
		{
		public:

			virtual void watch(IGTopicMsgCacheWatcher *w) = 0;

			virtual void revoke(IGTopicMsgCacheWatcher *w) = 0;
		};

		class IGTopicMsgCache : public virtual IOpGTopicMsgCache
		{
		public:

			virtual ~IGTopicMsgCache() {}

			// 获取最近的主题记录
			virtual void GetGTopicMsgRecent(const std::vector<CTopicInfo>& vecReq) = 0;

			// 请求未读主题聊天消息记录条数
			//virtual void GetGTopicUnreadMsgNum(const std::vector<CTopicInfo>& vecReq) = 0;

			// 发送主题聊天消息
			virtual void SendTopicMsg(uint32_t groupId,                // 群id
									uint32_t folderId,                 // 组id
									uint32_t topicId,                  // 主题id
									uint32_t seqId,                    // 消息流水号
									const protocol::gmsgcache::GTopicTextChat& text) = 0;
									
			//send readed topic id to access									
			virtual void SendTopicRead(uint32_t groupId, uint32_t folderId, const std::vector<uint32_t>& vecTopicId) = 0;

			// 请求最热主题
			//virtual void GetGTopicRecentTopic(uint32_t groupId, uint32_t folderId) = 0;

			//请求最热主题 和 置顶主题
			virtual void GetGTopicRecentPinnedTopic(uint32_t groupId, uint32_t folderId) = 0;

			// 请求未读主题数目
			//virtual void GetGTopicUnreadTopicNum(const std::vector<SUnreadTopicNum>& vecReq) = 0;

			//请求pop info
			virtual void GetGTopicPopInfo(const uint32_t uGrpID, const uint32_t uFldrID, const std::vector<uint32_t>& vecTids) = 0;
		};		
	}
}

#endif
