#pragma once

#include <vector>
#include <string>
#include "iproperty.h"
#include "protocol/const.h"
#include "core/base_svid.h"
#include "common/packet.h"
#include "common/protocol/PGMsgCacheType.h"

namespace protocol
{
	namespace gmsgcache
	{
		enum E_UPDATE_MSG
		{
			NOT_UPDATE_MSG = 0,
			UPDATE_MSG = 1,
		};

		// 一个主题的最近聊天记录
		struct CServerMsgRecord : public sox::Marshallable
		{
			CRelationID                   m_Id;          // 群/组/主题 ID
			uint32_t                      m_uUpdate;     // notify ui to update msgs, 1-update,  0-do not update
			std::vector<CServerTopicMsg>  m_vecMsg;      // 最近的聊天记录按照时间升序排序

			CServerMsgRecord(){}
			CServerMsgRecord(CRelationID id)	
			{
				m_Id = id;
			}
			CServerMsgRecord(CRelationID id, uint32_t update, const std::vector<CServerTopicMsg>& vecMsg)
			{
				m_vecMsg = vecMsg;
				m_Id = id;
				m_uUpdate = update;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUpdate;
				m_Id.marshal(p);
				sox::marshal_container(p, m_vecMsg);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uUpdate;
				m_Id.unmarshal(p);
				sox::unmarshal_container(p, std::back_inserter(m_vecMsg));
			} 
		};

		struct CServerMsgRecord2 : public sox::Marshallable
		{
			CRelationID                   m_Id;          // 群/组/主题 ID
			uint32_t                      m_uUpdate;     // notify ui to update msgs, 1-update,  0-do not update
			std::vector<CServerTopicMsg2>  m_vecMsg;      // 最近的聊天记录按照时间升序排序

			CServerMsgRecord2(){}
			CServerMsgRecord2(CRelationID id)	
			{
				m_Id = id;
			}
			CServerMsgRecord2(CRelationID id, uint32_t update, const std::vector<CServerTopicMsg2>& vecMsg)
			{
				m_vecMsg = vecMsg;
				m_Id = id;
				m_uUpdate = update;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUpdate;
				m_Id.marshal(p);
				sox::marshal_container(p, m_vecMsg);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uUpdate;
				m_Id.unmarshal(p);
				sox::unmarshal_container(p, std::back_inserter(m_vecMsg));
			} 
		};

		// 一个主题的最近聊天记录
		struct CServerChatMsgRecord : public sox::Marshallable
		{
			uint32_t                      m_uUpdate;     // notify ui to update msgs, 1-update,  0-do not update
			std::vector<CServerTopicMsg>  m_vecMsg;      // 最近的聊天记录按照时间升序排序

			CServerChatMsgRecord(){}

			CServerChatMsgRecord(uint32_t update, const std::vector<CServerTopicMsg>& vecMsg)
			{
				m_vecMsg = vecMsg;
				m_uUpdate = update;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUpdate;
				sox::marshal_container(p, m_vecMsg);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uUpdate;
				sox::unmarshal_container(p, std::back_inserter(m_vecMsg));
			} 
		};

		// 返回给客户端最近的聊天记录

		struct PCS_GTopicRecentMsgRes2 : public sox::Marshallable 
		{
			enum {uri = (1 << 8) | protocol::GMSGCACHE_SVID};

			std::vector< CServerMsgRecord2 > m_vecTopicInfo;

			PCS_GTopicRecentMsgRes2() {}
			PCS_GTopicRecentMsgRes2(const std::vector<CServerMsgRecord2>& vecTopic)
			{
				m_vecTopicInfo = vecTopic;
			}

			virtual void marshal(sox::Pack &p) const
			{
				marshal_container(p, m_vecTopicInfo);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				unmarshal_containerEx(p, m_vecTopicInfo);
			}
		};


		// gtransfer 转发给 gmsgcache 客户端请求

		struct PSS_GTopicRecentMsgReq2 : public sox::Marshallable 
		{
			enum {uri = (2 << 8) | protocol::GMSGCACHE_SVID};

			uint32_t   m_groupId;                        // 群id
			std::vector<CTopicInfo2>  m_vecTopics;

			PSS_GTopicRecentMsgReq2(){}
			PSS_GTopicRecentMsgReq2(uint32_t groupId, const std::vector<CTopicInfo2>& vec)
				: m_groupId(groupId)
			{
				m_vecTopics = vec;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_groupId;
				marshal_container(p, m_vecTopics);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_groupId;
				unmarshal_containerEx(p, m_vecTopics);
			}
		};

		// 该消息由Gmsgcache 发给protocol，用于对PCS_GTopicPopInfoReq 的响应
		struct PCS_GTopicPopInfoRes : public sox::Marshallable 
		{
			enum {uri = (3 << 8) | protocol::GMSGCACHE_SVID};

