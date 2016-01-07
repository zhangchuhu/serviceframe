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
        // 该消息由Gtransfer 发给Access 服务器，用于存储主题回帖
        // 消息数据来自Gtransfer 收到的PCS_GTopicMsg,
        // 但是GTransfer 要填写时间，发言者和回帖序号
        struct PSS_GTopicMsg : public sox::Marshallable
        {
            enum {uri = (0 << 8) | protocol::ACCESS_SVID};
            
            uint32_t     m_topicId;      // 主题ID
            uint32_t     m_groupId;      // 群id
            uint32_t     m_folderId;     // 组id

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

		// 获取最热的消息数, 由GmsgCache向Access请求
		// 参考 psearch.h的PSS_GTopicMsgListReq /Res
		// 不同之处在于GmsgCache要填写发送者id，发送者imlinkdid
		// Access原样填回
		// 另外现在 guid全部改成了 uint64_t了
		struct PSS_GTopicMsgListAccessReq: public sox::Marshallable 
		{
			enum {uri = (50 << 8) | protocol::ACCESS_SVID};

			uint32_t m_version;

			uint32_t m_uid;      // 请求者用户id
			uint32_t m_linkdId;  // 请求者imlinkdid

			uint32_t uTopicId; // 主题ID
			uint32_t uFolderId; // 组id
			uint32_t uGroupId; // 群id
			//std::string uuidMsgStart; //起点消息编号
			//std::string uuidMsgStop; //终点消息编号
			uint32_t uMsgCount; //需要的消息条数，默认是20

			uint64_t uTimestampStart; //起点消息编号
			uint64_t uTimestampStop; //终点消息编号

			/*注意：
			 0.游标方式获取消息，从uTimestampStart消息往前取uMsgCount条,或者碰到uTimestampStop为止
			 1.应该保证uTimestampStart比uTimestampStop来得更新，即时间戳更晚
			 2.如果uTimestampStart为0，则从最新消息开始取
			 3.uMsgCount在uTimestampStop为0时才有意义，当uTimestampStop非0时，忽略uMsgCount
			 4.返回结果包含uTimestampStart消息，但不包括uTimestampStop消息
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

		// 该消息由Access发给Gmsgcache，是对PSS_GTopicMsgListAccessReq的响应
		struct PSS_GTopicMsgListAccessRes: public sox::Marshallable 
		{
			enum {uri = (50 << 8) | protocol::ACCESS_SVID};

			uint32_t m_version;

			uint32_t m_uid;      // 请求者用户id
			uint32_t m_linkdId;  // 请求者imlinkdid

			uint32_t uLeftTopicMsgCount; //该消息之前的剩余消息总量
			//std::string uuidMsgNext; //下一条消息的编号；如果为空则表示往前没有消息了
			uint64_t uTimestampNext; //下一条消息的编号；如果为空则表示往前没有消息了
			uint32_t uTopicId; // 主题ID
			uint32_t uFolderId; // 组id
			uint32_t uGroupId; // 群id

			//std::vector<STopicMsgInfo> vecTopicMsgList; //按照发表时间倒序排列
			//CServerTopicMsg定义在PGMsgCacheType.h中
			std::vector<CServerTopicMsg> vecTopicMsgList; //按照发表时间倒序排列 

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

		// 由GmsgCache通知Access 主题的消息已读，每个topic对应一个消息总数
		// Access检查消息总数是否和本地一致
		// 该请求是一个通知，通常无需响应
		// 但如果消息总数和本地不一致的时候要返回正确的pop info ：用 PSS_GTopicPopInfoAccessRes
		// 返回PSS_GTopicPopInfoAccessRes的时候请原样填好这时发过去的m_uOriUri
		struct PSS_GTopicReadMsgNotifyReq : public sox::Marshallable 
		{
			enum {uri = (51 << 8) | protocol::ACCESS_SVID};

			uint32_t m_uOriUri;   // 原始请求uri
			uint32_t m_uid;		// 请求者uid
			uint32_t m_groupId; // 请求者群id
			uint32_t m_folderId; // 请求者组id

			//uint32_t  m_uTopicId;      
			//uint32_t  m_uSumMsgNum;    
			//uint32_t  m_uUpdateTime;   
			//如果gmsgcache本地cache miss，后两项设置为0传给Access
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

		// 该消息由GmsgCache发给Access，用于请求PopInfo
		// 消息数据来自GmsgCache收到的PSS_GTopicPopInfoReq,
		// Access 需要填写STopicPopInfo中的消息总数和最后回复时间
		// Access原样填回发送者id，发送者imlinkdid，以及m_uOriUri
		struct PSS_GTopicPopInfoAccessReq : public sox::Marshallable
		{
			enum {uri = (52 << 8) | protocol::ACCESS_SVID};

			uint32_t   m_uOriUri;   // 原始请求uri

			uint32_t   m_uid;      // 请求者用户id
			uint32_t   m_linkdId;  // 请求者imlinkdid
								
			uint32_t   m_groupId;  // 请求者群id
			uint32_t   m_folderId; // 请求者组id
			std::vector<STopicPopInfo>  m_vecTopics; // 请求的主题 集合

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

		// 该消息由Access 发给Gmsgcache，用于对PSS_GTopicPopInfoAccessReq 的响应
		struct PSS_GTopicPopInfoAccessRes : public sox::Marshallable 
		{
			enum {uri = (52 << 8) | protocol::ACCESS_SVID};

			//gmscache有两种情况会要求 PopInfo
			//   1）PSS_GTopicPopInfoAccessReq
			//   2）PSS_GTopicReadedMsgNotifyReq Access如果发现其中消息总数和本地数据库不符合
			//       会返回pop info
			// 因此必须有这项, gmsgcache来区分究竟是收到本条响应后如何处理。
			uint32_t   m_uOriUri;   
									
			uint32_t   m_uid;       // 请求者用户id
			uint32_t   m_linkdId;   // 请求者imlinkdid
			uint32_t   m_groupId;   // 请求者群id
			uint32_t   m_folderId;  // 请求者组id
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

		// 主题列表请求, 由GmsgCache向Access请求
		// 参考的是psearch.h的PSS_GTopicListReq/Res
		// 不同之处在于GmsgCache要填写发送者id，发送者imlinkdid
		// Access原样填回
		// 另外响应的时候Access多填一项返回的主题总数
		struct PSS_GTopicListAccessReq: public sox::Marshallable 
		{
			enum {uri = (53 << 8) | protocol::ACCESS_SVID};

			uint32_t m_uOriUri;  //原始请求uri

			uint32_t m_uid;       // 请求者用户id
			uint32_t m_linkdId;   // 请求者imlinkdid

			uint32_t uFolderId; // 组id
			uint32_t uGroupId; // 群id
			uint32_t uStartTime; //开始时间
			uint32_t uStopTime; //结束时间
			uint32_t uTopicCount; //每页数量，默认是20

			/*注意：
			 0.游标方式获取主题ID列表，从uStartTime消息往前取uTopicCount条,或者碰到uStopTime为止
			 1.应该保证uStartTime比uStopTime来得更新，即时间戳更晚
			 2.如果uStartTime为空，则从最新主题开始取
			 3.uTopicCount在uStopTime为0时才有意义，当uStopTime非0时，忽略uTopicCount
			 4.返回结果包含uStartTime时间点，但不包括uStopTime时间点
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

		//该消息由Access 发给Gmsgcache，用于对PSS_GTopicListAccessReq的响应
		struct PSS_GTopicListAccessRes: public sox::Marshallable 
		{
			enum {uri = (53 << 8) | protocol::ACCESS_SVID};

			uint32_t m_uOriUri;  //原始请求uri

			uint32_t m_uid;       // 请求者用户id
			uint32_t m_linkdId;   // 请求者imlinkdid

			uint32_t uFolderId; // 组id
			uint32_t uGroupId; // 群id
			uint32_t uTopicSum; // 返回的主题总数
			std::vector<uint32_t> vecTopicIDList; //按照主题最后更新时间倒序排列

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
//	End GmsgCache inter-communicate with Access ！
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

		// 该消息由GmsgCache发给Access，用于请求默认聊天区的PopInfo
		// 消息数据来自GmsgCache收到的 PCS_GChatPopInfoReq,
		// Access 需要返回消息总数和最后回复时间
		// 并原样填回发送者id，发送者imlinkdid，和fetchCount
		struct PSS_GChatPopInfoAccessReq : public sox::Marshallable
		{
			enum {uri = (55 << 8) | protocol::ACCESS_SVID};

			uint32_t	m_uid;      	// 请求者用户id
			uint32_t	m_linkdId;  	// 请求者imlinkdid							
			uint32_t	m_groupId;  	// 请求者群id
			uint32_t	m_folderId; 	// 请求者组id
										
			uint32_t 	m_fetchCount;	// since yy3.3,找Access获取的次数, 第一次为1, 第二次为2, 类推

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

		// 该消息由Access 发给Gmsgcache，用于对PSS_GChatPopInfoAccessReq 的响应
		// 从yy3.3开始由 63 << 8 取代
		struct PSS_GChatPopInfoAccessRes2 : public sox::Marshallable 
		{
			enum {uri = (55 << 8) | protocol::ACCESS_SVID};
											
			uint32_t	m_uid;       	// 请求者用户id
			uint32_t	m_linkdId;   	// 请求者imlinkdid
			uint32_t	m_groupId;   	// 请求Chat者群id
			uint32_t	m_folderId;  	// 请求者组id
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
		// 获取 默认聊天区 最热的消息数, 由GmsgCache向Access请求
		// GmsgCache要填写发送者id，发送者imlinkdid
		// Access原样填回
		struct PSS_GChatMsgListAccessReq: public sox::Marshallable 
		{
			enum {uri = (56 << 8) | protocol::ACCESS_SVID};

			uint32_t m_uid;      // 请求者用户id
			uint32_t m_linkdId;  // 请求者imlinkdid

			uint32_t uFolderId; // 组id
			uint32_t uGroupId; // 群id
	
			uint32_t uMsgCount; //需要的消息条数，默认是20

			uint64_t uTimestampStart; //起点消息编号
			uint64_t uTimestampStop; //终点消息编号

			/*注意：
			 0.游标方式获取消息，从uTimestampStart消息往前取uMsgCount条,或者碰到uTimestampStop为止
			 1.应该保证uTimestampStart比uTimestampStop来得更新，即时间戳更晚
			 2.如果uTimestampStart为0，则从最新消息开始取
			 3.uMsgCount在uTimestampStop为0时才有意义，当uTimestampStop非空时，忽略uMsgCount
			 4.返回结果包含uTimestampStart消息，但不包括uTimestampStop消息
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
		// 该消息由Access发给Gmsgcache，是对PSS_GTopicMsgListAccessReq的响应
		struct PSS_GChatMsgListAccessRes: public sox::Marshallable 
		{
			enum {uri = (56 << 8) | protocol::ACCESS_SVID};

			uint32_t m_uid;      // 请求者用户id
			uint32_t m_linkdId;  // 请求者imlinkdid

			uint32_t uLeftTopicMsgCount; //该消息之前的剩余消息总量
			uint64_t uTimestampNext; //下一条消息的编号；如果为0则表示往前没有消息了

			uint32_t uFolderId; // 组id
			uint32_t uGroupId; // 群id

			//std::vector<STopicMsgInfo> vecTopicMsgList; //按照发表时间倒序排列
			//CServerTopicMsg定义在PGMsgCacheType.h中
			std::vector<CServerTopicMsg> vecChatMsgList; //按照发表时间倒序排列 

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
		
		//修改默认主题区回贴
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

		// GAuth 向 Access要某个成员的所有消息(主题消息、默认聊天区消息)，做Update(或叫删除)操作
		struct PSS_UpdateGMemberMsgsAccessReq: public sox::Marshallable 
		{
			enum {uri = (59 << 8) | protocol::ACCESS_SVID};

			uint32_t m_uSenderId;   // GAuth填好， Access原样填好
			uint32_t m_uImlinkdId;  // GAuth填好， Access原样填好
			uint32_t m_uSeqId;		// protocol填好的， Access原样填好
			uint32_t m_uSrvId;		// GAuth填好， Access用于决定下一跳(现在是GMsgCache)
			uint32_t m_uUid;	    // 待查的用户Id
			uint32_t m_uGroupId; 
			std::vector<uint32_t> m_vecFids; 

			protocol::gmsgcache::GTopicTextChat m_msgText; //客户端填好的, Access原样填回

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

        // 该消息由Gtransfer 发给Access 服务器，用于存储默认聊天区回帖
        // 消息数据来自Gtransfer 收到的PCS_GChatMsg,
        // 但是GTransfer 要填写时间，发言者和回帖序号
        struct PSS_GChatMsg : public sox::Marshallable
        {
            enum {uri = (60 << 8) | protocol::ACCESS_SVID};
            
            uint32_t     m_groupId;      // 群id
            uint32_t     m_folderId;     // 组id

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
		// 获取群最近的消息数, 由GmsgCache向Access请求
		struct PSS_GChatMsgGetAccessReq: public sox::Marshallable 
		{
			enum {uri = (61 << 8) | protocol::ACCESS_SVID};

			// 以下三个字段由GMsgcache填写
			uint32_t m_uid;      		// 请求者用户id
			uint32_t m_linkdId;  		// 请求者imlinkdid
			uint32_t m_uUnreadMsgNum; 	// 用户最初向gmsgcache请求的未读消息数

			uint32_t uFolderId;			// 组id
			uint32_t uGroupId; 			// 群id
	
			uint32_t uMsgCount; 		//需要的消息条数

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

		// 该消息由Access发给Gmsgcache，是对 PSS_GrpRecentMsgListReq 的响应
		struct PSS_GChatMsgGetAccessRes: public sox::Marshallable 
		{
			enum {uri = (62 << 8) | protocol::ACCESS_SVID};

			// 以下三个字段由GMsgcache填写, Access原样填回
			uint32_t m_uid;      // 请求者用户id
			uint32_t m_linkdId;  // 请求者imlinkdid
			uint32_t m_uUnreadMsgNum; // 用户最初向gmsgcache请求的未读消息数

			uint32_t uFolderId; // 组id
			uint32_t uGroupId; // 群id

			// CServerTopicMsg 定义在 PGMsgCacheType.h 中
			std::vector<CServerTopicMsg> vecChatMsgList; //按照发表时间倒序排列 

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
		// 该消息由Access 发给Gmsgcache，用于对PSS_GChatPopInfoAccessReq 的响应
		// 其中前5项由Gmsgcache填写, Access原样填回
		struct PSS_GChatPopInfoAccessRes : public sox::Marshallable 
		{
			enum {uri = (63 << 8) | protocol::ACCESS_SVID};
													
			uint32_t		m_uid;       	// 请求者用户id
			uint32_t		m_linkdId;   	// 请求者imlinkdid
			uint32_t		m_groupId;   	// 请求Chat者群id
			uint32_t		m_folderId;  	// 请求者组id
			uint32_t 		m_fetchCount;	// GMsgcache向Access获取的次数, 第一次为1, 第二次为2, 类推...

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
			uint32_t uToken; // 用户请求中的原始token
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

			uint32_t m_uIp;      		// 请求者用户ip，网络序
			uint32_t m_uOriUri; 		//原包uri，解析并派发原包时使用
			std::string m_strOriPacket; //原包数据

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
