#pragma once

#include "common/core/base_svid.h"
#include "packet.h"
#include <string>

namespace server
{
	namespace imrouter
	{
		struct SLinkData: public sox::Marshallable
		{
			uint16_t m_uRtt;

			SLinkData():m_uRtt(-1){}

			// Send an SLinkData object to an ostream...
			friend std::ostream& operator<<(std::ostream& out, const SLinkData& linkData)
			{	
				out << linkData.m_uRtt;
				return(out);
			}

			virtual void marshal(sox::Pack &p) const
			{
				p.push_uint16(m_uRtt);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				m_uRtt = p.pop_uint16();
			}
		};

		struct PSS_RouteProbeReq: public sox::Marshallable
		{
			enum {uri = (1 << 8) | protocol::IMROUTER_SVID};

			uint64_t m_seqId; 	  // 发送序列号 同时也是发送时间

			PSS_RouteProbeReq(): m_seqId(-1){};

			PSS_RouteProbeReq(uint64_t seqId): m_seqId(seqId){};

			virtual void marshal(sox::Pack &p) const
			{
				p << m_seqId;
			}
		
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_seqId;
			}
		};

		struct PSS_RouteProbeRes: public sox::Marshallable
		{
			enum {uri = (2 << 8) | protocol::IMROUTER_SVID};

			uint32_t m_destGid;	  // 接收方的gid
			uint64_t m_seqId; 	  // 发送序列号 由发送方填写

			// 判断当前机房是否拥有imbridge
			// 如果是，在回应PSS_RouteProbeReq时
			// 需要带上该imbridge能够直连的机房
			bool m_hasImbridge;		
			std::map<uint32_t, SLinkData> m_mapGidLinkdata;

			// 版本号
			// - 0: 默认值, 只支持UDP探测
			// - 1: 增加对TCP探测的支持
			uint32_t m_version;	 

			PSS_RouteProbeRes():m_destGid(-1), m_seqId(-1), m_hasImbridge(false), m_version(0){}

			PSS_RouteProbeRes(const PSS_RouteProbeReq& obj, uint32_t groupId)
				: m_destGid(groupId), m_seqId(obj.m_seqId), m_hasImbridge(false), m_version(0)
			{}

			PSS_RouteProbeRes(const PSS_RouteProbeReq& obj, uint32_t groupId, const std::map<uint32_t, SLinkData>& mapGidLinkdata)
				: m_destGid(groupId), m_seqId(obj.m_seqId), m_hasImbridge(true), 
				  m_mapGidLinkdata(mapGidLinkdata), m_version(0)
			{}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_destGid << m_seqId;
				p.push_uint8(m_hasImbridge);
				sox::marshal_container(p, m_mapGidLinkdata);
				p << m_version;
			}
		
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_destGid >> m_seqId;
				m_hasImbridge = p.pop_uint8() != 0;
				sox::unmarshal_container(p, std::inserter(m_mapGidLinkdata,
					m_mapGidLinkdata.begin()));
							
				if (p.size() != 0){
					try{
						p >> m_version;
					}catch(...){
						m_version = 0;
					}
				}else{
					m_version = 0;
				}
			}
		};

		struct PSS_RouteNotifyServer: public sox::Marshallable
		{
			enum {uri = (3 << 8) | protocol::IMROUTER_SVID};

			// value: gid of destination
			// serverId(0 if direct link with dest, imbridge id if can't link directly but can path via imbridge, -1 if none connects)
			// -2 if server removed
			std::map<uint32_t, uint32_t> m_mapGidServerIds;

			PSS_RouteNotifyServer(){};

			PSS_RouteNotifyServer(const std::map<uint32_t, uint32_t>& mapGidServerIds)
				: m_mapGidServerIds(mapGidServerIds){};

			virtual void marshal(sox::Pack &p) const
			{
				sox::marshal_container(p, m_mapGidServerIds);
			}
		
			virtual void unmarshal(const sox::Unpack &p)
			{
				sox::unmarshal_container(p, std::inserter(m_mapGidServerIds,
					m_mapGidServerIds.begin()));
			}
		};

		struct PSS_RouteTcpProbeReq: public sox::Marshallable
		{
			enum {uri = (4 << 8) | protocol::IMROUTER_SVID};

			uint64_t m_seqId; 	  // 发送序列号 同时也是发送时间

			PSS_RouteTcpProbeReq(): m_seqId(-1){};

			PSS_RouteTcpProbeReq(uint64_t seqId): m_seqId(seqId){};

			virtual void marshal(sox::Pack &p) const
			{
				p << m_seqId;
			}
		
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_seqId;
			}
		};

		struct PSS_RouteTcpProbeRes: public sox::Marshallable
		{
			enum {uri = (5 << 8) | protocol::IMROUTER_SVID};

			uint64_t m_seqId; 	  // 发送序列号 同时也是发送时间
			uint32_t m_groupId;	  // 

			PSS_RouteTcpProbeRes(): m_seqId(-1){};

			PSS_RouteTcpProbeRes(const PSS_RouteTcpProbeReq& objReq, uint32_t groupId)
				: m_seqId(objReq.m_seqId), m_groupId(groupId){};
			
			virtual void marshal(sox::Pack &p) const
			{
				p << m_seqId << m_groupId;
			}
		
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_seqId >> m_groupId;
			}
		};

		struct PSS_RouteExchangeData: public sox::Marshallable
		{
			enum {uri = (6 << 8) | protocol::IMROUTER_SVID};
	
			std::map<uint32_t, SLinkData> mapGidLinkdata;

			PSS_RouteExchangeData(){}

			PSS_RouteExchangeData(const std::map<uint32_t, SLinkData>& linkData)
				: mapGidLinkdata(linkData)
			{}

			virtual void marshal(sox::Pack &p) const
			{
				sox::marshal_container(p, mapGidLinkdata);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				sox::unmarshal_container(p, std::inserter(mapGidLinkdata, mapGidLinkdata.begin()));
			}
		};
	}
}

