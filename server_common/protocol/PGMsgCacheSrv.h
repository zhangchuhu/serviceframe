#pragma once

#include <vector>
//#include <string>
//#include "iproperty.h"
//#include "protocol/const.h"
#include "core/base_svid.h"

#include "common/packet.h"
#include "common/protocol/PGMsgCacheType.h"
#include "server_common/protocol/PStoreGMsg.h"

namespace protocol
{
	namespace gmsgcache
	{    
		// 主题及最后回复时间
		struct STopicLatestTime : public sox::Marshallable
		{
			uint32_t   m_topicId;      // 主题ID
			uint32_t   m_groupId;      // 群/组ID
			uint32_t   m_counter;      // 新发言的条数
			uint32_t   m_latestTime;   // 最后回复时间

			STopicLatestTime() {}
			STopicLatestTime(uint32_t topicId, uint32_t groupId, uint32_t counter, uint32_t latesttime)
				: m_topicId(topicId), m_groupId(groupId), m_counter(counter), m_latestTime(latesttime)
			{
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_topicId << m_groupId << m_counter << m_latestTime;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_topicId >> m_groupId >> m_counter >> m_latestTime;
			}
		};

		// 更新主题最后回复时间

		struct PSS_GTopicLatestTime : public sox::Marshallable 
		{
			enum {uri = (101 << 8) | protocol::GMSGCACHE_SVID};

			std::vector<STopicLatestTime> m_vecUpdate;

			PSS_GTopicLatestTime() {}
			PSS_GTopicLatestTime(std::vector<STopicLatestTime>& vec)
				: m_vecUpdate(vec)
			{
			}

			virtual void marshal(sox::Pack &p) const
			{
				sox::marshal_container(p, m_vecUpdate);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				sox::unmarshal_container(p, std::back_inserter(m_vecUpdate));
			}
		};


		// 该消息由Gtopic 发给Gmsgcache，用于对PSS_GTopicPopInfoDBReQ 的响应
		struct PSS_GTopicPopInfoDBRes : public sox::Marshallable 
		{
			enum {uri = (102 << 8) | protocol::GMSGCACHE_SVID};

			uint32_t   m_uid;       // 请求者用户id
			uint32_t   m_linkdId;   // 请求者imlinkdid
			uint32_t   m_groupId;   // 请求者群id
			uint32_t   m_folderId;  // 请求者组id
			std::vector<STopicPopInfo> m_vecTopics;

			PSS_GTopicPopInfoDBRes() {}
			PSS_GTopicPopInfoDBRes(uint32_t uid, uint32_t linkdId, uint32_t groupId, uint32_t folderId, 
												const std::vector<STopicPopInfo>& vec)
				: m_uid(uid), m_linkdId(linkdId), m_groupId(groupId), m_folderId(folderId)
			{
				m_vecTopics = vec;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uid << m_linkdId << m_groupId << m_folderId;
				marshal_container(p, m_vecTopics);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uid >> m_linkdId >> m_groupId >> m_folderId;
				unmarshal_containerEx(p, m_vecTopics);
			}
		
		};

		// 该消息由Gtransfer 发给Gmsgcache，用于请求主题回帖总数
		// 消息数据来自Gtransfer 收到的PCS_GTopicPopInfoReq,
		// 但是GTransfer 要填写发送者id，发送者imlinkdid
		struct PSS_GTopicPopInfoDBReq : public sox::Marshallable
		{
			enum {uri = (103 << 8) | protocol::GMSGCACHE_SVID};
			uint32_t   m_uid;      // 请求者用户id
			uint32_t   m_linkdId;  // 请求者imlinkdid
			uint32_t   m_groupId;  // 请求者群id
			uint32_t   m_folderId; // 请求者组id
			std::vector<STopicPopInfo>  m_vecTopics; // 请求的主题 集合

