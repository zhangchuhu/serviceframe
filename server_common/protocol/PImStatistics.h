#pragma once 

#include "common/packet.h"
#include "common/core/base_svid.h"
#include <vector>


namespace server
{
	namespace imstatistics
	{

		//É¾³ýÈº
		struct SStatData : public sox::Marshallable
		{
			uint32_t m_uStatId;
			uint32_t m_uCount;

			SStatData() {};
			SStatData(uint32_t id, uint32_t count) : 
						m_uStatId(id), m_uCount(count){}
			
			virtual void marshal(sox::Pack &p) const
			{
				p << m_uStatId << m_uCount;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uStatId >> m_uCount;
			}
		};

		enum EStatOperType
		{
			STATBYTIME = 1,
			STATBYDAY,
			STATWITHNUM
		};
		
		struct PSS_StatNotify : public sox::Marshallable
		{
			enum {uri = (1 << 8 | protocol::IMSTAT_SVID)};

			uint32_t m_uOperType;    //StatOperType
			std::vector<SStatData> m_vecStatData;

			PSS_StatNotify() {};
			PSS_StatNotify(uint32_t uOper, const std::vector<SStatData>& vec) : 
						m_uOperType(uOper)
			{
				m_vecStatData = vec;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uOperType;
				marshal_container(p, m_vecStatData);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uOperType;
				unmarshal_containerEx(p, m_vecStatData);
			}
		};

		struct PSS_StatUidsNotify : public sox::Marshallable
		{
			enum {uri = (2 << 8 | protocol::IMSTAT_SVID)};

			std::vector<uint32_t> m_vecUids;

			PSS_StatUidsNotify() {};
			PSS_StatUidsNotify(const std::vector<uint32_t>& vec)
			{
				m_vecUids = vec;
			}

			virtual void marshal(sox::Pack &p) const
			{
				marshal_container(p, m_vecUids);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				unmarshal_container(p, std::back_inserter(m_vecUids));
			}
		};

		struct PSS_StatFlowNotify : public sox::Marshallable
		{
			enum {uri = (3 << 8 | protocol::IMSTAT_SVID)};
			//key:flow type, value:flow count
			std::map<uint32_t, uint64_t> m_mapFlow;
			
			PSS_StatFlowNotify()
			{
				m_mapFlow.clear();
			};

			virtual void marshal(sox::Pack &p) const
			{
				sox::marshal_container(p, m_mapFlow);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				sox::unmarshal_container(p, std::inserter(m_mapFlow, m_mapFlow.begin()));
			}
		};

		struct PSS_StatGameBuddyConvert : public sox::Marshallable
		{
			enum {uri = (4 << 8 | protocol::IMSTAT_SVID)};

			uint32_t m_uType;	//0-invite, 1-answer
			uint32_t m_uUidInvite;
			uint32_t m_uUidAnswer;

			PSS_StatGameBuddyConvert() {};
			PSS_StatGameBuddyConvert(uint32_t uType, uint32_t uUidInvite, uint32_t uUidAnswer):
				m_uType(uType), m_uUidInvite(uUidInvite), m_uUidAnswer(uUidAnswer)
			{};

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uType << m_uUidInvite << m_uUidAnswer;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uType >> m_uUidInvite >> m_uUidAnswer;
			}
		};

		struct PSS_GBroadFailure : public sox::Marshallable
		{
			enum {uri = (5 << 8 | protocol::IMSTAT_SVID)};

			uint32_t m_uGid;
			uint32_t m_uFid;
			uint64_t m_uTimeStamp;
			std::map<uint32_t, uint32_t> m_mapUidImlinkd;

			PSS_GBroadFailure() {};
			PSS_GBroadFailure(uint32_t gid, uint32_t fid, uint64_t timestamp, std::map<uint32_t, uint32_t>& map):
				m_uGid(gid), m_uFid(fid)
			{
				m_uTimeStamp = timestamp;
				m_mapUidImlinkd = map;
			};

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGid << m_uFid << m_uTimeStamp;
				sox::marshal_container(p, m_mapUidImlinkd);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGid >> m_uFid >> m_uTimeStamp;
				sox::unmarshal_container(p, std::inserter(m_mapUidImlinkd, m_mapUidImlinkd.begin()));
			}
		};

		struct CImBuddyRecordData: public sox::Marshallable
		{
			uint32_t m_uId;
			uint32_t m_uBid;
			uint32_t m_uActionType;	//1. Ìí¼Ó   2.É¾³ý

			CImBuddyRecordData(uint32_t uid=0, uint32_t ubid=0, uint32_t uActionType=1):m_uId(uid),m_uBid(ubid),m_uActionType(uActionType)
			{

			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uId << m_uBid << m_uActionType;
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uId >> m_uBid >> m_uActionType;
			}
/*
			sox::Pack& operator<< (sox::Pack &p)
			{
				marshal(p);
				return p;
			}

			const sox::Unpack& operator >> (const sox::Unpack& s)
			{
				unmarshal(s);
				return s;
			}
			*/
		};

		struct PSS_ImBuddyRecord : public sox::Marshallable
		{
			enum {uri = (6 << 8 | protocol::IMSTAT_SVID)};

			std::vector<CImBuddyRecordData> m_vtImBuddyRecordData;

			virtual void marshal(sox::Pack &p) const
			{
				sox::marshal_container(p, m_vtImBuddyRecordData);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				sox::unmarshal_containerEx(p, m_vtImBuddyRecordData);
			}
		};
	}
}

