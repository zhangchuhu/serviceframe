#ifndef PROTOCOL_IM_ONLINEV2_H_INCLUDE
#define PROTOCOL_IM_ONLINEV2_H_INCLUDE

#include "int_types.h"
#include "core/base_svid.h"
#include "protocol/const.h"
#include "packet.h"

#include <sys/time.h>
#include <vector>
#include "common/protocol/Im_Const.h"

namespace server
{
	namespace imonlinev2
	{
		// 用户在线信息
		struct CImUserStatusCommonV2 : public sox::Marshallable
		{
			CImUserStatusCommonV2 () 
				: m_uUid(0), m_uLinkdId(NONE_SERVER_ID), m_emFront(ENUM_FRONT_TCP), m_emStatus(protocol::IMOFFLINE)				
			{
			}
			CImUserStatusCommonV2 (uint32_t id, uint32_t linkid, EImFrontEndNo emFront, protocol::E_IMSTATUS st) 
				: m_uUid(id), m_uLinkdId(linkid), m_emFront(emFront), m_emStatus(st) 
			{
			}

			uint32_t m_uUid;
			uint32_t m_uLinkdId;

			EImFrontEndNo        m_emFront;
			protocol::E_IMSTATUS m_emStatus;
			

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid << m_uLinkdId;
				p.push_uint32(m_emFront);
				p.push_uint8(m_emStatus);
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid >> m_uLinkdId;
				m_emFront  = (EImFrontEndNo) p.pop_uint32();
				m_emStatus = (protocol::E_IMSTATUS)p.pop_uint8();
			}
		};

		// 登陆请求

		struct PSS_LoginReqV2 : public CImUserStatusCommonV2
		{
			enum {uri = (1 << 8 | protocol::IMONLINEV2_SVID)};

			PSS_LoginReqV2() 
			{
				m_uVersion = 0;
				m_uTimeStamp = 0;
			}
			PSS_LoginReqV2(uint32_t uUid, uint32_t uLinkd, uint32_t uVersion, protocol::E_IMSTATUS st, EImFrontEndNo emFront, uint64_t t)
				: CImUserStatusCommonV2(uUid, uLinkd, emFront, st), m_uVersion(uVersion), m_uTimeStamp(t)
			{
			}

