#pragma once

#include "common/packet.h"
#include "common/core/base_svid.h"
#include "common/protocol/const.h"

#include "protocol/common/protocol/PGChat.h"
#include "protocol/common/protocol/PGTransfer.h" 

#include <iostream>
#include <string>

namespace server
{
	namespace gchat
	{
		struct SFolderLinkdList : public sox::Marshallable
		{
			uint32_t                  m_uFolderId;   // folder id
			std::vector<uint32_t>     m_vecLinkds;   // imlinkdid 

			SFolderLinkdList(){}
			SFolderLinkdList(uint32_t folderId, const std::vector<uint32_t>& vec)
				: m_uFolderId(folderId)
			{
				m_vecLinkds = vec;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uFolderId;
				sox::marshal_container(p, m_vecLinkds);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uFolderId;
				sox::unmarshal_container(p, std::back_inserter(m_vecLinkds));
			}
		};
		
		//update of imlinkdlist which is sent by imlinkd or gtransfer
		struct PSS_GImlinkdUpdate : public sox::Marshallable
		{
 			enum {uri = (101 << 8) | protocol::GCHAT_SVID};

			uint32_t     m_uGroupId;         // group id
			uint32_t     m_uOper;            // operation 0 is add, 1 is delete, 2 is gtransfer add
			std::vector<server::gchat::SFolderLinkdList>    m_vecFidLinkdList; // folders with imlinkd list

			PSS_GImlinkdUpdate(){}
			PSS_GImlinkdUpdate(uint32_t uGid, uint32_t uOper) : m_uGroupId(uGid), m_uOper(uOper)
			{
			}
			PSS_GImlinkdUpdate(uint32_t groupId, uint32_t oper, const std::vector<server::gchat::SFolderLinkdList>& vec)
				: m_uGroupId(groupId), m_uOper(oper)
			{
				m_vecFidLinkdList = vec;
			}

			// 不检查是否存在fid记录，直接添加（特定环境使用）
			void AddFidNoCheck(uint32_t uFid, uint32_t uLinkd)
			{
				m_vecFidLinkdList.push_back(SFolderLinkdList(uFid, std::vector<uint32_t>(1, uLinkd)));
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId << m_uOper;
				sox::marshal_container(p, m_vecFidLinkdList);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uGroupId >> m_uOper;
				sox::unmarshal_containerEx(p, m_vecFidLinkdList);
			}
		};

		
		//topic msg, gchat send it to imlinkd, imlinkd dispatch its original packet to clients
		struct PSS_GTopicMsgToImlinkd : public sox::Marshallable
		{
 			enum {uri = (102 << 8) | protocol::GCHAT_SVID};

			uint32_t     m_uGroupId;             // group id
			uint32_t     m_uFolderId;            // folder id
			uint32_t     m_uOriUri;              // original packet uri
			std::string  m_strOriPacket;         //original packet data

			PSS_GTopicMsgToImlinkd(){}
			PSS_GTopicMsgToImlinkd(uint32_t groupId, uint32_t folderId, uint32_t uUri, const std::string &strOriPacket)
				: m_uGroupId(groupId), m_uFolderId(folderId), m_uOriUri(uUri)
			{
				m_strOriPacket.assign(strOriPacket.data(), strOriPacket.size());
			}
			PSS_GTopicMsgToImlinkd(uint32_t groupId, uint32_t folderId, uint32_t uUri, const sox::Marshallable &objMarshal) 
				: m_uGroupId(groupId), m_uFolderId(folderId), m_uOriUri(uUri)
			{
				protocol::gtransfer::Packet2String(objMarshal, m_strOriPacket);
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId << m_uFolderId << m_uOriUri << m_strOriPacket;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uGroupId >> m_uFolderId >> m_uOriUri >> m_strOriPacket;
			}
		};
	
	//deliver all the gids and fids from imlinkd to gchatd
	struct PSS_GImlinkdRecover : public sox::Marshallable
	{
		enum {uri = (104 << 8) | protocol::GCHAT_SVID};

		uint32_t     m_uOper;            // operation 0 initial update to gchatd.
		uint32_t     m_uLinkdId;
		typedef std::map<uint32_t, std::set<uint32_t> > mapList;