			uint32_t   m_groupId;   // 请求者群id
			uint32_t   m_folderId;  // 请求者组id
			std::vector<STopicPopInfo> m_vecTopics;

			PCS_GTopicPopInfoRes() {}
			PCS_GTopicPopInfoRes(uint32_t groupId, uint32_t folderId, const std::vector<STopicPopInfo>& vec)
			{
				m_groupId = groupId;
				m_folderId = folderId;
				m_vecTopics = vec;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_groupId << m_folderId;
				marshal_container(p, m_vecTopics);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_groupId >> m_folderId;
				unmarshal_containerEx(p, m_vecTopics);
			}
		
		};

		// 该消息由Gtransfer 发给Gmsgcache，用于请求主题回帖总数
		// 消息数据来自Gtransfer 收到的PCS_GTopicPopInfoReq,
		// 但是GTransfer 要填写发送者id，发送者imlinkdid
		struct PSS_GTopicPopInfoReq : public sox::Marshallable
		{
			enum {uri = (4 << 8) | protocol::GMSGCACHE_SVID};
			uint32_t   m_groupId; // 请求者群id
			uint32_t   m_folderId; // 请求者组id
			std::vector<STopicPopInfo>  m_vecTopics; // 请求的主题 集合

			PSS_GTopicPopInfoReq(){}
			PSS_GTopicPopInfoReq(uint32_t groupId, uint32_t folderId, const std::vector<STopicPopInfo>& vec)
				: m_groupId(groupId), m_folderId(folderId)
			{
				m_vecTopics = vec;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_groupId << m_folderId;
				marshal_container(p, m_vecTopics);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_groupId >> m_folderId;
				unmarshal_containerEx(p, m_vecTopics);
			}

		};

		// 对PSS_GTopicRecentPinnedTopicReq响应的一部分
		// 该消息由Gmsgcache 发给protocol，返回最近主题
		struct PCS_GTopicRecentTopicRes : public sox::Marshallable 
		{
			enum {uri = (5 << 8) | protocol::GMSGCACHE_SVID};

			uint32_t   m_groupId;  // 请求者群id
			uint32_t   m_folderId; // 请求者组id
			uint32_t   m_topicSumNum;   // 主题总数
			std::vector<uint32_t> m_vecTid; // 最热主题，按时间顺序降序排序

			PCS_GTopicRecentTopicRes() {}
			PCS_GTopicRecentTopicRes(uint32_t groupId, uint32_t folderId, uint32_t sumnum, const std::vector<uint32_t>& vec)
			{
				m_groupId = groupId;
				m_folderId = folderId;
				m_topicSumNum = sumnum;
				m_vecTid = vec;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_groupId << m_folderId << m_topicSumNum;
				marshal_container(p, m_vecTid);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_groupId >> m_folderId >> m_topicSumNum;
				unmarshal_container(p, std::back_inserter(m_vecTid));
			}
		
		};

		//对PSS_GTopicRecentPinnedTopicReq响应的一部分
		//该消息由Gmsgcache 发给protocol，返回置顶主题
		struct PCS_GTopicPinnedListRes: public sox::Marshallable
		{
			enum {uri = (6 << 8 | protocol::GMSGCACHE_SVID)};

			uint32_t m_groupId;		
			uint32_t m_folderId;
			std::set<uint32_t> m_setTIDs;

			PCS_GTopicPinnedListRes(){}

			PCS_GTopicPinnedListRes(uint32_t gid, uint32_t fid, std::set<uint32_t> setTIDs)
				: m_groupId(gid), m_folderId(fid), m_setTIDs(setTIDs){}

			virtual void marshal(sox::Pack &p) const
			{
				p <<  m_groupId << m_folderId;	
				marshal_container(p, m_setTIDs);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >>  m_groupId >> m_folderId;
				unmarshal_container(p, std::inserter(m_setTIDs,m_setTIDs.begin()));
			}
		};

		// 该消息由Gtransfer 发给Gmsgcache，用于请求最热主题和置顶主题
		// 消息数据来自Gtransfer 收到的PCS_GTopicRecentTopicReq,
		// 但是GTransfer 要填写发送者id，发送者imlinkdid
		struct PSS_GTopicRecentPinnedTopicReq : public sox::Marshallable
		{
			enum {uri = (7 << 8) | protocol::GMSGCACHE_SVID};

			uint32_t   m_groupId;  // 请求者群id
			uint32_t   m_folderId; // 请求的组id

			PSS_GTopicRecentPinnedTopicReq(){}
			PSS_GTopicRecentPinnedTopicReq(uint32_t groupId, uint32_t folderId)
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

		struct PSS_GTopicRecentMsgReq : public sox::Marshallable 
		{
			enum {uri = (8 << 8) | protocol::GMSGCACHE_SVID};