			PSS_GTopicPopInfoDBReq(){}
			PSS_GTopicPopInfoDBReq(uint32_t uid, uint32_t linkdId, uint32_t groupId, uint32_t folderId,
												const std::vector<STopicPopInfo>& vec)
				: m_uid(uid), m_linkdId(linkdId), m_groupId(groupId), m_folderId(folderId)
			{
				m_vecTopics = vec;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uid << m_linkdId << m_groupId << m_folderId;
				marshal_container(p, m_vecTopics);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uid >> m_linkdId >> m_groupId >> m_folderId;
				unmarshal_containerEx(p, m_vecTopics);
			}

		};

		
		// 该消息由Gtopic 发给Gmsgcache，用于对PSS_GTopicSumNumDBReq 的响应
		struct PSS_GTopicSumNumDBRes : public sox::Marshallable 
		{
			enum {uri = (104 << 8) | protocol::GMSGCACHE_SVID};

			uint32_t   m_uid;	   // 请求者用户id
			uint32_t   m_linkdId;  // 请求者imlinkdid
			uint32_t   m_groupId;  // 请求者群id
			uint32_t   m_folderId; // 请求者组id
			uint32_t   m_sumnum;   // 主题总数

			PSS_GTopicSumNumDBRes() {}
			PSS_GTopicSumNumDBRes(uint32_t uid, uint32_t linkdid, uint32_t groupId, uint32_t folderId, uint32_t num)
				: m_uid(uid), m_linkdId(linkdid), m_groupId(groupId), m_folderId(folderId), m_sumnum(num)
			{}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uid << m_linkdId << m_groupId << m_folderId << m_sumnum;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uid >> m_linkdId >> m_groupId >> m_folderId >> m_sumnum;
			}

		};

		// 该消息由Gmsgcache 发给Gtopic，用于请求主题总数
		struct PSS_GTopicSumNumDBReq : public sox::Marshallable	
		{
			enum {uri = (105 << 8) | protocol::GMSGCACHE_SVID};

			uint32_t   m_uid;      // 请求者用户id
			uint32_t   m_linkdId;  // 请求者imlinkdid
			uint32_t   m_groupId;  // 请求者群id
			uint32_t   m_folderId; // 请求者组id

