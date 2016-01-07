#ifndef PMSG_STORE_H
#define PMSG_STORE_H

#include <string>
#include "iproperty.h"
#include "protocol/const.h"
#include "core/base_svid.h"
#include "common/packet.h"
#include "common/protocol/PGMsgCache.h"
#include "common/protocol/psearch.h"

using namespace protocol::gmsgcache;

namespace server
{
    namespace store
    {   
		inline void Packet2String(const sox::Marshallable &objMarshal, std::string &strPacket)
		{
			//将原包序列化，并装载
			sox::PackBuffer pb;
			sox::Pack pk(pb);
			objMarshal.marshal(pk);

			strPacket.assign(pk.data(), pk.size());
		}

		enum E_PACK_STATUS_TYPE
		{
			E_NOT_PACK = 0,
			E_PACKING,
			E_PACED
		};

		enum E_TABLE_TYPE
		{
			E_IMMSG_TABLE = 0,
			E_GMSG_TABLE
		};

		struct PSS_MsgPack : public sox::Marshallable
		{
			enum {uri = (100 << 8) | protocol::ACCESS_SVID};

			E_TABLE_TYPE m_eType;
			uint32_t	 m_uDbid;
			std::string  m_strTblname;
			uint32_t     m_uIndex;
		
			PSS_MsgPack(){}
			PSS_MsgPack(E_TABLE_TYPE type, uint32_t dbid, const std::string& tblname, uint32_t index)
				: m_eType(type), m_uDbid(dbid), m_strTblname(tblname), m_uIndex(index)
			{}
		
			virtual void marshal(sox::Pack &p) const
			{
				p.push_uint8((uint8_t)m_eType);
				p << m_uDbid << m_strTblname << m_uIndex;
			}
		
			virtual void unmarshal(const sox::Unpack &p) 
			{
				m_eType = (E_TABLE_TYPE)p.pop_uint8();
				p >> m_uDbid >> m_strTblname >> m_uIndex;
			}
		};

		struct PSS_GPageInfoSync : public sox::Marshallable
		{
			enum {uri = (101 << 8) | protocol::ACCESS_SVID};

			uint32_t m_uGid;
			uint32_t m_uFid;
			uint32_t m_uLatestGid;
			uint32_t m_uMaxMsgCount;
			std::vector<uint32_t> m_vecGids;
			
			PSS_GPageInfoSync(){}
			PSS_GPageInfoSync(uint32_t gid, uint32_t fid, uint32_t groupid, std::vector<uint32_t>& vec, uint32_t msgcount)
			{
				m_uGid = gid;
				m_uFid = fid;
				m_uLatestGid = groupid;
				m_uMaxMsgCount = msgcount;
				m_vecGids = vec;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGid << m_uFid << m_uLatestGid << m_uMaxMsgCount;
				marshal_container(p, m_vecGids);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uGid >> m_uFid >> m_uLatestGid >> m_uMaxMsgCount;
				unmarshal_container(p, std::back_inserter(m_vecGids));
			}
		};

		struct PSS_GPageDataSync : public sox::Marshallable
		{
			enum {uri = (102 << 8) | protocol::ACCESS_SVID};

			uint32_t m_uGid;
			uint32_t m_uFid;
			uint32_t m_uPageId;
			uint32_t m_uMsgCount;
			uint64_t m_uBeginTS;
			uint64_t m_uEndTS;
			uint32_t m_uMsgTotalCount;
			
			PSS_GPageDataSync(){}
			PSS_GPageDataSync(uint32_t gid, uint32_t fid, uint32_t pageid, uint32_t count, uint64_t begin, uint64_t end, uint32_t totalcount)
			{
				m_uGid = gid;
				m_uFid = fid;
				m_uPageId = pageid;
				m_uMsgCount = count;
				m_uBeginTS = begin;
				m_uEndTS = end;
				m_uMsgTotalCount = totalcount;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGid << m_uFid << m_uPageId << m_uMsgCount << m_uBeginTS << m_uEndTS << m_uMsgTotalCount;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uGid >> m_uFid >> m_uPageId >> m_uMsgCount >> m_uBeginTS >> m_uEndTS >> m_uMsgTotalCount;
			}
		};
		
		struct PSS_GPageDataSyncRes : public sox::Marshallable
		{
			enum {uri = (103 << 8) | protocol::ACCESS_SVID};

			uint32_t m_uGid;
			uint32_t m_uFid;
			uint32_t m_uPageId;
			uint32_t m_uMsgCount;
			uint64_t m_uBeginTS;
			uint64_t m_uEndTS;
			std::vector<protocol::gmsgcache::CServerTopicMsg> m_vecMsgs;
			uint32_t m_uMsgTotalCount;
			