			uint32_t m_uVersion;
			uint64_t m_uTimeStamp;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uVersion << m_uTimeStamp;
				CImUserStatusCommonV2::marshal(p);
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uVersion >> m_uTimeStamp;
				CImUserStatusCommonV2::unmarshal(p);
			}
		};

		struct PSS_LoginReqResV2 : public sox::Marshallable
		{
			enum {uri = (2 << 8 | protocol::IMONLINEV2_SVID)};

			PSS_LoginReqResV2()	{}
			PSS_LoginReqResV2(uint32_t uid, uint64_t uTime, EImFrontEndNo emFront, protocol::E_IMSTATUS status) 
				: m_uUid(uid), m_uTimeStamp(uTime), m_emFront(emFront), m_emStatus(status)
			{

			}

			uint32_t  m_uUid;
			uint64_t  m_uTimeStamp;       

			EImFrontEndNo        m_emFront;
			protocol::E_IMSTATUS m_emStatus;

			virtual void marshal(sox::Pack &pk) const 
			{
				pk << m_uUid << m_uTimeStamp;
				pk.push_uint32(m_emFront);
				pk.push_uint8(m_emStatus);
			}
			virtual void unmarshal(const sox::Unpack &up) 
			{
				up >> m_uUid >> m_uTimeStamp;
				m_emFront  = (EImFrontEndNo)up.pop_uint32();
				m_emStatus = (protocol::E_IMSTATUS)up.pop_uint8();
			}
		};



		// 请求改变在线状态

		struct PSS_ChangeStatusReqV2 : public PSS_LoginReqV2
		{
			enum {uri = (3 << 8 | protocol::IMONLINEV2_SVID)};

			PSS_ChangeStatusReqV2() {}
			PSS_ChangeStatusReqV2(uint32_t id, uint32_t linkid, uint32_t uVersion, EImFrontEndNo emFront, protocol::E_IMSTATUS st, uint64_t uTime) 
				: PSS_LoginReqV2(id, linkid, uVersion, st, emFront, uTime)
			{
			}
		};

		// 用户改变在线状态请求的响应

		struct PSS_ChangeStatusReqResV2 : public PSS_LoginReqResV2
		{
			enum {uri = (4 << 8) | protocol::IMONLINEV2_SVID};

			PSS_ChangeStatusReqResV2()	{}
			PSS_ChangeStatusReqResV2(uint32_t uid, uint64_t uTime, EImFrontEndNo emFront, protocol::E_IMSTATUS status) 
				: PSS_LoginReqResV2(uid, uTime, emFront, status)
			{
			}
		};

		// imonline 启动时，recover

		struct PSS_UserStatusRecoverV2 : public sox::Marshallable 
		{
			enum {uri = (5 << 8) | protocol::IMONLINEV2_SVID};
			
			PSS_UserStatusRecoverV2() : m_emFront(ENUM_FRONT_TCP)
			{
			}
			PSS_UserStatusRecoverV2(const EImFrontEndNo emFront) : m_emFront(emFront)
			{
			}
			
			EImFrontEndNo m_emFront;

			virtual void marshal(sox::Pack &pk) const 
			{
				pk.push_uint32(m_emFront);
			}
			virtual void unmarshal(const sox::Unpack &up) 
			{
				m_emFront  = (EImFrontEndNo)up.pop_uint32();
			}
		};

		// imlinkd 返回在自己登陆的用户的在线状态

		struct CImUserStatusV2 : public sox::Marshallable
		{	
			CImUserStatusV2() : m_uVersion(0), m_emStatus(protocol::IMONLINE)
			{
			}
			CImUserStatusV2(uint32_t uVersion, protocol::E_IMSTATUS emSt)
				: m_uVersion(uVersion), m_emStatus(emSt)
			{
			}

			uint32_t m_uVersion;
			protocol::E_IMSTATUS m_emStatus;

			virtual void marshal(sox::Pack &pk) const 
			{
				pk << m_uVersion;
				pk.push_uint8(m_emStatus);
			}
			virtual void unmarshal(const sox::Unpack &pk) 
			{
				pk >> m_uVersion;
				m_emStatus = (protocol::E_IMSTATUS)pk.pop_uint8();
			}
		};		

		struct PSS_UserStatusRecoverResV2 : public sox::Marshallable
		{
			enum {uri = (6 << 8) | protocol::IMONLINEV2_SVID};

			uint32_t  m_uLinkd;
			uint64_t  m_uTimeStamp;

			EImFrontEndNo m_emFront;
			std::map<uint32_t, CImUserStatusV2> m_mapUid2St;

			virtual void marshal(sox::Pack &pk) const 
			{
				pk << m_uLinkd << m_uTimeStamp;
				pk.push_uint32(m_emFront);
				sox::marshal_container(pk, m_mapUid2St);
			}
			virtual void unmarshal(const sox::Unpack &up) 
			{
				up >> m_uLinkd >> m_uTimeStamp;
				m_emFront = (EImFrontEndNo)up.pop_uint32();
				sox::unmarshal_container(up, std::inserter(m_mapUid2St, m_mapUid2St.begin()));
			}
		};

		
		// imlinkd 断线重连，主动推送的数据

		struct PSS_PushUserStatusV2 : public PSS_UserStatusRecoverResV2
		{
			enum {uri = (7 << 8) | protocol::IMONLINEV2_SVID};
		};

		// imonline_proxy 收到后，给imlinkd的回应

		struct PSS_PushUserStatusResV2 : public sox::Marshallable
		{
			enum {uri = (8 << 8) | protocol::IMONLINEV2_SVID};

			PSS_PushUserStatusResV2() {}
			PSS_PushUserStatusResV2(uint32_t uLinkd, uint64_t uTime, EImFrontEndNo emFront)
				: m_uLinkdId(uLinkd), m_uTimeStamp(uTime), m_emFront(emFront)
			{
			}

			uint32_t       m_uLinkdId;
			uint64_t       m_uTimeStamp;
			EImFrontEndNo  m_emFront;

			virtual void marshal(sox::Pack &pk) const 
			{
				pk << m_uLinkdId << m_uTimeStamp;
				pk.push_uint32(m_emFront);
			}
			virtual void unmarshal(const sox::Unpack &up) 
			{
				up >> m_uLinkdId >> m_uTimeStamp;
				m_emFront = (EImFrontEndNo)up.pop_uint32();
			}
		};

		// imonidex请求用户列表
		
		struct PSS_UserIndexRecoverV2 : public sox::Marshallable
		{
			enum {uri = (9 << 8) | protocol::IMONLINEV2_SVID};

			PSS_UserIndexRecoverV2() : m_emFront(ENUM_FRONT_TCP)
			{
			}
		
			PSS_UserIndexRecoverV2(const EImFrontEndNo emFront) : m_emFront(emFront)
			{
			}

			EImFrontEndNo m_emFront;

			virtual void marshal(sox::Pack &pk) const 
			{
				pk.push_uint32(m_emFront);
			}
			virtual void unmarshal(const sox::Unpack &up) 
			{
				m_emFront = (EImFrontEndNo)up.pop_uint32();
			}
		};

		// 响应的用户列表

		struct PSS_UserIndexRecoverResV2 : public sox::Marshallable
		{
			enum {uri = (10 << 8) | protocol::IMONLINEV2_SVID};

			uint32_t              m_uLikdId;
			EImFrontEndNo         m_emFront;
			std::vector<uint32_t> m_vecUids;

			virtual void marshal(sox::Pack &pk) const 
			{
				pk << m_uLikdId;
				pk.push_uint32(m_emFront);
				sox::marshal_container(pk, m_vecUids);
			}
			virtual void unmarshal(const sox::Unpack &up) 
			{
				up >> m_uLikdId;
				m_emFront = (EImFrontEndNo)up.pop_uint32();
				sox::unmarshal_container(up, std::back_inserter(m_vecUids));
			}
		};

		// imlinkd掉线，通知imonline删除该imlinkd上的用户

		struct PSS_NotifyRemoveImlinkdV2 : sox::Marshallable
		{
			enum {uri = (11 << 8) | protocol::IMONLINEV2_SVID};

			PSS_NotifyRemoveImlinkdV2() {}
			PSS_NotifyRemoveImlinkdV2(EImFrontEndNo front, uint32_t linkd, const std::set<uint32_t>& setV)
				: m_emFrontEnd(front), m_uLinkdId(linkd), m_vecUids(setV.begin(), setV.end())
			{
			}

			EImFrontEndNo  m_emFrontEnd;
			uint32_t       m_uLinkdId;

			std::vector<uint32_t> m_vecUids;

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uLinkdId;
				p.push_uint32(m_emFrontEnd);
				sox::marshal_container(p, m_vecUids);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uLinkdId;
				m_emFrontEnd = (EImFrontEndNo)p.pop_uint32();
				sox::unmarshal_container(p, std::back_inserter(m_vecUids));
			}
		};


		// 通知imonindex状态变更

		struct PSS_NotifyUserIndexV2 : public sox::Marshallable
		{
			enum {uri = (12 << 8) | protocol::IMONLINEV2_SVID};

			std::vector<CImUserStatusCommonV2> m_vecUserSt;			

			virtual void marshal(sox::Pack &p) const 
			{
				sox::marshal_container(p, m_vecUserSt);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				sox::unmarshal_container(p, std::back_inserter(m_vecUserSt));
			}
		};

		// 批量通知imonindex用户状态变更

		struct PSS_BatchNotifyUserIndexV2 : public sox::Marshallable
		{
			enum {uri = (13 << 8) | protocol::IMONLINEV2_SVID};

			uint32_t             m_uLinkdId;
			EImFrontEndNo        m_emFront;
			std::map<uint32_t, protocol::E_IMSTATUS> m_mapUid2St;


			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uLinkdId;
				p.push_uint32(m_emFront);

				p.push_uint32(m_mapUid2St.size());
				for (std::map<uint32_t, protocol::E_IMSTATUS>::const_iterator it = m_mapUid2St.begin(); 
						it != m_mapUid2St.end(); ++it)
				{
					p.push_uint32(it->first);
					p.push_uint8(it->second);
				}
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uLinkdId;
				m_emFront  = (EImFrontEndNo)p.pop_uint32();

				for(uint32_t count = p.pop_uint32() ; count >0 ; --count)
				{
					m_mapUid2St[p.pop_uint32()] = (protocol::E_IMSTATUS)p.pop_uint8();
				}
			}
		};

		// 组内第一个imonline启动时，通知imonindex清缓存

		struct PSS_NotifyIndexFlushV2 : sox::Voidmable
		{
			enum {uri = (14 << 8) | protocol::IMONLINEV2_SVID};
		};

		// 超时批量重发

		struct CImUserStatusReSendV2 : public sox::Marshallable
		{	
			CImUserStatusReSendV2() : m_uVersion(0), m_uTimeStamp(0), m_emStatus(protocol::IMONLINE)
			{
			}
			CImUserStatusReSendV2(uint32_t uVersion, uint64_t uTime, protocol::E_IMSTATUS emSt)
				: m_uVersion(uVersion), m_uTimeStamp(uTime), m_emStatus(emSt)
			{
			}

			uint32_t m_uVersion;
			uint64_t m_uTimeStamp;
			protocol::E_IMSTATUS m_emStatus;

			virtual void marshal(sox::Pack &pk) const 
			{
				pk << m_uVersion << m_uTimeStamp;
				pk.push_uint8(m_emStatus);
			}
			virtual void unmarshal(const sox::Unpack &up) 
			{
				up >> m_uVersion >> m_uTimeStamp;
				m_emStatus = (protocol::E_IMSTATUS)up.pop_uint8();
			}
		};		

		struct PSS_ReSendUserStatusV2 : sox::Marshallable
		{
			enum {uri = (15 << 8) | protocol::IMONLINEV2_SVID};

			uint32_t      m_uLinkd;
			uint64_t      m_uTimeStamp;   // 让imonline_proxy 轮询选imonbridge
			EImFrontEndNo m_emFront;

			std::map<uint32_t, CImUserStatusReSendV2> m_mapUid2St;
			//std::vector<CImUserStatusReSendV2> m_vecUserSt;

			virtual void marshal(sox::Pack &pk) const 
			{
				pk << m_uLinkd;
				pk.push_uint32(m_emFront);
				sox::marshal_container(pk, m_mapUid2St);
			}
			virtual void unmarshal(const sox::Unpack &up) 
			{
				up >> m_uLinkd;
				m_emFront = (EImFrontEndNo)up.pop_uint32();
				sox::unmarshal_container(up, std::inserter(m_mapUid2St, m_mapUid2St.begin()));
			}
		};

		// imonline_proxy 超时重发Res

		struct PSS_ReSendUserStatusResV2 : sox::Marshallable
		{
			enum {uri = (16 << 8) | protocol::IMONLINEV2_SVID};

			uint32_t m_uLinkd;
			EImFrontEndNo m_emFront;
			std::map<uint32_t, uint64_t> m_mapUid2Time;

			virtual void marshal(sox::Pack &pk) const 
			{
				pk << m_uLinkd;
				pk.push_uint32(m_emFront);
				sox::marshal_container(pk, m_mapUid2Time);
			}
			virtual void unmarshal(const sox::Unpack &up) 
			{
				up >> m_uLinkd;
				m_emFront = (EImFrontEndNo)up.pop_uint32();
				sox::unmarshal_container(up, std::inserter(m_mapUid2Time, m_mapUid2Time.begin()));
			}
		};
		
	}
}

#endif