			PSS_GTopicSumNumDBReq(){}
			PSS_GTopicSumNumDBReq(uint32_t uid, uint32_t linkdid, uint32_t groupId, uint32_t folderId)
				: m_uid(uid), m_linkdId(linkdid), m_groupId(groupId), m_folderId(folderId)
			{}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uid << m_linkdId << m_groupId << m_folderId;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uid >> m_linkdId >> m_groupId >> m_folderId;
			}

		};

		enum E_GTOPIC_UPDATE_OPER_FLAG
		{
			EMPTY_GROUP = 0,
			EMPTY_FOLDER = 1,
			ADD_TOPIC = 2,
			DEL_TOPIC = 3,
			MODIFY_TOPIC = 4,
			NEW_GROUP_FOLDER = 5
		};

		// 该消息由Gtopic 经由Gtransfer发给Gmsgcache服务器，用于添加删除主题
		struct PSS_GTopicUpdate : public sox::Marshallable
		{
			enum {uri = (106 << 8) | protocol::GMSGCACHE_SVID};

			uint32_t   m_oper;     // 请求操作符
			uint32_t   m_groupId;  // 请求者群id
			uint32_t   m_folderId; // 请求的组id
			uint32_t   m_createTime; //主题创建时间
			std::vector<uint32_t>   m_vecTid; // 更新的topic

			PSS_GTopicUpdate(){}
			PSS_GTopicUpdate(uint32_t oper, uint32_t groupId, uint32_t folderId, uint32_t createTime,
									 const std::vector<uint32_t>& vec)
				:  m_oper(oper), m_groupId(groupId), m_folderId(folderId), m_createTime(createTime)
			{
				m_vecTid = vec;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_oper << m_groupId << m_folderId << m_createTime;
				marshal_container(p, m_vecTid);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_oper >> m_groupId >> m_folderId >> m_createTime;
				unmarshal_container(p, std::back_inserter(m_vecTid));
			}

		};	

		enum E_GTOPIC_PINNED_UPDATE_OPER_FLAG
		{
			PIN_TOPIC = 0,
			UNPIN_TOPIC = 1
		};

		// 该消息由Gtopic 经由Gtransfer发给Gmsgcache服务器，用于通知置顶主题状态更改
		struct PSS_GTopicPinnedUpdate : public sox::Marshallable
		{
			enum {uri = (107 << 8) | protocol::GMSGCACHE_SVID};

			uint32_t   m_oper;     // 请求操作符
			uint32_t   m_groupId;  // 请求者群id
			uint32_t   m_folderId; // 请求的组id
			std::vector<uint32_t>   m_vecTid; // 涉及状态更改的主题

			PSS_GTopicPinnedUpdate(){}
			PSS_GTopicPinnedUpdate(uint32_t oper, uint32_t groupId, uint32_t folderId,
									 const std::vector<uint32_t>& vec)
				:  m_oper(oper), m_groupId(groupId), m_folderId(folderId)
			{
				m_vecTid = vec;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_oper << m_groupId << m_folderId;
				marshal_container(p, m_vecTid);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_oper >> m_groupId >> m_folderId;
				unmarshal_container(p, std::back_inserter(m_vecTid));
			}
		};	

		// GMsgCache向 GTopic请求 置顶主题的tids
		struct PSS_GTopicPinnedListReq: public sox::Marshallable
		{
			enum {uri = (108 << 8 | protocol::GMSGCACHE_SVID)};

			uint32_t   m_uid;      // 请求者用户id
			uint32_t   m_linkdId;  // 请求者imlinkdid
			uint32_t   m_groupId;  // 请求者群id
			uint32_t   m_folderId; // 请求的组id

			PSS_GTopicPinnedListReq(){}

			PSS_GTopicPinnedListReq(uint32_t uid, uint32_t linkdid, uint32_t gid, uint32_t fid)
				: m_uid(uid), m_linkdId(linkdid), m_groupId(gid), m_folderId(fid) {}
			
			virtual void marshal(sox::Pack &p) const
			{
				p << m_uid << m_linkdId << m_groupId << m_folderId;	
			}
		
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >>  m_uid >> m_linkdId >> m_groupId >> m_folderId;
			}
		};

		// GTopic向GMsgCache返回置顶主题tids的响应
		struct PSS_GTopicPinnedListRes: public sox::Marshallable
		{
			enum {uri = (109 << 8 | protocol::GMSGCACHE_SVID)};

			uint32_t	m_uid;      // 请求者用户id
			uint32_t	m_linkdId;  // 请求者imlinkdid
			uint32_t	m_groupId;  // 请求者群id
			uint32_t	m_folderId; // 请求的组id

			std::set<uint32_t> setTIDs; 

			PSS_GTopicPinnedListRes(){}

			PSS_GTopicPinnedListRes(uint32_t uid, uint32_t linkdid, uint32_t gid, uint32_t fid, std::set<uint32_t> setTopicIDs)
					: m_uid(uid), m_linkdId(linkdid), m_groupId(gid), m_folderId(fid), setTIDs(setTopicIDs){}

			//add a new ctor according to jinpei's requirement
			PSS_GTopicPinnedListRes(PSS_GTopicPinnedListReq& obj)
					: m_uid(obj.m_uid), m_linkdId(obj.m_linkdId), m_groupId(obj.m_groupId), m_folderId(obj.m_folderId){}
			
			virtual void marshal(sox::Pack &p) const
			{
				p << m_uid << m_linkdId << m_groupId << m_folderId;	
				marshal_container(p, setTIDs);
			}
		
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >>  m_uid >> m_linkdId >> m_groupId >> m_folderId;
				unmarshal_container(p, std::inserter(setTIDs,setTIDs.begin()));
			}
		};
	
		// Access 删除完指定Gid->fid->uid的
		// 		1)主题
		// 		2)默认聊天消息
		// 		3)主题消息
		// 填好此消息的 m_vecTids m_vecChatMsgs和m_vecTopicMsgs发送给GMsgCache
		//   		routeS(srvId, obj.uri, obj);	
		// 其中srvId由GAuth通过PSS_UpdateGMemberMsgsAccessReq携带给Access
		struct PSS_UpdateGMemberMsgsReq: public sox::Marshallable 
		{
			enum {uri = (110 << 8) | protocol::GMSGCACHE_SVID};

			uint32_t m_uSenderId;   // Access原样填回
			uint32_t m_uImlinkdId;  // Access原样填回 
			uint32_t m_uSeqId;	    // Access原样填回 
			uint32_t m_uUid;	    // Access原样填回			
			uint32_t m_uGroupId;    // Access原样填回	
			std::vector<uint32_t> m_vecFids;   // Access原样填回	

			protocol::gmsgcache::GTopicTextChat m_msgText; // 希望更新后消息的内容

			//std::vector<uint32_t> m_vecTids; // 待删主题
			std::vector<protocol::gtopic::FIDTID> m_vecTids; // 待删主题

			// 待删默认聊天区消息, 仅需要每个Fid对应的最近30条消息之内的 ！
			std::vector<protocol::gmonitor::SUpdateMsg> m_vecChatMsgs;  

			// 待删默认主题消息， 仅需要每个tid对应的最近30条消息之内的 ！
			std::vector<protocol::gmonitor::SUpdateMsg> m_vecTopicMsgs; 

			PSS_UpdateGMemberMsgsReq() {}

			PSS_UpdateGMemberMsgsReq(uint32_t senderId, uint32_t linkdId, uint32_t seqId, uint32_t uid, uint32_t gid,
				   const std::vector<uint32_t>& vecFids, 
				   const protocol::gmsgcache::GTopicTextChat& text,
				   const std::vector<protocol::gtopic::FIDTID>& vecTids,
				   const std::vector<protocol::gmonitor::SUpdateMsg>& vecChatMsgs,
				   const std::vector<protocol::gmonitor::SUpdateMsg>& vecTopicMsgs) 
				: m_uSenderId(senderId), m_uImlinkdId(linkdId), m_uSeqId(seqId), m_uUid(uid), m_uGroupId(gid), 
				  m_vecFids(vecFids), m_msgText(text), m_vecTids(vecTids),
				  m_vecChatMsgs(vecChatMsgs), m_vecTopicMsgs(vecTopicMsgs) {}

			PSS_UpdateGMemberMsgsReq(server::store::PSS_UpdateGMemberMsgsAccessReq& obj,
				   const std::vector<protocol::gtopic::FIDTID>& vecTids,
				   const std::vector<protocol::gmonitor::SUpdateMsg>& vecChatMsgs,
				   const std::vector<protocol::gmonitor::SUpdateMsg>& vecTopicMsgs) 
				: m_uSenderId(obj.m_uSenderId), m_uImlinkdId(obj.m_uImlinkdId), m_uSeqId(obj.m_uSeqId), m_uUid(obj.m_uUid),
				  m_uGroupId(obj.m_uGroupId), m_vecFids(obj.m_vecFids), m_msgText(obj.m_msgText),
				  m_vecTids(vecTids), m_vecChatMsgs(vecChatMsgs), m_vecTopicMsgs(vecTopicMsgs) {}
				  	
			virtual void marshal(sox::Pack &p) const
			{
				p << m_uSenderId << m_uImlinkdId << m_uSeqId << m_uUid << m_uGroupId;
				sox::marshal_container(p, m_vecFids);
				m_msgText.marshal(p);
				sox::marshal_container(p, m_vecTids);
				sox::marshal_container(p, m_vecChatMsgs);
				sox::marshal_container(p, m_vecTopicMsgs);
			}
		
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uSenderId >> m_uImlinkdId >> m_uSeqId >> m_uUid >> m_uGroupId;
				sox::unmarshal_container(p, std::back_inserter(m_vecFids));
				m_msgText.unmarshal(p);
				sox::unmarshal_container(p, std::back_inserter(m_vecTids));
				sox::unmarshal_containerEx(p, m_vecChatMsgs);
				sox::unmarshal_containerEx(p, m_vecTopicMsgs);
			}
		};
	}
}

