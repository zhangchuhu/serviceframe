#pragma once 

#include "common/packet.h"
#include "common/core/base_svid.h"
#include "common/protocol/const.h"
#include "common/protocol/pgtopicbase.h"

namespace server
{
	namespace gtopiccache
	{
		typedef protocol::gtopic::GTopicBaseInfoSvr SGTopicBaseInfo;
		typedef protocol::gtopic::GTopicReqMask SGTopicInfoMask;

		// gtopic to gtopiccache, notify a topic is updated.
		struct PSS_GTPCacheUpdate:public sox::Marshallable
		{
			enum {uri = (1 << 8 | protocol::GTOPICCACHE_SVID)};
			
			uint32_t m_uMask;
			SGTopicBaseInfo m_objTopic;

			PSS_GTPCacheUpdate(){ m_uMask = 0;}
			
			PSS_GTPCacheUpdate(const uint32_t & uMask, const SGTopicBaseInfo& tempTopic):
				m_uMask(uMask),
				m_objTopic(tempTopic)
			{
			}
			
			virtual void marshal(sox::Pack &p) const
			{
				p << m_uMask;
				m_objTopic.marshal(p);
			}
			
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uMask;
				m_objTopic.unmarshal(p);
			}

		};

		// gtopic to gtopiccache, notify a topic is created.
		struct PSS_GTPCacheCreate:public sox::Marshallable
		{
			enum {uri = (2 << 8 | protocol::GTOPICCACHE_SVID)};
			
			SGTopicBaseInfo m_objTopic;

			PSS_GTPCacheCreate(){}
			
			PSS_GTPCacheCreate(const SGTopicBaseInfo& tempTopic):
				m_objTopic(tempTopic)
			{
			}
			
			virtual void marshal(sox::Pack &p) const
			{
				m_objTopic.marshal(p);
			}
			
			virtual void unmarshal(const sox::Unpack &p)
			{
				m_objTopic.unmarshal(p);
			}

		};

		// gtopic to gtopiccache, notify which topic is deleted.
		struct PSS_GTPCacheDelete:public sox::Marshallable
		{
			enum {uri = (3 << 8 | protocol::GTOPICCACHE_SVID)};

			uint32_t m_uGrpID;
			uint32_t m_uFldrID;
			std::vector<uint32_t> m_vecTopicID;

			PSS_GTPCacheDelete()
			{
				m_uGrpID = 0;
				m_uFldrID = 0;
			}
			
