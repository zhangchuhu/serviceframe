#ifndef PSTORE_GMSG_H
#define PSTORE_GMSG_H

#include <string>
#include "iproperty.h"
#include "protocol/const.h"
#include "core/base_svid.h"
#include "common/packet.h"
#include "common/protocol/PGMsgCache.h"
#include "common/protocol/PGMsgCacheType.h"
#include "common/protocol/pgtopicbase.h"

#include "server_common/protocol/PGMonitor.h"

//#include "common/protocol/ISearchRecord.h"

using namespace protocol::gmsgcache;
//using namespace protocol::search;

namespace server
{
    namespace store
    {   
        // ����Ϣ��Gtransfer ����Access �����������ڴ洢�������
        // ��Ϣ��������Gtransfer �յ���PCS_GTopicMsg,
        // ����GTransfer Ҫ��дʱ�䣬�����ߺͻ������
        struct PSS_GTopicMsg : public sox::Marshallable
        {
            enum {uri = (0 << 8) | protocol::ACCESS_SVID};
            
            uint32_t     m_topicId;      // ����ID
            uint32_t     m_groupId;      // Ⱥid
            uint32_t     m_folderId;     // ��id

			uint32_t     m_senderUid;    // send uid
			uint32_t     m_sendTime;     // send time
			uint64_t     m_timestamp;    // timestamp microsecond level
			protocol::gmsgcache::GTopicTextChat  m_msgText;
            
            PSS_GTopicMsg(){}
            PSS_GTopicMsg(uint32_t topicId, uint32_t groupId, uint32_t folderId, uint32_t senderUid, 
            						uint32_t sendtime, uint64_t time, const protocol::gmsgcache::GTopicTextChat& text)
                                  : m_topicId(topicId), m_groupId(groupId), m_folderId(folderId)
            {
				m_senderUid = senderUid;
				m_sendTime = sendtime;
				m_timestamp = time;
                m_msgText = text;
            }
            
            virtual void marshal(sox::Pack &p) const
            {
                p << m_topicId << m_groupId << m_folderId << m_senderUid << m_sendTime << m_timestamp;
				m_msgText.marshal(p);
            }
            
            virtual void unmarshal(const sox::Unpack &p) 
            {
                p >> m_topicId >> m_groupId >> m_folderId >> m_senderUid >> m_sendTime >> m_timestamp;
				m_msgText.unmarshal(p);
            }

        };
		
//----------------------------------------------------------
//	For GmsgCache inter-communicate with Access

		// ��ȡ���ȵ���Ϣ��, ��GmsgCache��Access����
		// �ο� psearch.h��PSS_GTopicMsgListReq /Res
		// ��֮ͬ������GmsgCacheҪ��д������id��������imlinkdid
		// Accessԭ�����
		// �������� guidȫ���ĳ��� uint64_t��
		struct PSS_GTopicMsgListAccessReq: public sox::Marshallable 
		{
			enum {uri = (50 << 8) | protocol::ACCESS_SVID};

			uint32_t m_version;

			uint32_t m_uid;      // �������û�id
			uint32_t m_linkdId;  // ������imlinkdid

			uint32_t uTopicId; // ����ID
			uint32_t uFolderId; // ��id
			uint32_t uGroupId; // Ⱥid
			//std::string uuidMsgStart; //�����Ϣ���
			//std::string uuidMsgStop; //�յ���Ϣ���
			uint32_t uMsgCount; //��Ҫ����Ϣ������Ĭ����20

			uint64_t uTimestampStart; //�����Ϣ���
			uint64_t uTimestampStop; //�յ���Ϣ���

			/*ע�⣺
			 0.�α귽ʽ��ȡ��Ϣ����uTimestampStart��Ϣ��ǰȡuMsgCount��,��������uTimestampStopΪֹ
			 1.Ӧ�ñ�֤uTimestampStart��uTimestampStop���ø��£���ʱ�������
			 2.���uTimestampStartΪ0�����������Ϣ��ʼȡ
			 3.uMsgCount��uTimestampStopΪ0ʱ�������壬��uTimestampStop��0ʱ������uMsgCount
			 4.���ؽ������uTimestampStart��Ϣ����������uTimestampStop��Ϣ
			*/
			PSS_GTopicMsgListAccessReq() {}
			
			PSS_GTopicMsgListAccessReq(uint32_t version, uint32_t uid, uint32_t linkdId, uint32_t tid,
				uint32_t fid, uint32_t gid, uint32_t msgCnt, uint64_t tsStart, uint64_t tsStop)
				: m_version(version), m_uid(uid), m_linkdId(linkdId), uTopicId(tid), uFolderId(fid),
				  uGroupId(gid), uMsgCount(msgCnt), uTimestampStart(tsStart), uTimestampStop(tsStop)
			{
			}

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_version << m_uid << m_linkdId << uTopicId << uFolderId << uGroupId 
						<< uMsgCount << uTimestampStart << uTimestampStop;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_version >> m_uid >> m_linkdId >> uTopicId >> uFolderId >> uGroupId
						>> uMsgCount >> uTimestampStart >> uTimestampStop;
			}
		};

