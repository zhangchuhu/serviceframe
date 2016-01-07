#ifndef IGTOPICMSG_CACHE_H
#define IGTOPICMSG_CACHE_H

#include <vector>
#include "common/protocol/PGTopicMsg.h"
#include "common/protocol/PGMsgCacheType.h"

namespace protocol
{
	namespace gmsgcache
	{
		// ���������¼
		struct CClientTopicMsg
		{
			bool       m_update;                  // true identify msg has been updated by admin
			uint32_t   m_uid;                     // ������			
			uint32_t   m_sendTime;                // ����ʱ��			
			uint64_t   m_timestamp;               // timestamp, microsecond level
			protocol::gmsgcache::GTopicTextChat m_text;      // ��������

			CClientTopicMsg(){}
			CClientTopicMsg(bool update, uint32_t uid, uint32_t time, uint64_t timestamp, protocol::gmsgcache::GTopicTextChat& text)
				: m_update(update), m_uid(uid), m_sendTime(time), m_timestamp(timestamp)
			{
				m_text = text;
			}
		};

		// �������������Ϣ
		struct CClientMsgRecord
		{
			CRelationID                   m_Id;       // Ⱥ/��/���� ID
			uint32_t                      m_update;     // �Ƿ���Ҫ����msg��1Ϊ�ǣ�0Ϊ��
			uint64_t                      m_eldesttime; // �Ѷ�����������ʱ���
			uint64_t                      m_latesttime; // �Ѷ������������ʱ���
			std::vector<CClientTopicMsg>  m_vecMsg;   // ��¼

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
			// ��ȺidΪ��λ��ȡ�ص����������¼
			virtual void OnGTopicMsgRecentRes(const std::vector<CClientMsgRecord>& vecRecord) = 0;

			// ����������Ϣ����
			virtual void OnGTopicMsgArrive(const uint32_t& senderUid,         // ������
									const uint32_t& sendTime,                 // ����ʱ��
									const uint64_t& timestamp,                // timestamp, microsecond level
									const uint32_t& groupId,                  // ��id
									const uint32_t& folderId,                 // Ⱥid
									const uint32_t& topicId,                  // ����id
									const protocol::gmsgcache::GTopicTextChat& text) = 0;

			// topic msg timeout
			virtual void OnGTopicMsgTimeout(const uint32_t& groupId,// ��id
									const uint32_t& folderId,                 // Ⱥid
									const uint32_t& topicId,                  // ����id
									const uint32_t& seqId,				  // ����id
									const protocol::gmsgcache::GTopicTextChat& text) = 0;

			// ����������Ϣ��Ӧ����
			virtual void OnTopicMsgRes(const uint32_t& groupId,               // Ⱥid
									const uint32_t& folderId,                 // ��id
									const uint32_t& topicId,                  // ����id
									const uint32_t& seqId,
									const uint32_t& sendtime,
									const uint64_t& timestamp) = 0;           // timestamp, microsecond level

			// δ������������Ϣ��������
			//virtual void OnGTopicUnreadMsgNumRes(const std::vector<CTopicInfo>& vecTopics) = 0;

			// ���������Ϣ����
			virtual void OnGTopicRecentTopicRes(const uint32_t& groupId, const uint32_t& folderId, const uint32_t& sumnumber, const std::vector<uint32_t>& vecTopics) = 0;

			// δ��������Ŀ��Ϣ����
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

			// ��ȡ����������¼
			virtual void GetGTopicMsgRecent(const std::vector<CTopicInfo>& vecReq) = 0;

			// ����δ������������Ϣ��¼����
			//virtual void GetGTopicUnreadMsgNum(const std::vector<CTopicInfo>& vecReq) = 0;

			// ��������������Ϣ
			virtual void SendTopicMsg(uint32_t groupId,                // Ⱥid
									uint32_t folderId,                 // ��id
									uint32_t topicId,                  // ����id
									uint32_t seqId,                    // ��Ϣ��ˮ��
									const protocol::gmsgcache::GTopicTextChat& text) = 0;
									
			//send readed topic id to access									
			virtual void SendTopicRead(uint32_t groupId, uint32_t folderId, const std::vector<uint32_t>& vecTopicId) = 0;

			// ������������
			//virtual void GetGTopicRecentTopic(uint32_t groupId, uint32_t folderId) = 0;

			//������������ �� �ö�����
			virtual void GetGTopicRecentPinnedTopic(uint32_t groupId, uint32_t folderId) = 0;

			// ����δ��������Ŀ
			//virtual void GetGTopicUnreadTopicNum(const std::vector<SUnreadTopicNum>& vecReq) = 0;

			//����pop info
			virtual void GetGTopicPopInfo(const uint32_t uGrpID, const uint32_t uFldrID, const std::vector<uint32_t>& vecTids) = 0;
		};		
	}
}

#endif
