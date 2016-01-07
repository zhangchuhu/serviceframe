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
		// ���⼰���ظ�ʱ��
		struct STopicLatestTime : public sox::Marshallable
		{
			uint32_t   m_topicId;      // ����ID
			uint32_t   m_groupId;      // Ⱥ/��ID
			uint32_t   m_counter;      // �·��Ե�����
			uint32_t   m_latestTime;   // ���ظ�ʱ��

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

		// �����������ظ�ʱ��

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


		// ����Ϣ��Gtopic ����Gmsgcache�����ڶ�PSS_GTopicPopInfoDBReQ ����Ӧ
		struct PSS_GTopicPopInfoDBRes : public sox::Marshallable 
		{
			enum {uri = (102 << 8) | protocol::GMSGCACHE_SVID};

			uint32_t   m_uid;       // �������û�id
			uint32_t   m_linkdId;   // ������imlinkdid
			uint32_t   m_groupId;   // ������Ⱥid
			uint32_t   m_folderId;  // ��������id
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

		// ����Ϣ��Gtransfer ����Gmsgcache���������������������
		// ��Ϣ��������Gtransfer �յ���PCS_GTopicPopInfoReq,
		// ����GTransfer Ҫ��д������id��������imlinkdid
		struct PSS_GTopicPopInfoDBReq : public sox::Marshallable
		{
			enum {uri = (103 << 8) | protocol::GMSGCACHE_SVID};
			uint32_t   m_uid;      // �������û�id
			uint32_t   m_linkdId;  // ������imlinkdid
			uint32_t   m_groupId;  // ������Ⱥid
			uint32_t   m_folderId; // ��������id
			std::vector<STopicPopInfo>  m_vecTopics; // ��������� ����

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

		
		// ����Ϣ��Gtopic ����Gmsgcache�����ڶ�PSS_GTopicSumNumDBReq ����Ӧ
		struct PSS_GTopicSumNumDBRes : public sox::Marshallable 
		{
			enum {uri = (104 << 8) | protocol::GMSGCACHE_SVID};

			uint32_t   m_uid;	   // �������û�id
			uint32_t   m_linkdId;  // ������imlinkdid
			uint32_t   m_groupId;  // ������Ⱥid
			uint32_t   m_folderId; // ��������id
			uint32_t   m_sumnum;   // ��������

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

		// ����Ϣ��Gmsgcache ����Gtopic������������������
		struct PSS_GTopicSumNumDBReq : public sox::Marshallable	
		{
			enum {uri = (105 << 8) | protocol::GMSGCACHE_SVID};

			uint32_t   m_uid;      // �������û�id
			uint32_t   m_linkdId;  // ������imlinkdid
			uint32_t   m_groupId;  // ������Ⱥid
			uint32_t   m_folderId; // ��������id

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

		// ����Ϣ��Gtopic ����Gtransfer����Gmsgcache���������������ɾ������
		struct PSS_GTopicUpdate : public sox::Marshallable
		{
			enum {uri = (106 << 8) | protocol::GMSGCACHE_SVID};

			uint32_t   m_oper;     // ���������
			uint32_t   m_groupId;  // ������Ⱥid
			uint32_t   m_folderId; // �������id
			uint32_t   m_createTime; //���ⴴ��ʱ��
			std::vector<uint32_t>   m_vecTid; // ���µ�topic

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

		// ����Ϣ��Gtopic ����Gtransfer����Gmsgcache������������֪ͨ�ö�����״̬����
		struct PSS_GTopicPinnedUpdate : public sox::Marshallable
		{
			enum {uri = (107 << 8) | protocol::GMSGCACHE_SVID};

			uint32_t   m_oper;     // ���������
			uint32_t   m_groupId;  // ������Ⱥid
			uint32_t   m_folderId; // �������id
			std::vector<uint32_t>   m_vecTid; // �漰״̬���ĵ�����

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

		// GMsgCache�� GTopic���� �ö������tids
		struct PSS_GTopicPinnedListReq: public sox::Marshallable
		{
			enum {uri = (108 << 8 | protocol::GMSGCACHE_SVID)};

			uint32_t   m_uid;      // �������û�id
			uint32_t   m_linkdId;  // ������imlinkdid
			uint32_t   m_groupId;  // ������Ⱥid
			uint32_t   m_folderId; // �������id

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

		// GTopic��GMsgCache�����ö�����tids����Ӧ
		struct PSS_GTopicPinnedListRes: public sox::Marshallable
		{
			enum {uri = (109 << 8 | protocol::GMSGCACHE_SVID)};

			uint32_t	m_uid;      // �������û�id
			uint32_t	m_linkdId;  // ������imlinkdid
			uint32_t	m_groupId;  // ������Ⱥid
			uint32_t	m_folderId; // �������id

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
	
		// Access ɾ����ָ��Gid->fid->uid��
		// 		1)����
		// 		2)Ĭ��������Ϣ
		// 		3)������Ϣ
		// ��ô���Ϣ�� m_vecTids m_vecChatMsgs��m_vecTopicMsgs���͸�GMsgCache
		//   		routeS(srvId, obj.uri, obj);	
		// ����srvId��GAuthͨ��PSS_UpdateGMemberMsgsAccessReqЯ����Access
		struct PSS_UpdateGMemberMsgsReq: public sox::Marshallable 
		{
			enum {uri = (110 << 8) | protocol::GMSGCACHE_SVID};

			uint32_t m_uSenderId;   // Accessԭ�����
			uint32_t m_uImlinkdId;  // Accessԭ����� 
			uint32_t m_uSeqId;	    // Accessԭ����� 
			uint32_t m_uUid;	    // Accessԭ�����			
			uint32_t m_uGroupId;    // Accessԭ�����	
			std::vector<uint32_t> m_vecFids;   // Accessԭ�����	

			protocol::gmsgcache::GTopicTextChat m_msgText; // ϣ�����º���Ϣ������

			//std::vector<uint32_t> m_vecTids; // ��ɾ����
			std::vector<protocol::gtopic::FIDTID> m_vecTids; // ��ɾ����

			// ��ɾĬ����������Ϣ, ����Ҫÿ��Fid��Ӧ�����30����Ϣ֮�ڵ� ��
			std::vector<protocol::gmonitor::SUpdateMsg> m_vecChatMsgs;  

			// ��ɾĬ��������Ϣ�� ����Ҫÿ��tid��Ӧ�����30����Ϣ֮�ڵ� ��
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