		// ����Ϣ��Access����Gmsgcache���Ƕ�PSS_GTopicMsgListAccessReq����Ӧ
		struct PSS_GTopicMsgListAccessRes: public sox::Marshallable 
		{
			enum {uri = (50 << 8) | protocol::ACCESS_SVID};

			uint32_t m_version;

			uint32_t m_uid;      // �������û�id
			uint32_t m_linkdId;  // ������imlinkdid

			uint32_t uLeftTopicMsgCount; //����Ϣ֮ǰ��ʣ����Ϣ����
			//std::string uuidMsgNext; //��һ����Ϣ�ı�ţ����Ϊ�����ʾ��ǰû����Ϣ��
			uint64_t uTimestampNext; //��һ����Ϣ�ı�ţ����Ϊ�����ʾ��ǰû����Ϣ��
			uint32_t uTopicId; // ����ID
			uint32_t uFolderId; // ��id
			uint32_t uGroupId; // Ⱥid

			//std::vector<STopicMsgInfo> vecTopicMsgList; //���շ���ʱ�䵹������
			//CServerTopicMsg������PGMsgCacheType.h��
			std::vector<CServerTopicMsg> vecTopicMsgList; //���շ���ʱ�䵹������ 

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_version << m_uid << m_linkdId << uLeftTopicMsgCount << uTimestampNext 
						<< uTopicId << uFolderId << uGroupId;
				marshal_container(p, vecTopicMsgList);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_version >> m_uid >> m_linkdId >> uLeftTopicMsgCount >> uTimestampNext
						>> uTopicId >> uFolderId >> uGroupId;
				unmarshal_containerEx(p, vecTopicMsgList);
			}
		};

		// ��GmsgCache֪ͨAccess �������Ϣ�Ѷ���ÿ��topic��Ӧһ����Ϣ����
		// Access�����Ϣ�����Ƿ�ͱ���һ��
		// ��������һ��֪ͨ��ͨ��������Ӧ
		// �������Ϣ�����ͱ��ز�һ�µ�ʱ��Ҫ������ȷ��pop info ���� PSS_GTopicPopInfoAccessRes
		// ����PSS_GTopicPopInfoAccessRes��ʱ����ԭ�������ʱ����ȥ��m_uOriUri
		struct PSS_GTopicReadMsgNotifyReq : public sox::Marshallable 
		{
			enum {uri = (51 << 8) | protocol::ACCESS_SVID};

			uint32_t m_uOriUri;   // ԭʼ����uri
			uint32_t m_uid;		// ������uid
			uint32_t m_groupId; // ������Ⱥid
			uint32_t m_folderId; // ��������id

			//uint32_t  m_uTopicId;      
			//uint32_t  m_uSumMsgNum;    
			//uint32_t  m_uUpdateTime;   
			//���gmsgcache����cache miss������������Ϊ0����Access
			std::vector<STopicPopInfo>  m_vecTopics;

			PSS_GTopicReadMsgNotifyReq(){}
			PSS_GTopicReadMsgNotifyReq(uint32_t uri, uint32_t uid, uint32_t groupId,
				 uint32_t folderId, const std::vector<STopicPopInfo>& vec)
				:m_uOriUri(uri), m_uid(uid), m_groupId(groupId), m_folderId(folderId)
			{
				m_vecTopics = vec;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uOriUri << m_uid << m_groupId << m_folderId;
				marshal_container(p, m_vecTopics);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uOriUri >> m_uid >> m_groupId >> m_folderId;
				unmarshal_containerEx(p, m_vecTopics);
			}

		};

		// ����Ϣ��GmsgCache����Access����������PopInfo
		// ��Ϣ��������GmsgCache�յ���PSS_GTopicPopInfoReq,
		// Access ��Ҫ��дSTopicPopInfo�е���Ϣ���������ظ�ʱ��
		// Accessԭ����ط�����id��������imlinkdid���Լ�m_uOriUri
		struct PSS_GTopicPopInfoAccessReq : public sox::Marshallable
		{
			enum {uri = (52 << 8) | protocol::ACCESS_SVID};

			uint32_t   m_uOriUri;   // ԭʼ����uri

			uint32_t   m_uid;      // �������û�id
			uint32_t   m_linkdId;  // ������imlinkdid
								
			uint32_t   m_groupId;  // ������Ⱥid
			uint32_t   m_folderId; // ��������id
			std::vector<STopicPopInfo>  m_vecTopics; // ��������� ����

			PSS_GTopicPopInfoAccessReq(){}
			PSS_GTopicPopInfoAccessReq(uint32_t uri, uint32_t uid, uint32_t linkdId, uint32_t groupId,
					uint32_t folderId, const std::vector<STopicPopInfo>& vec)
				: m_uOriUri(uri), m_uid(uid), m_linkdId(linkdId), m_groupId(groupId), m_folderId(folderId)
			{
				m_vecTopics = vec;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uOriUri << m_uid << m_linkdId << m_groupId << m_folderId;
				marshal_container(p, m_vecTopics);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uOriUri >> m_uid >> m_linkdId >> m_groupId >> m_folderId;
				unmarshal_containerEx(p, m_vecTopics);
			}

		};

		// ����Ϣ��Access ����Gmsgcache�����ڶ�PSS_GTopicPopInfoAccessReq ����Ӧ
		struct PSS_GTopicPopInfoAccessRes : public sox::Marshallable 
		{
			enum {uri = (52 << 8) | protocol::ACCESS_SVID};

			//gmscache�����������Ҫ�� PopInfo
			//   1��PSS_GTopicPopInfoAccessReq
			//   2��PSS_GTopicReadedMsgNotifyReq Access�������������Ϣ�����ͱ������ݿⲻ����
			//       �᷵��pop info
			// ��˱���������, gmsgcache�����־������յ�������Ӧ����δ���
			uint32_t   m_uOriUri;   
									
			uint32_t   m_uid;       // �������û�id
			uint32_t   m_linkdId;   // ������imlinkdid
			uint32_t   m_groupId;   // ������Ⱥid
			uint32_t   m_folderId;  // ��������id
			std::vector<STopicPopInfo> m_vecTopics;

			PSS_GTopicPopInfoAccessRes() {}
			PSS_GTopicPopInfoAccessRes(uint32_t uid, uint32_t linkdId, uint32_t groupId, uint32_t folderId, 
												const std::vector<STopicPopInfo>& vec)
				: m_uid(uid), m_linkdId(linkdId), m_groupId(groupId), m_folderId(folderId)
			{
				m_vecTopics = vec;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uOriUri << m_uid << m_linkdId << m_groupId << m_folderId;
				marshal_container(p, m_vecTopics);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uOriUri >> m_uid >> m_linkdId >> m_groupId >> m_folderId;
				unmarshal_containerEx(p, m_vecTopics);
			}	
		};	

		// �����б�����, ��GmsgCache��Access����
		// �ο�����psearch.h��PSS_GTopicListReq/Res
		// ��֮ͬ������GmsgCacheҪ��д������id��������imlinkdid
		// Accessԭ�����
		// ������Ӧ��ʱ��Access����һ��ص���������
		struct PSS_GTopicListAccessReq: public sox::Marshallable 
		{
			enum {uri = (53 << 8) | protocol::ACCESS_SVID};

			uint32_t m_uOriUri;  //ԭʼ����uri

			uint32_t m_uid;       // �������û�id
			uint32_t m_linkdId;   // ������imlinkdid

			uint32_t uFolderId; // ��id
			uint32_t uGroupId; // Ⱥid
			uint32_t uStartTime; //��ʼʱ��
			uint32_t uStopTime; //����ʱ��
			uint32_t uTopicCount; //ÿҳ������Ĭ����20

			/*ע�⣺
			 0.�α귽ʽ��ȡ����ID�б���uStartTime��Ϣ��ǰȡuTopicCount��,��������uStopTimeΪֹ
			 1.Ӧ�ñ�֤uStartTime��uStopTime���ø��£���ʱ�������
			 2.���uStartTimeΪ�գ�����������⿪ʼȡ
			 3.uTopicCount��uStopTimeΪ0ʱ�������壬��uStopTime��0ʱ������uTopicCount
			 4.���ؽ������uStartTimeʱ��㣬��������uStopTimeʱ���
			*/
			PSS_GTopicListAccessReq(){}
			PSS_GTopicListAccessReq(uint32_t uri, uint32_t uid, uint32_t linkdId, uint32_t folderId, uint32_t groupId, 
									uint32_t startTime, uint32_t stopTime, uint32_t topicCnt)
				: m_uOriUri(uri), m_uid(uid), m_linkdId(linkdId), uFolderId(folderId), uGroupId(groupId), 
				  uStartTime(startTime), uStopTime(stopTime), uTopicCount(topicCnt)		  
			{
			}

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uOriUri << m_uid << m_linkdId << uFolderId << uGroupId << uStartTime 
						<< uStopTime << uTopicCount;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uOriUri >> m_uid >> m_linkdId >> uFolderId >> uGroupId >> uStartTime
						>> uStopTime >> uTopicCount;
			}
		};

		//����Ϣ��Access ����Gmsgcache�����ڶ�PSS_GTopicListAccessReq����Ӧ
		struct PSS_GTopicListAccessRes: public sox::Marshallable 
		{
			enum {uri = (53 << 8) | protocol::ACCESS_SVID};

			uint32_t m_uOriUri;  //ԭʼ����uri

			uint32_t m_uid;       // �������û�id
			uint32_t m_linkdId;   // ������imlinkdid

			uint32_t uFolderId; // ��id
			uint32_t uGroupId; // Ⱥid
			uint32_t uTopicSum; // ���ص���������
			std::vector<uint32_t> vecTopicIDList; //��������������ʱ�䵹������

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uOriUri << m_uid << m_linkdId << uFolderId << uGroupId << uTopicSum;
				marshal_container(p, vecTopicIDList);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uOriUri >> m_uid >> m_linkdId >> uFolderId >> uGroupId >> uTopicSum;
				unmarshal_container(p, std::back_inserter(vecTopicIDList));		
			}
		};