			PSS_GPageDataSyncRes(){}
			PSS_GPageDataSyncRes(uint32_t gid, uint32_t fid, uint32_t pageid, uint32_t count, uint64_t begin, uint64_t end, uint32_t totalcount)
			{
				m_uGid = gid;
				m_uFid = fid;
				m_uPageId = pageid;
				m_uMsgCount = count;
				m_uBeginTS = begin;
				m_uEndTS = end;
				m_uMsgTotalCount = totalcount;
			}
			PSS_GPageDataSyncRes(uint32_t gid, uint32_t fid, uint32_t pageid, uint32_t count, uint64_t begin, uint64_t end,
							const std::vector<protocol::gmsgcache::CServerTopicMsg>& vec, uint32_t totalcount)
			{
				m_uGid = gid;
				m_uFid = fid;
				m_uPageId = pageid;
				m_uMsgCount = count;
				m_uBeginTS = begin;
				m_uEndTS = end;
				m_vecMsgs = vec;
				m_uMsgTotalCount = totalcount;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGid << m_uFid << m_uPageId << m_uMsgCount << m_uBeginTS << m_uEndTS << m_uMsgTotalCount;
				marshal_container(p, m_vecMsgs);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uGid >> m_uFid >> m_uPageId >> m_uMsgCount >> m_uBeginTS >> m_uEndTS >> m_uMsgTotalCount;
				unmarshal_containerEx(p, m_vecMsgs);
			}
		};

		//msgstore 向access请求消息总数和最近的消息
		struct PSS_GMsgInfoAccessReq : public sox::Marshallable
		{
			enum {uri = (104 << 8) | protocol::ACCESS_SVID};

			uint32_t	m_uid;      	// 请求者用户id
			uint32_t	m_linkdId;  	// 请求者imlinkdid	
			uint32_t	m_groupId;  	// 请求者群id
			uint32_t	m_folderId; 	// 请求者组id

			uint32_t m_uOriUri; //原包uri，解析并派发原包时使用
			std::string m_strOriPacket; //原包数据

			PSS_GMsgInfoAccessReq(){}
			
			PSS_GMsgInfoAccessReq(uint32_t uid, uint32_t linkdId, uint32_t groupid, uint32_t folderId, 
												uint32_t uUri, const sox::Marshallable &objMarshal)
				: m_uid(uid), m_linkdId(linkdId), m_groupId(groupid), m_folderId(folderId), m_uOriUri(uUri)
			{
				server::store::Packet2String(objMarshal, m_strOriPacket);
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uid << m_linkdId << m_groupId << m_folderId << m_uOriUri << m_strOriPacket;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uid >> m_linkdId >> m_groupId >> m_folderId >> m_uOriUri >> m_strOriPacket;
			}
		};
		
		//msgstore 向access请求消息总数和最近的消息的响应
		struct PSS_GMsgInfoAccessRes : public sox::Marshallable 
		{
			enum {uri = (105 << 8) | protocol::ACCESS_SVID};
													
			uint32_t		m_uid;       	// 请求者用户id
			uint32_t		m_linkdId;   	// 请求者imlinkdid
			uint32_t		m_groupId;  	// 请求者群id
			uint32_t		m_folderId;  	// 请求者组id
			uint32_t        m_msgcount;
			
			//按照发表时间降序排列
			//从第一条消息开始分页，如果最后一页为60条的话，返回最后一页，
			//如果最后一页不为60条的话，返回最后两页
			//务必保证msgstore有大于等于60条消息，以便获取最新60条消息
			std::vector<CServerTopicMsg> vecChatMsgList; 

			uint32_t m_uOriUri; //原包uri，解析并派发原包时使用
			std::string m_strOriPacket; //原包数据

			PSS_GMsgInfoAccessRes() {}
			PSS_GMsgInfoAccessRes(uint32_t uid, uint32_t linkdId, uint32_t groupid, uint32_t folderId, uint32_t msgcount, 
									const std::vector<CServerTopicMsg>& vecmsg, uint32_t oriuri, std::string& stroripacket)
				: m_uid(uid), m_linkdId(linkdId), m_groupId(groupid), m_folderId(folderId), m_msgcount(msgcount), m_uOriUri(oriuri), m_strOriPacket(stroripacket)
			{
				vecChatMsgList = vecmsg;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uid << m_linkdId << m_groupId << m_folderId << m_msgcount << m_uOriUri << m_strOriPacket;
				marshal_container(p, vecChatMsgList);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uid >> m_linkdId >> m_groupId >> m_folderId >> m_msgcount >> m_uOriUri >> m_strOriPacket;
				unmarshal_containerEx(p, vecChatMsgList);
			}
		};
		
		struct PSS_MsgPackGidFid : public sox::Marshallable
		{
			enum {uri = (106 << 8) | protocol::ACCESS_SVID};
		
			uint32_t	 m_uDbid;
			std::string  m_strTblname;
			uint32_t     m_uGid;
			uint32_t     m_uFid;
		