			uint32_t   m_groupId;                        // 群id
			std::vector<CTopicInfo>  m_vecTopics;

			PSS_GTopicRecentMsgReq(){}
			PSS_GTopicRecentMsgReq(uint32_t groupId, const std::vector<CTopicInfo>& vec)
				: m_groupId(groupId)
			{
				m_vecTopics = vec;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_groupId;
				marshal_container(p, m_vecTopics);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_groupId;
				unmarshal_containerEx(p, m_vecTopics);
			}
		};

		struct PCS_GTopicRecentMsgRes : public sox::Marshallable 
		{
			enum {uri = (9 << 8) | protocol::GMSGCACHE_SVID};

			std::vector< CServerMsgRecord > m_vecTopicInfo;

			PCS_GTopicRecentMsgRes() {}
			PCS_GTopicRecentMsgRes(const std::vector<CServerMsgRecord>& vecTopic)
			{
				m_vecTopicInfo = vecTopic;
			}

			virtual void marshal(sox::Pack &p) const
			{
				marshal_container(p, m_vecTopicInfo);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				unmarshal_containerEx(p, m_vecTopicInfo);
			}
		};

		// 该消息由protocol 发给GMsgCache，用于请求群/组主题聊天记录总数
		struct PCS_GChatPopInfoReq : public sox::Marshallable 
		{
			enum {uri = (10 << 8) | protocol::GMSGCACHE_SVID};
			uint32_t	m_uGroupId;  			
			uint32_t	m_uFolderId; 

			PCS_GChatPopInfoReq(){}
			PCS_GChatPopInfoReq(uint32_t groudId, uint32_t folderId)
				: m_uGroupId(groudId), m_uFolderId(folderId)
			{
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId << m_uFolderId;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uGroupId >> m_uFolderId;
			}
		};

		// 该消息由Gmsgcache 发给protocol，用于对PCS_GTopicPopInfoReq 的响应
		struct PCS_GChatPopInfoRes : public sox::Marshallable 
		{
			enum {uri = (11 << 8) | protocol::GMSGCACHE_SVID};

			uint32_t	m_groupId;   
			uint32_t	m_folderId;  
			SChatPopInfo m_chatPopInfo;

			PCS_GChatPopInfoRes() {}
			PCS_GChatPopInfoRes(uint32_t groudId, uint32_t folderId, const SChatPopInfo& obj)
				: m_groupId(groudId), m_folderId(folderId), m_chatPopInfo(obj)
			{
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_groupId << m_folderId;
				m_chatPopInfo.marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_groupId >> m_folderId;
				m_chatPopInfo.unmarshal(p);
			}
		
		};

		struct PCS_GChatRecentMsgReq : public sox::Marshallable 
		{
			enum {uri = (12 << 8) | protocol::GMSGCACHE_SVID};

			uint32_t	m_uGroupId;
			uint32_t	m_uFolderId;
			CGChatInfo	m_chatInfo;

			PCS_GChatRecentMsgReq(){}
			PCS_GChatRecentMsgReq(uint32_t groudId, uint32_t folderId, const CGChatInfo& chatInfo)
				: m_uGroupId(groudId), m_uFolderId(folderId)
			{
				m_chatInfo = chatInfo;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId << m_uFolderId;
				m_chatInfo.marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uGroupId >> m_uFolderId;
				m_chatInfo.unmarshal(p);
			}
		};

		struct PCS_GChatRecentMsgRes : public sox::Marshallable 
		{
			enum {uri = (13 << 8) | protocol::GMSGCACHE_SVID};

			uint32_t	m_uGroupId;
			uint32_t	m_uFolderId;
			CServerChatMsgRecord m_chatInfo;

			PCS_GChatRecentMsgRes() {}
			PCS_GChatRecentMsgRes(uint32_t groupId, uint32_t folderId, const CServerChatMsgRecord& chatInfo)
				: m_uGroupId(groupId), m_uFolderId(folderId), m_chatInfo(chatInfo)
			{
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId << m_uFolderId;
				m_chatInfo.marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uGroupId >> m_uFolderId;
				m_chatInfo.unmarshal(p);
			}
		};

		// 删除某人在群或组所有主题和回复的功能 since beta5
		struct PCS_UpdateGMemberMsgsReq: public sox::Marshallable 
		{
			enum {uri = (14 << 8) | protocol::GMSGCACHE_SVID};
			