//	End GmsgCache inter-communicate with Access ��
//----------------------------------------------------------
/*		//update topic message to access
		struct PSS_UpdateMessageAccess : public sox::Marshallable
		{
			enum {uri = (54 << 8 | protocol::ACCESS_SVID)};
		
			uint32_t m_uSenderID;
			uint32_t m_uGrpID;
			uint32_t m_uFldID;
			uint32_t m_uTopicID;
			std::map<uint64_t, protocol::gmsgcache::GTopicTextChat> m_mapMessage;
		
			typedef std::map<uint64_t, protocol::gmsgcache::GTopicTextChat> MAP_MESSAGE;
			typedef const std::map<uint64_t, protocol::gmsgcache::GTopicTextChat> CONST_MAP_MESSAGE;
		
			PSS_UpdateMessageAccess() {};
			PSS_UpdateMessageAccess(uint32_t uSenderID, uint32_t uGrpID, uint32_t uFldID, uint32_t uTopicID, CONST_MAP_MESSAGE &mapMessage) : 
					m_uSenderID(uSenderID), m_uGrpID(uGrpID), m_uFldID(uFldID), m_uTopicID(uTopicID), m_mapMessage(mapMessage) {};
		
			virtual void marshal(sox::Pack &p) const
			{
				p << m_uSenderID << m_uGrpID << m_uFldID << m_uTopicID;
		
				uint32_t uMsgSize = m_mapMessage.size();
				p.push_uint32(uMsgSize);
		
				for (MAP_MESSAGE::const_iterator iterMessage = m_mapMessage.begin();
					 iterMessage != m_mapMessage.end(); iterMessage ++)
				{
					p.push_uint64(iterMessage->first);
					iterMessage->second.marshal(p);
					//p.push_varstr(iterMessage->second);
				}
			}
		
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uSenderID >> m_uGrpID >> m_uFldID >> m_uTopicID;
				
				uint32_t uMsgSize = p.pop_uint32();
				for (uint32_t i = 0; i < uMsgSize; i ++)
				{
					//std::string strGuid = p.pop_varstr();
					//std::string strMessage = p.pop_varstr();
		
					uint64_t uMessageID = p.pop_uint64();
		
					protocol::gmsgcache::GTopicTextChat TopicMessage;
					TopicMessage.unmarshal(p);
		
					m_mapMessage.insert( std::make_pair(uMessageID, TopicMessage) );
				}
			}
		};
*/

		// ����Ϣ��GmsgCache����Access����������Ĭ����������PopInfo
		// ��Ϣ��������GmsgCache�յ��� PCS_GChatPopInfoReq,
		// Access ��Ҫ������Ϣ���������ظ�ʱ��
		// ��ԭ����ط�����id��������imlinkdid����fetchCount
		struct PSS_GChatPopInfoAccessReq : public sox::Marshallable
		{
			enum {uri = (55 << 8) | protocol::ACCESS_SVID};

			uint32_t	m_uid;      	// �������û�id
			uint32_t	m_linkdId;  	// ������imlinkdid							
			uint32_t	m_groupId;  	// ������Ⱥid
			uint32_t	m_folderId; 	// ��������id
										
			uint32_t 	m_fetchCount;	// since yy3.3,��Access��ȡ�Ĵ���, ��һ��Ϊ1, �ڶ���Ϊ2, ����

			PSS_GChatPopInfoAccessReq(){}
			PSS_GChatPopInfoAccessReq(uint32_t uid, uint32_t linkdId, uint32_t groupId, uint32_t folderId, uint32_t fetchCount)
				: m_uid(uid), m_linkdId(linkdId), m_groupId(groupId), m_folderId(folderId), m_fetchCount(fetchCount)
			{
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uid << m_linkdId << m_groupId << m_folderId << m_fetchCount;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uid >> m_linkdId >> m_groupId >> m_folderId;

				if (p.size() != 0){
					try{
						p >> m_fetchCount;
					}catch(...){
						m_fetchCount = 0;
					}
				}else{
					m_fetchCount = 0;
				}
			}
		};

		// ����Ϣ��Access ����Gmsgcache�����ڶ�PSS_GChatPopInfoAccessReq ����Ӧ
		// ��yy3.3��ʼ�� 63 << 8 ȡ��
		struct PSS_GChatPopInfoAccessRes2 : public sox::Marshallable 
		{
			enum {uri = (55 << 8) | protocol::ACCESS_SVID};
											
			uint32_t	m_uid;       	// �������û�id
			uint32_t	m_linkdId;   	// ������imlinkdid
			uint32_t	m_groupId;   	// ����Chat��Ⱥid
			uint32_t	m_folderId;  	// ��������id
			SChatPopInfo m_chatPopInfo;

			PSS_GChatPopInfoAccessRes2() {}
			PSS_GChatPopInfoAccessRes2(uint32_t uid, uint32_t linkdId, uint32_t groupId, uint32_t folderId, 
									   const SChatPopInfo& chatPopInfo)
				: m_uid(uid), m_linkdId(linkdId), m_groupId(groupId), m_folderId(folderId), m_chatPopInfo(chatPopInfo)
			{}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uid << m_linkdId << m_groupId << m_folderId;
				m_chatPopInfo.marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uid >> m_linkdId >> m_groupId >> m_folderId;
				m_chatPopInfo.unmarshal(p);
			}	
		};	

		// deprecated since yy3.0 beta6
		// ��ȡ Ĭ�������� ���ȵ���Ϣ��, ��GmsgCache��Access����
		// GmsgCacheҪ��д������id��������imlinkdid
		// Accessԭ�����
		struct PSS_GChatMsgListAccessReq: public sox::Marshallable 
		{
			enum {uri = (56 << 8) | protocol::ACCESS_SVID};

			uint32_t m_uid;      // �������û�id
			uint32_t m_linkdId;  // ������imlinkdid

			uint32_t uFolderId; // ��id
			uint32_t uGroupId; // Ⱥid
	
			uint32_t uMsgCount; //��Ҫ����Ϣ������Ĭ����20

			uint64_t uTimestampStart; //�����Ϣ���
			uint64_t uTimestampStop; //�յ���Ϣ���

			/*ע�⣺
			 0.�α귽ʽ��ȡ��Ϣ����uTimestampStart��Ϣ��ǰȡuMsgCount��,��������uTimestampStopΪֹ
			 1.Ӧ�ñ�֤uTimestampStart��uTimestampStop���ø��£���ʱ�������
			 2.���uTimestampStartΪ0�����������Ϣ��ʼȡ
			 3.uMsgCount��uTimestampStopΪ0ʱ�������壬��uTimestampStop�ǿ�ʱ������uMsgCount
			 4.���ؽ������uTimestampStart��Ϣ����������uTimestampStop��Ϣ
			*/
			PSS_GChatMsgListAccessReq() {}
			PSS_GChatMsgListAccessReq(uint32_t uid, uint32_t linkdId, uint32_t folderId, uint32_t groupId, 
									  uint32_t msgCount, uint32_t timestampStart, uint32_t timestampStop)
				: m_uid(uid), m_linkdId(linkdId), uFolderId(folderId), uGroupId(groupId), 
					uMsgCount(msgCount), uTimestampStart(timestampStart), uTimestampStop(timestampStop) {}

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uid << m_linkdId << uFolderId << uGroupId 
						<< uMsgCount << uTimestampStart << uTimestampStop;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uid >> m_linkdId >> uFolderId >> uGroupId
						>> uMsgCount >> uTimestampStart >> uTimestampStop;
			}
		};

		// deprecated since yy3.0 beta6
		// ����Ϣ��Access����Gmsgcache���Ƕ�PSS_GTopicMsgListAccessReq����Ӧ
		struct PSS_GChatMsgListAccessRes: public sox::Marshallable 
		{
			enum {uri = (56 << 8) | protocol::ACCESS_SVID};

			uint32_t m_uid;      // �������û�id
			uint32_t m_linkdId;  // ������imlinkdid

			uint32_t uLeftTopicMsgCount; //����Ϣ֮ǰ��ʣ����Ϣ����
			uint64_t uTimestampNext; //��һ����Ϣ�ı�ţ����Ϊ0���ʾ��ǰû����Ϣ��

			uint32_t uFolderId; // ��id
			uint32_t uGroupId; // Ⱥid

			//std::vector<STopicMsgInfo> vecTopicMsgList; //���շ���ʱ�䵹������
			//CServerTopicMsg������PGMsgCacheType.h��
			std::vector<CServerTopicMsg> vecChatMsgList; //���շ���ʱ�䵹������ 

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uid << m_linkdId << uLeftTopicMsgCount << uTimestampNext 
						<< uFolderId << uGroupId;
				marshal_container(p, vecChatMsgList);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uid >> m_linkdId >> uLeftTopicMsgCount >> uTimestampNext
						>> uFolderId >> uGroupId;
				unmarshal_containerEx(p, vecChatMsgList);
			}
		};

		struct PSS_UpdateTopicMsgAccess : public sox::Marshallable
		{
			enum {uri = (57 << 8 | protocol::ACCESS_SVID)};
		
			uint32_t m_uGrpID;
			protocol::gmsgcache::GTopicTextChat m_msgText; 
			std::vector<protocol::gmonitor::SUpdateMsg> m_vecTopicMsgs;
		
			PSS_UpdateTopicMsgAccess() {};
			PSS_UpdateTopicMsgAccess(uint32_t uGrpID, 
												protocol::gmsgcache::GTopicTextChat& text,
												const std::vector<protocol::gmonitor::SUpdateMsg>& vec) : 
					m_uGrpID(uGrpID), m_vecTopicMsgs(vec) 
			{
				m_msgText = text;
			};
		
			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGrpID;
				m_msgText.marshal(p);
				sox::marshal_container(p, m_vecTopicMsgs);
			}
		
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGrpID;
				m_msgText.unmarshal(p);
				sox::unmarshal_containerEx(p, m_vecTopicMsgs);
			}
		};
		
		//�޸�Ĭ������������
		struct PSS_UpdateChatMsgAccess : public sox::Marshallable
		{
			enum {uri = (58 << 8 | protocol::ACCESS_SVID)};
		
			uint32_t m_uGrpID;
			protocol::gmsgcache::GTopicTextChat m_msgText; 
			std::vector<protocol::gmonitor::SUpdateMsg> m_vecChatMsgs;
		
			PSS_UpdateChatMsgAccess() {};
			PSS_UpdateChatMsgAccess(uint32_t uGrpID, 
												protocol::gmsgcache::GTopicTextChat& text,
												const std::vector<protocol::gmonitor::SUpdateMsg>& vec) : 
					m_uGrpID(uGrpID), m_vecChatMsgs(vec)
			{
				m_msgText = text;
			};
		
			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGrpID;
				m_msgText.marshal(p);
				sox::marshal_container(p, m_vecChatMsgs);
			}
		
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGrpID;
				m_msgText.unmarshal(p);
				sox::unmarshal_containerEx(p, m_vecChatMsgs);
			}
		};

		// GAuth �� AccessҪĳ����Ա��������Ϣ(������Ϣ��Ĭ����������Ϣ)����Update(���ɾ��)����
		struct PSS_UpdateGMemberMsgsAccessReq: public sox::Marshallable 
		{
			enum {uri = (59 << 8) | protocol::ACCESS_SVID};

			uint32_t m_uSenderId;   // GAuth��ã� Accessԭ�����
			uint32_t m_uImlinkdId;  // GAuth��ã� Accessԭ�����
			uint32_t m_uSeqId;		// protocol��õģ� Accessԭ�����
			uint32_t m_uSrvId;		// GAuth��ã� Access���ھ�����һ��(������GMsgCache)
			uint32_t m_uUid;	    // ������û�Id
			uint32_t m_uGroupId; 
			std::vector<uint32_t> m_vecFids; 

			protocol::gmsgcache::GTopicTextChat m_msgText; //�ͻ�����õ�, Accessԭ�����

			PSS_UpdateGMemberMsgsAccessReq() {}
			PSS_UpdateGMemberMsgsAccessReq(uint32_t uSenderId, uint32_t linkdId, uint32_t seqId, uint32_t srvId, uint32_t uid, 
				  uint32_t groupId, const std::vector<uint32_t> vecFids, const protocol::gmsgcache::GTopicTextChat& text)
				: m_uSenderId(uSenderId), m_uImlinkdId(linkdId), m_uSeqId(seqId), m_uSrvId(srvId), m_uUid(uid), 
				  m_uGroupId(groupId), m_vecFids(vecFids), m_msgText(text) {}

			PSS_UpdateGMemberMsgsAccessReq(uint32_t srvId, protocol::gmsgcache::PCS_UpdateGMemberMsgsReq& obj) 
				: m_uSenderId(obj.m_uSenderId), m_uImlinkdId(obj.m_uImlinkdId), m_uSeqId(obj.m_uSeqId), m_uSrvId(srvId), 
				  m_uUid(obj.m_uUid), m_uGroupId(obj.m_uGroupId), m_vecFids(obj.m_vecFids), m_msgText(obj.m_msgText) {}

			PSS_UpdateGMemberMsgsAccessReq(uint32_t srvId, const protocol::gmsgcache::GTopicTextChat& text, 
										   protocol::gmsgcache::PCS_UpdateGMemberMsgsReq& obj) 
				: m_uSenderId(obj.m_uSenderId), m_uImlinkdId(obj.m_uImlinkdId), m_uSeqId(obj.m_uSeqId), m_uSrvId(srvId), 
				  m_uUid(obj.m_uUid), m_uGroupId(obj.m_uGroupId), m_vecFids(obj.m_vecFids), m_msgText(text) {}

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uSenderId << m_uImlinkdId << m_uSeqId << m_uSrvId << m_uUid << m_uGroupId;
				sox::marshal_container(p, m_vecFids);
				m_msgText.marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uSenderId >> m_uImlinkdId >> m_uSeqId >> m_uSrvId >> m_uUid >> m_uGroupId;
				sox::unmarshal_container(p, std::back_inserter(m_vecFids));
				m_msgText.unmarshal(p);
			}
		};

        // ����Ϣ��Gtransfer ����Access �����������ڴ洢Ĭ������������
        // ��Ϣ��������Gtransfer �յ���PCS_GChatMsg,
        // ����GTransfer Ҫ��дʱ�䣬�����ߺͻ������
        struct PSS_GChatMsg : public sox::Marshallable
        {
            enum {uri = (60 << 8) | protocol::ACCESS_SVID};
            
            uint32_t     m_groupId;      // Ⱥid
            uint32_t     m_folderId;     // ��id

			uint32_t     m_senderUid;    // send uid
			uint32_t     m_sendTime;     // send time
			uint64_t     m_timestamp;    // timestamp microsecond level
			protocol::gmsgcache::GTopicTextChat  m_msgText;
            
            PSS_GChatMsg(){}
            PSS_GChatMsg(uint32_t groupId, uint32_t folderId, uint32_t senderUid, 
            						uint32_t sendtime, uint64_t time, const protocol::gmsgcache::GTopicTextChat& text)
                                  : m_groupId(groupId), m_folderId(folderId)
            {
				m_senderUid = senderUid;
				m_sendTime = sendtime;
				m_timestamp = time;
                m_msgText = text;
            }
            
            virtual void marshal(sox::Pack &p) const
            {
                p << m_groupId << m_folderId << m_senderUid << m_sendTime << m_timestamp;
				m_msgText.marshal(p);
            }
            
            virtual void unmarshal(const sox::Unpack &p) 
            {
                p >> m_groupId >> m_folderId >> m_senderUid >> m_sendTime >> m_timestamp;
				m_msgText.unmarshal(p);
            }

        };

		// Since yy 3.0 beta6 
		// ��ȡȺ�������Ϣ��, ��GmsgCache��Access����
		struct PSS_GChatMsgGetAccessReq: public sox::Marshallable 
		{
			enum {uri = (61 << 8) | protocol::ACCESS_SVID};

			// ���������ֶ���GMsgcache��д
			uint32_t m_uid;      		// �������û�id
			uint32_t m_linkdId;  		// ������imlinkdid
			uint32_t m_uUnreadMsgNum; 	// �û������gmsgcache�����δ����Ϣ��

			uint32_t uFolderId;			// ��id
			uint32_t uGroupId; 			// Ⱥid
	
			uint32_t uMsgCount; 		//��Ҫ����Ϣ����

			PSS_GChatMsgGetAccessReq() {}
			PSS_GChatMsgGetAccessReq(uint32_t uid, uint32_t linkdId, uint32_t unreadMsgNum, 
								uint32_t folderId, uint32_t groupId, uint32_t msgCount)
				: m_uid(uid), m_linkdId(linkdId), m_uUnreadMsgNum(unreadMsgNum), 
				  uFolderId(folderId), uGroupId(groupId), uMsgCount(msgCount) {}

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uid << m_linkdId << m_uUnreadMsgNum << uFolderId << uGroupId << uMsgCount;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uid >> m_linkdId >> m_uUnreadMsgNum >> uFolderId >> uGroupId >> uMsgCount;
			}
		};

		// ����Ϣ��Access����Gmsgcache���Ƕ� PSS_GrpRecentMsgListReq ����Ӧ
		struct PSS_GChatMsgGetAccessRes: public sox::Marshallable 
		{
			enum {uri = (62 << 8) | protocol::ACCESS_SVID};

			// ���������ֶ���GMsgcache��д, Accessԭ�����
			uint32_t m_uid;      // �������û�id
			uint32_t m_linkdId;  // ������imlinkdid
			uint32_t m_uUnreadMsgNum; // �û������gmsgcache�����δ����Ϣ��

			uint32_t uFolderId; // ��id
			uint32_t uGroupId; // Ⱥid

			// CServerTopicMsg ������ PGMsgCacheType.h ��
			std::vector<CServerTopicMsg> vecChatMsgList; //���շ���ʱ�䵹������ 

			PSS_GChatMsgGetAccessRes() {}
			PSS_GChatMsgGetAccessRes(const PSS_GChatMsgGetAccessReq& obj, std::vector<CServerTopicMsg>& vecTopicMsg)
				: m_uid(obj.m_uid), m_linkdId(obj.m_linkdId), m_uUnreadMsgNum(obj.m_uUnreadMsgNum), 
				  uFolderId(obj.uFolderId), uGroupId(obj.uGroupId), vecChatMsgList(vecTopicMsg) 
			{}

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uid << m_linkdId << m_uUnreadMsgNum << uFolderId << uGroupId ;
				marshal_container(p, vecChatMsgList);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uid >> m_linkdId >> m_uUnreadMsgNum >> uFolderId >> uGroupId ;
				unmarshal_containerEx(p, vecChatMsgList);
			}
		};

		// since yy3.3
		// ����Ϣ��Access ����Gmsgcache�����ڶ�PSS_GChatPopInfoAccessReq ����Ӧ
		// ����ǰ5����Gmsgcache��д, Accessԭ�����
		struct PSS_GChatPopInfoAccessRes : public sox::Marshallable 
		{
			enum {uri = (63 << 8) | protocol::ACCESS_SVID};
													
			uint32_t		m_uid;       	// �������û�id
			uint32_t		m_linkdId;   	// ������imlinkdid
			uint32_t		m_groupId;   	// ����Chat��Ⱥid
			uint32_t		m_folderId;  	// ��������id
			uint32_t 		m_fetchCount;	// GMsgcache��Access��ȡ�Ĵ���, ��һ��Ϊ1, �ڶ���Ϊ2, ����...

			uint32_t		m_accessIP;    	// filled by Access, for gmsgcache to retrieve popInfo from another computer room
			SChatPopInfo	m_chatPopInfo; 	// filled by Access, msg sum + last reply timestamp

			PSS_GChatPopInfoAccessRes() {}
			PSS_GChatPopInfoAccessRes(uint32_t uid, uint32_t linkdId, uint32_t groupId, uint32_t folderId, uint32_t fetchCount, 
									  uint32_t accessIP, const SChatPopInfo& chatPopInfo)
				: m_uid(uid), m_linkdId(linkdId), m_groupId(groupId), m_folderId(folderId), m_fetchCount(fetchCount),
				  m_accessIP(accessIP), m_chatPopInfo(chatPopInfo)
			{}

			PSS_GChatPopInfoAccessRes(const PSS_GChatPopInfoAccessReq& objReq, uint32_t accessIP, const SChatPopInfo& chatPopInfo)
				: m_uid(objReq.m_uid), m_linkdId(objReq.m_linkdId), m_groupId(objReq.m_groupId), m_folderId(objReq.m_folderId),
				  m_fetchCount(objReq.m_fetchCount), m_accessIP(accessIP), m_chatPopInfo(chatPopInfo)
			{}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uid << m_linkdId << m_groupId << m_folderId << m_fetchCount << m_accessIP;
				m_chatPopInfo.marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uid >> m_linkdId >> m_groupId >> m_folderId >> m_fetchCount >> m_accessIP;
				m_chatPopInfo.unmarshal(p);
			}	
		};	

		// from gmsgreadinfo to Access
		struct PSS_GetGrpMsgReadInfoReq : public sox::Marshallable
		{
			enum {uri = (64 << 8 | protocol::ACCESS_SVID)};

			uint32_t uUid;
			uint32_t uImlinkdId;

			virtual void marshal(sox::Pack &p) const 
			{
				p << uUid << uImlinkdId;
			}
			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> uUid >> uImlinkdId;
			}
		};

		// from access to gmsgreadinfo
		struct PSS_GetGrpMsgReadInfoRes : public sox::Marshallable
		{
			enum {uri = (64 << 8 | protocol::ACCESS_SVID)};

			uint32_t uUid;
			uint32_t uImlinkdId;

			std::map<uint32_t, protocol::gtopic::STopicReadInfoA>  mapReadInfo;

			virtual void marshal(sox::Pack &p) const 
			{
				p << uUid << uImlinkdId;
				sox::marshal_container(p, mapReadInfo);
			}
			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> uUid >> uImlinkdId;
				sox::unmarshal_container(p, std::inserter(mapReadInfo, mapReadInfo.begin()));
			}
		};

		// from gmsgreadinfo to access
		struct PSS_TokenReq : public sox::Marshallable 
		{
			enum {uri = (65 << 8) | protocol::ACCESS_SVID};

			uint32_t uUid;
			uint32_t uImlinkdId;
			uint32_t uToken;

			virtual void marshal(sox::Pack &p) const 
			{
				p  << uUid << uImlinkdId << uToken;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p  >> uUid >> uImlinkdId >> uToken;
			}
		};

		// from access to gmsgreadinfo
		struct PSS_TokenRes : public sox::Marshallable 
		{
			enum {uri = (65 << 8) | protocol::ACCESS_SVID};

			uint32_t uUid;
			uint32_t uImlinkdId;
			uint32_t uToken; // �û������е�ԭʼtoken
			uint32_t bChecked;

			virtual void marshal(sox::Pack &p) const 
			{
				p << uUid << uImlinkdId << uToken << bChecked;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> uUid >> uImlinkdId >> uToken >> bChecked;
			}
		};

		// forward from gmsgreadinfo to access_svid or vise versa
		struct PSS_UserGTopicsReadInfo : public sox::Marshallable
		{
			enum {uri = (66 << 8) | protocol::ACCESS_SVID};

			uint32_t uUid;
			std::vector<protocol::gtopic::STopicReadInfo>  m_vecTopics;

			PSS_UserGTopicsReadInfo()
				: uUid(-1)
			{
			}

			PSS_UserGTopicsReadInfo(uint32_t uid, const std::vector<protocol::gtopic::STopicReadInfo>& vecTopics)
				: uUid(uid), m_vecTopics(vecTopics)
			{
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << uUid;
				marshal_container(p, m_vecTopics);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> uUid;
				unmarshal_containerEx(p, m_vecTopics);
			}
		};

		struct PSS_MsgReqWithIP : public sox::Marshallable
		{
			enum {uri = (67 << 8) | protocol::ACCESS_SVID};

			uint32_t m_uIp;      		// �������û�ip��������
			uint32_t m_uOriUri; 		//ԭ��uri���������ɷ�ԭ��ʱʹ��
			std::string m_strOriPacket; //ԭ������

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uIp << m_uOriUri << m_strOriPacket;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uIp >> m_uOriUri >> m_strOriPacket;
			}
		};

		// forward from gmsgreadinfo to access_svid or vise versa
		struct PSS_SetToken : public sox::Marshallable
		{
			enum {uri = (68 << 8) | protocol::ACCESS_SVID};

			uint32_t uUid;
			uint32_t uToken;

			PSS_SetToken()
				: uUid(0), uToken(0)
			{
			}

			PSS_SetToken(uint32_t uid, uint32_t token)
				: uUid(uid), uToken(token)
			{
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << uUid << uToken;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> uUid >> uToken;
			}
		};
    }
}

#endif