			PSS_MsgPackGidFid(){}
			PSS_MsgPackGidFid(uint32_t dbid, const std::string& tblname, uint32_t gid, uint32_t fid)
				: m_uDbid(dbid), m_strTblname(tblname), m_uGid(gid), m_uFid(fid)
			{}
		
			virtual void marshal(sox::Pack &p) const
			{
				p << m_uDbid << m_strTblname << m_uGid << m_uFid;
			}
		
			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uDbid >> m_strTblname >> m_uGid >> m_uFid;
			}
		};

		//msgstore 向access请求消息总数和最近的消息
		struct PSS_ImMsgInfoAccessReq : public sox::Marshallable
		{
			enum {uri = (107 << 8) | protocol::ACCESS_SVID};

			uint32_t	m_uid;      	// 请求者用户id
			uint32_t	m_linkdId;  	// 请求者imlinkdid	
			uint32_t	m_fromId;  	// 请求者群id
			uint32_t	m_toId; 	// 请求者组id

			uint32_t m_uOriUri; //原包uri，解析并派发原包时使用
			std::string m_strOriPacket; //原包数据

			PSS_ImMsgInfoAccessReq(){}
			
			PSS_ImMsgInfoAccessReq(uint32_t uid, uint32_t linkdId, uint32_t fromid, uint32_t toid, 
												uint32_t uUri, const sox::Marshallable &objMarshal)
				: m_uid(uid), m_linkdId(linkdId), m_fromId(fromid), m_toId(toid), m_uOriUri(uUri)
			{
				server::store::Packet2String(objMarshal, m_strOriPacket);
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uid << m_linkdId << m_fromId << m_toId << m_uOriUri << m_strOriPacket;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uid >> m_linkdId >> m_fromId >> m_toId >> m_uOriUri >> m_strOriPacket;
			}
		};

		//msgstore 向access请求消息总数和最近的消息的响应
		struct PSS_ImMsgInfoAccessRes : public sox::Marshallable 
		{
			enum {uri = (108 << 8) | protocol::ACCESS_SVID};
													
			uint32_t		m_uid;       	// 请求者用户id
			uint32_t		m_linkdId;   	// 请求者imlinkdid
			uint32_t		m_fromId;  	// 发送者id
			uint32_t		m_toId;  	//接受者id
			uint32_t        m_msgcntfrom;
			uint32_t        m_msgcntto;
			
			//按照发表时间升序排列
			//从第一条消息开始分页，如果最后一页为60条的话，返回最后一页，
			//如果最后一页不为60条的话，返回最后两页
			//务必保证msgstore有大于等于60条消息，以便获取最新60条消息
			std::vector<protocol::search::SChatMsgInfoGrp> vecChatMsgList; 

			uint32_t m_uOriUri; //原包uri，解析并派发原包时使用
			std::string m_strOriPacket; //原包数据

			PSS_ImMsgInfoAccessRes() {}
			PSS_ImMsgInfoAccessRes(uint32_t uid, uint32_t linkdId, uint32_t fromid, uint32_t toid, uint32_t msgcount1, uint32_t msgcount2, 
									const std::vector<protocol::search::SChatMsgInfoGrp>& vecmsg, uint32_t oriuri, std::string& stroripacket)
				: m_uid(uid), m_linkdId(linkdId), m_fromId(fromid), m_toId(toid), m_msgcntfrom(msgcount1), m_msgcntto(msgcount2), m_uOriUri(oriuri), m_strOriPacket(stroripacket)
			{
				vecChatMsgList = vecmsg;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uid << m_linkdId << m_fromId << m_toId << m_msgcntfrom << m_msgcntto << m_uOriUri << m_strOriPacket;
				marshal_container(p, vecChatMsgList);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uid >> m_linkdId >> m_fromId >> m_toId >> m_msgcntfrom >> m_msgcntto >> m_uOriUri >> m_strOriPacket;
				unmarshal_containerEx(p, vecChatMsgList);
			}
		};

		struct PSS_MsgPackUidBid : public sox::Marshallable
		{
			enum {uri = (109 << 8) | protocol::ACCESS_SVID};
		
			uint32_t	 m_uDbid;
			std::string  m_strTblname;
			uint32_t     m_uUid;
			uint32_t     m_uBid;
		
			PSS_MsgPackUidBid(){}
			PSS_MsgPackUidBid(uint32_t dbid, const std::string& tblname, uint32_t uid, uint32_t bid)
				: m_uDbid(dbid), m_strTblname(tblname), m_uUid(uid), m_uBid(bid)
			{}
		
			virtual void marshal(sox::Pack &p) const
			{
				p << m_uDbid << m_strTblname << m_uUid << m_uBid;
			}
		
			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uDbid >> m_strTblname >> m_uUid >> m_uBid;
			}
		};
    }
}

#endif