			uint32_t 	m_uSenderId;  // 由GAuth来填, 主要是考虑到鉴权可能缺乏权限要找GInfo
			uint32_t 	m_uImlinkdId; // 由GAuth来填, 主要是考虑到鉴权可能缺乏权限要找GInfo
			uint32_t	m_uSeqId;	  // 唯一标识客户端一次请求，由protocol生成				  			  
			uint32_t	m_uUid; 	  // Admin 希望操作的用户Id
			uint32_t	m_uGroupId;   // Admin和被操纵用户所在的群Id
			std::vector<uint32_t>	m_vecFids;  // Admin和被操纵用户所在的组Id
			protocol::gmsgcache::GTopicTextChat	m_msgText; // Admin希望显示的更新后的消息的样式

			PCS_UpdateGMemberMsgsReq() {}
			PCS_UpdateGMemberMsgsReq(uint32_t seqId, uint32_t uid, uint32_t groupId, const std::vector<uint32_t>& vecFids,
									  const protocol::gmsgcache::GTopicTextChat& text)
				: m_uSeqId(seqId), m_uUid(uid), m_uGroupId(groupId), m_vecFids(vecFids), m_msgText(text) 
			{
				m_uSenderId = 0;
				m_uImlinkdId = 0;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uSenderId << m_uImlinkdId << m_uSeqId  << m_uUid << m_uGroupId;
				sox::marshal_container(p, m_vecFids);
				m_msgText.marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uSenderId >> m_uImlinkdId >> m_uSeqId >> m_uUid >> m_uGroupId;
				sox::unmarshal_container(p, std::back_inserter(m_vecFids));
				m_msgText.unmarshal(p);
			}
		};

		// 删除某人在群/组所有主题和回复后，给客户端的响应
		// 目前由GTopic来调用
		struct PCS_UpdateGMemberMsgsRes : public sox::Marshallable 
		{
			enum {uri = (15 << 8) | protocol::GMSGCACHE_SVID};
	
			uint32_t    m_uRescode;	// 如果成功,返回 RES_SUCCESS
									// 否则返回 RES_ERETRY	
			uint32_t 	m_uSeqId;   // protocol之前填好的 用于标识唯一的删除请求
			uint32_t	m_uUid;
			uint32_t	m_uGroupId;
			std::vector<uint32_t>	m_vecFids;

			PCS_UpdateGMemberMsgsRes() {}
			PCS_UpdateGMemberMsgsRes(uint32_t rescode, uint32_t seqId, uint32_t uid, uint32_t groupId, 
									 const std::vector<uint32_t>& vecFids)
				: m_uRescode(rescode), m_uSeqId(seqId), m_uUid(uid), m_uGroupId(groupId), m_vecFids(vecFids) {}

			PCS_UpdateGMemberMsgsRes(uint32_t resCode, protocol::gmsgcache::PCS_UpdateGMemberMsgsReq& obj) 
				: m_uSeqId(obj.m_uSeqId), m_uUid(obj.m_uUid), m_uGroupId(obj.m_uGroupId), 
				  m_vecFids(obj.m_vecFids) {}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uRescode << m_uSeqId << m_uUid << m_uGroupId;
				sox::marshal_container(p, m_vecFids);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uRescode >> m_uSeqId >> m_uUid >> m_uGroupId;
				sox::unmarshal_container(p, std::back_inserter(m_vecFids));
			}
		};

		// Client caculate num of unread chat msg, and send req to GMsgcache
		struct PCS_GChatUnreadMsgReq : public sox::Marshallable 
		{
			enum {uri = (16 << 8) | protocol::GMSGCACHE_SVID};

			uint32_t	m_uGroupId;
			uint32_t	m_uFolderId;
			uint32_t	m_uUnreadMsgNum;

			PCS_GChatUnreadMsgReq(){}
			PCS_GChatUnreadMsgReq(uint32_t groudId, uint32_t folderId, uint32_t msgNum)
				: m_uGroupId(groudId), m_uFolderId(folderId), m_uUnreadMsgNum(msgNum)
			{}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId << m_uFolderId << m_uUnreadMsgNum;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uGroupId >> m_uFolderId >> m_uUnreadMsgNum;
			}
		};

		struct PCS_GChatUnreadMsgRes : public sox::Marshallable 
		{
			enum {uri = (17 << 8) | protocol::GMSGCACHE_SVID};

			uint32_t	m_uGroupId;
			uint32_t	m_uFolderId;
			std::vector<CServerTopicMsg>		m_vecMsg;  // 未读聊天记录 按照时间升序排序

			PCS_GChatUnreadMsgRes() {}
			PCS_GChatUnreadMsgRes(uint32_t groupId, uint32_t folderId, const std::vector<CServerTopicMsg>& vecMsg)
				: m_uGroupId(groupId), m_uFolderId(folderId), m_vecMsg(vecMsg)
			{}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId << m_uFolderId;
				sox::marshal_container(p, m_vecMsg);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uGroupId >> m_uFolderId;		
				sox::unmarshal_container(p, std::back_inserter(m_vecMsg));
			}
		};
	}
}

