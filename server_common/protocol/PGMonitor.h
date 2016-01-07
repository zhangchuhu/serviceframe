#pragma once 

#include "common/packet.h"
#include "common/core/base_svid.h"

#include "common/protocol/PGTopicMsg.h"

#include <iostream>
#include <string>
#include <vector>
#include <map>

namespace protocol
{
	namespace gmonitor
	{

		//删除群
		
		struct PSS_DeleteGroup : public sox::Marshallable
		{
			enum {uri = (1 << 8 | protocol::GMONITOR_SVID)};

			uint32_t m_uSenderID;
			uint32_t m_uGrpID;

			PSS_DeleteGroup() {};
			PSS_DeleteGroup(uint32_t uSenderID, uint32_t uGrpID) : 
						m_uSenderID(uSenderID), m_uGrpID(uGrpID) {};

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uSenderID << m_uGrpID;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uSenderID >> m_uGrpID;
			}
		};

		//删除分组

		struct PSS_DeleteFolder : public sox::Marshallable
		{
			enum {uri = (2 << 8 | protocol::GMONITOR_SVID)};

			uint32_t m_uSenderID;
			uint32_t m_uGrpID;
			uint32_t m_uFldID;

			PSS_DeleteFolder() {};
			PSS_DeleteFolder(uint32_t uSenderID, uint32_t uGrpID, uint32_t uFldID) : 
						m_uSenderID(uSenderID), m_uGrpID(uGrpID), m_uFldID(uFldID) {};

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uSenderID << m_uGrpID << m_uFldID;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uSenderID >> m_uGrpID >> m_uFldID;
			}
		};

		//删除主题

		struct PSS_DeleteTopic : public sox::Marshallable
		{
			enum {uri = (3 << 8 | protocol::GMONITOR_SVID)};

			uint32_t              m_uSenderID;
			uint32_t              m_uGrpID;
			uint32_t              m_uFldID;
			std::vector<uint32_t> m_vecTopicID;

			PSS_DeleteTopic() {};
			PSS_DeleteTopic(uint32_t uSenderID, uint32_t uGrpID, uint32_t uFldID, const std::vector<uint32_t>& vecTopicID) 
				            :m_uSenderID(uSenderID), m_uGrpID(uGrpID), m_uFldID(uFldID) 
			{
				m_vecTopicID = vecTopicID;
			};

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uSenderID << m_uGrpID << m_uFldID;
				sox::marshal_container(p, m_vecTopicID); 
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uSenderID >> m_uGrpID >> m_uFldID;
				sox::unmarshal_container(p, std::back_inserter(m_vecTopicID));
			}
		};

		//删除回贴

		struct PSS_DeleteMessage : public sox::Marshallable
		{
			enum {uri = (4 << 8 | protocol::GMONITOR_SVID)};

			uint32_t m_uSenderID;
			uint32_t m_uGrpID;
			uint32_t m_uFldID;
			uint32_t m_uTopicID;
			std::vector<uint64_t> m_vecMessageID;

			PSS_DeleteMessage() {};
			PSS_DeleteMessage(uint32_t uSenderID, uint32_t uGrpID, uint32_t uFldID, uint32_t uTopicID, const std::vector<uint64_t> &vecMessageID) : 
						m_uSenderID(uSenderID), m_uGrpID(uGrpID), m_uFldID(uFldID), m_uTopicID(uTopicID), m_vecMessageID(vecMessageID) {};

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uSenderID << m_uGrpID << m_uFldID << m_uTopicID;
				sox::marshal_container(p, m_vecMessageID);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uSenderID >> m_uGrpID >> m_uFldID >> m_uTopicID;
				sox::unmarshal_container(p, std::back_inserter(m_vecMessageID));
			}
		};

		//修改回贴

		struct PSS_UpdateMessage : public sox::Marshallable
		{
			enum {uri = (4 << 8 | protocol::GMONITOR_SVID)};

			uint32_t m_uSenderID;
			uint32_t m_uGrpID;
			uint32_t m_uFldID;
			uint32_t m_uTopicID;
			std::map<uint64_t, protocol::gmsgcache::GTopicTextChat> m_mapMessage;

			typedef std::map<uint64_t, protocol::gmsgcache::GTopicTextChat> MAP_MESSAGE;
			typedef const std::map<uint64_t, protocol::gmsgcache::GTopicTextChat> CONST_MAP_MESSAGE;

			PSS_UpdateMessage() {};
			PSS_UpdateMessage(uint32_t uSenderID, uint32_t uGrpID, uint32_t uFldID, uint32_t uTopicID, CONST_MAP_MESSAGE &mapMessage) : 
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

		struct SUpdateMsg : public sox::Marshallable
		{
			uint32_t     m_uId;    // 用于更新主题消息时, 它是tid; 
								   // 用于更新默认聊天区消息时, it is fid
			std::vector<uint64_t> m_vecTimeStamps;

			SUpdateMsg(){}
			SUpdateMsg(uint32_t id, std::vector<uint64_t>& vec)
				: m_uId(id), m_vecTimeStamps(vec)
			{
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uId;
				sox::marshal_container(p, m_vecTimeStamps);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uId;
				sox::unmarshal_container(p, std::back_inserter(m_vecTimeStamps));
			}
		};

		//修改主题回帖
		struct PSS_UpdateTopicMsg : public sox::Marshallable
		{
			enum {uri = (5 << 8 | protocol::GMONITOR_SVID)};

			uint32_t m_uGrpID;
			protocol::gmsgcache::GTopicTextChat m_msgText; // 希望更新后消息的内容
			std::vector<SUpdateMsg> m_vecTopicMsgs;

			PSS_UpdateTopicMsg() {};
			PSS_UpdateTopicMsg(uint32_t uGrpID, 
										const protocol::gmsgcache::GTopicTextChat text, 
										const std::vector<SUpdateMsg>& vec) : 
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
		struct PSS_UpdateChatMsg : public sox::Marshallable
		{
			enum {uri = (6 << 8 | protocol::GMONITOR_SVID)};

			uint32_t m_uGrpID;
			protocol::gmsgcache::GTopicTextChat m_msgText; // 希望更新后消息的内容
			std::vector<SUpdateMsg> m_vecChatMsgs;

			PSS_UpdateChatMsg() {};
			PSS_UpdateChatMsg(uint32_t uGrpID, 
										const protocol::gmsgcache::GTopicTextChat text, 
										const std::vector<SUpdateMsg>& vec) : 
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

		struct PSS_KickUser : public sox::Marshallable
		{
			enum {uri = (7 << 8 | protocol::GMONITOR_SVID)};

			uint32_t  m_uUID;
			uint32_t  m_aliasID;		
			std::set<uint32_t> m_setGIDs;	

			PSS_KickUser() {};
			PSS_KickUser(uint32_t uUID, uint32_t uAliasID,const std::set<uint32_t> &setGIDs) : m_uUID(uUID), m_aliasID(uAliasID),m_setGIDs(setGIDs) {};

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUID<<m_aliasID;
				sox::marshal_container(p, m_setGIDs);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUID>>m_aliasID;
				sox::unmarshal_container(p, std::inserter(m_setGIDs, m_setGIDs.begin()));
			}
		};

		struct PSS_AppointGroupOwner : public sox::Marshallable
		{
			enum {uri = (8 << 8 | protocol::GMONITOR_SVID)};

			uint32_t m_uUID;
			uint32_t m_uGrpID;

			PSS_AppointGroupOwner(){};
			PSS_AppointGroupOwner(uint32_t uUID, uint32_t uGrpID) : m_uUID(uUID), m_uGrpID(uGrpID){};

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUID << m_uGrpID;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUID >> m_uGrpID;
			}
		};

		struct PSS_NewGroupFromGmonitor : public sox::Marshallable
		{
			enum {uri = (9 << 8 | protocol::GMONITOR_SVID)};

			uint32_t m_uUID;
			uint32_t m_uAliasID;
			std::string m_strName;

			PSS_NewGroupFromGmonitor(){};
			PSS_NewGroupFromGmonitor(uint32_t uUID, uint32_t uAliasID,std::string name) : m_uUID(uUID), m_uAliasID(uAliasID),m_strName(name){};

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUID << m_uAliasID << m_strName;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUID >> m_uAliasID >> m_strName;
			}
		};
	}
}