		mapList  m_mapGid2SetFids;
		PSS_GImlinkdRecover(){}
		PSS_GImlinkdRecover(uint32_t oper,uint32_t linkd)
			:m_uOper(oper),m_uLinkdId(linkd)	{}
		PSS_GImlinkdRecover(uint32_t oper,uint32_t linkd,const mapList & map)
			:m_uOper(oper),m_uLinkdId(linkd)
		{
			m_mapGid2SetFids = map;
		}

		virtual void marshal(sox::Pack &p) const
		{
			p << m_uOper << m_uLinkdId;
			sox::marshal_container(p, m_mapGid2SetFids);
		}
		virtual void unmarshal(const sox::Unpack &p) 
		{
			p >> m_uOper >> m_uLinkdId;
			sox::unmarshal_container(p, std::inserter( m_mapGid2SetFids,  m_mapGid2SetFids.begin()));
		}
	};	

	struct PSS_GImlinkdRecoverReq : public sox::Marshallable
	{
		enum {uri = (105 << 8) | protocol::GCHAT_SVID};
		uint32_t m_uServerId;

		PSS_GImlinkdRecoverReq(){}
		PSS_GImlinkdRecoverReq(uint32_t sid)
			:m_uServerId(sid){}

		virtual void marshal(sox::Pack &p) const
		{
			p << m_uServerId;
  }
		virtual void unmarshal(const sox::Unpack &p) 
		{
			p >> m_uServerId;
		}
	};

	struct PSS_GDispatchMsg : public sox::Marshallable
	{
		enum {uri = (106 << 8) | protocol::GCHAT_SVID};
		std::set<uint32_t> m_setImlinkds;
		uint32_t     m_uSenderId;
		uint32_t	 m_uGroupId;			 // group id
		uint32_t	 m_uFolderId;			 // folder id
		uint32_t	 m_uOriUri; 			 // original packet uri
		std::string  m_strOriPacket;		 //original packet data

		PSS_GDispatchMsg(){}
		PSS_GDispatchMsg(uint32_t uSenderId, uint32_t groupId, uint32_t folderId, uint32_t uUri, const std::string &strOriPacket)
			: m_uSenderId(uSenderId), m_uGroupId(groupId), m_uFolderId(folderId), m_uOriUri(uUri)
		{
			m_strOriPacket.assign(strOriPacket.data(), strOriPacket.size());
		}
		PSS_GDispatchMsg(uint32_t uSenderId, uint32_t groupId, uint32_t folderId, uint32_t uUri, const sox::Marshallable &objMarshal) 
			: m_uSenderId(uSenderId), m_uGroupId(groupId), m_uFolderId(folderId), m_uOriUri(uUri)
		{
			protocol::gtransfer::Packet2String(objMarshal, m_strOriPacket);
		}

		virtual void marshal(sox::Pack &p) const
		{
			p << m_uSenderId << m_uGroupId << m_uFolderId << m_uOriUri << m_strOriPacket;
			sox::marshal_container(p, m_setImlinkds);
		}
		
		virtual void unmarshal(const sox::Unpack &p) 
		{
			p >> m_uSenderId >> m_uGroupId >> m_uFolderId >> m_uOriUri >> m_strOriPacket;
			sox::unmarshal_container(p, std::inserter( m_setImlinkds,  m_setImlinkds.begin()));
		}
	};

	struct PSS_GChatStat : public sox::Marshallable
	{
		enum {uri = (107 << 8) | protocol::GCHAT_SVID};
		uint32_t	   m_uGroupId;			   // group id
		uint32_t	   m_uFolderId; 		   // folder id
		std::string  m_strOriPacket;		   //original packet data
		std::vector<uint32_t> m_vecMembers;

		PSS_GChatStat(){}
		PSS_GChatStat(uint32_t groupId, uint32_t folderId, const sox::Marshallable &objMarshal) 
			: m_uGroupId(groupId), m_uFolderId(folderId)
		{
			protocol::gtransfer::Packet2String(objMarshal, m_strOriPacket);
		}

		virtual void marshal(sox::Pack &p) const
		{
			p << m_uGroupId << m_uFolderId << m_strOriPacket;
			sox::marshal_container(p, m_vecMembers);
		}

		virtual void unmarshal(const sox::Unpack &p) 
		{
			p >> m_uGroupId >> m_uFolderId >> m_strOriPacket;
			sox::unmarshal_container(p, std::back_inserter( m_vecMembers));
		}
	};

	}
}