			PSS_GTPCacheDelete(const uint32_t & uGrpID, 
				const uint32_t & uFldrID, 
				const std::vector<uint32_t> & vecTopicID):
				m_uGrpID(uGrpID),
				m_uFldrID(uFldrID),
				m_vecTopicID(vecTopicID)
			{
			}
			
			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGrpID
				<< m_uFldrID;
				marshal_container(p, m_vecTopicID);
			}
			
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGrpID
				>> m_uFldrID;
				unmarshal_container(p, std::back_inserter(m_vecTopicID));
			}

		};

		// gtopiccache to gtopic, get the topic base info which is not existed in cache.
		struct PSS_GTPCacheGetBaseInfosReq:public sox::Marshallable
		{
			enum {uri = (4 << 8 | protocol::GTOPICCACHE_SVID)};
			
			uint32_t m_uGrpID;
			uint32_t m_uFldrID;
			bool m_bIsSimple;
			std::vector<uint32_t> m_vecTopicIDs;

			PSS_GTPCacheGetBaseInfosReq()
			{
				m_uGrpID = 0;
				m_uFldrID = 0;
				m_bIsSimple = true;
			}
			
			PSS_GTPCacheGetBaseInfosReq(
				const uint32_t & uGrpID, 
				const uint32_t & uFldrID,
				const bool & bIsSimple,
				const std::vector<uint32_t>& vecTids):
				m_uGrpID(uGrpID),
				m_uFldrID(uFldrID),
				m_bIsSimple(bIsSimple),
				m_vecTopicIDs(vecTids)
			{
			}
			
			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGrpID
				<< m_uFldrID
				<< m_bIsSimple;
				marshal_container(p, m_vecTopicIDs);
			}
			
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGrpID
				>> m_uFldrID
				>> m_bIsSimple;
				unmarshal_container(p, std::back_inserter(m_vecTopicIDs));
			}
			
		};

		// gtopic to gtopiccache response to PSS_GTPCacheGetBaseInfosReq
		struct PSS_GTPCacheGetBaseInfosRes:public sox::Marshallable
		{
			enum {uri = (4 << 8 | protocol::GTOPICCACHE_SVID)};

			uint32_t m_uGrpID;
			uint32_t m_uFldrID;
			bool m_bIsSimple;
			std::vector<SGTopicBaseInfo> m_vecTopicInfos;
			std::vector<uint32_t> m_vecNotExistTids;
			
			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGrpID
				<< m_uFldrID
				<< m_bIsSimple;
				marshal_container(p, m_vecTopicInfos);
				marshal_container(p, m_vecNotExistTids);
			}
			
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGrpID
				>> m_uFldrID
				>> m_bIsSimple;;
				unmarshal_container(p, std::back_inserter(m_vecTopicInfos));
				unmarshal_container(p, std::back_inserter(m_vecNotExistTids));
			}
		};

		struct PSS_GTPCacheGetPinnedIDsReq: public sox::Marshallable{
			enum {uri = (5 << 8 | protocol::GTOPICCACHE_SVID)};

			uint32_t m_uGrpID;
			std::vector<uint32_t> m_vecFldrIDs;
			
			PSS_GTPCacheGetPinnedIDsReq(){}
			
			PSS_GTPCacheGetPinnedIDsReq(uint32_t gid, std::vector<uint32_t>& fids ):
				m_uGrpID(gid),
				m_vecFldrIDs(fids)
			{
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGrpID;
				marshal_container(p, m_vecFldrIDs);
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGrpID;
				unmarshal_container(p, std::back_inserter(m_vecFldrIDs));
			}
		};

		struct PSS_GTPCacheGetPinnedIDsRes: public sox::Marshallable{
			enum {uri = (5 << 8 | protocol::GTOPICCACHE_SVID)};

			uint32_t m_uGrpID;		
			std::map<uint32_t, std::vector<uint32_t> > m_mapFID2TIDs;

			PSS_GTPCacheGetPinnedIDsRes(){}

			PSS_GTPCacheGetPinnedIDsRes(uint32_t gid, std::map<uint32_t, std::vector<uint32_t> > mapFID2TIDs):
				m_uGrpID(gid),
				m_mapFID2TIDs(mapFID2TIDs)
			{
			}
				
			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGrpID;	
				marshal_container(p, m_mapFID2TIDs);
			}
			
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGrpID;
				unmarshal_container(p, std::inserter(m_mapFID2TIDs,m_mapFID2TIDs.begin()));
			}
		};
		struct PSS_GTPCacheDelGroupOrFolder : public sox::Marshallable
		{
			enum {uri = (6 << 8 | protocol::GTOPICCACHE_SVID)};
			
			uint32_t m_uGrpID; 
			// if del group, contain group ids and folder ids in group
			// if del folder, contain folder id
			std::set<uint32_t> m_setFldID; 

			PSS_GTPCacheDelGroupOrFolder():
				m_uGrpID(0){}

			PSS_GTPCacheDelGroupOrFolder(uint32_t uGrpID, std::set<uint32_t>& setFldID):
				m_uGrpID(uGrpID),
				m_setFldID(setFldID)
			{
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGrpID;
				sox::marshal_container(p, m_setFldID);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGrpID;
				sox::unmarshal_container(p, std::inserter(m_setFldID, m_setFldID.begin()));
			}
		};
	}
}
