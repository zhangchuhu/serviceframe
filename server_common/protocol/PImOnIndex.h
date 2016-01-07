#ifndef PROTOCOL_IM_ONLINE_INDEX_INCLUDE
#define PROTOCOL_IM_ONLINE_INDEX_INCLUDE

#include "int_types.h"
#include "core/base_svid.h"
#include "protocol/const.h"
#include "packet.h"
#include "PImOnlineNew.h"

#include <sys/time.h>
#include <vector>

namespace server
{
	namespace imonindex
	{
		// imonline 向 onlineindex通知用户所在imlinkd改变

		enum EUserLogLinkd
		{
			ENUM_LOGIN = 0,
			ENUM_LOGOUT
		};

		struct CLinkdUidOp : public sox::Marshallable
		{
			uint32_t m_uUid;
			uint32_t m_uLinkdId;
			EUserLogLinkd m_op;

			CLinkdUidOp() {}
			CLinkdUidOp(uint32_t uid, uint32_t linkd, EUserLogLinkd op) : m_uUid(uid), m_uLinkdId(linkd), m_op(op) {}

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uUid << m_uLinkdId;
				p.push_uint8(m_op);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uUid >> m_uLinkdId;
				m_op = (EUserLogLinkd)p.pop_uint8();
			}
		};

		struct PSS_NotifyUserLinkdChange : public sox::Marshallable
		{
			enum {uri = (1 << 8) | protocol::IMONINDEX_SVID};

			EImFrontEndNo m_emFrontNo;
			std::vector<CLinkdUidOp> m_vecLinkUser;

			PSS_NotifyUserLinkdChange(EImFrontEndNo emFrontNo = ENUM_FRONT_TCP) : m_emFrontNo(emFrontNo) {}
			PSS_NotifyUserLinkdChange(EImFrontEndNo emFrontNo, const server::imonline::CUserOnlineInfoNew& tmp)
				: m_emFrontNo(emFrontNo)
			{
				SetUserInfo(tmp);
			}
			PSS_NotifyUserLinkdChange(EImFrontEndNo emFrontNo, const std::vector<CLinkdUidOp>& vec)
				: m_emFrontNo(emFrontNo), m_vecLinkUser(vec) 
			{

			}

			void SetUserInfo(const server::imonline::CUserOnlineInfoNew& tmp)
			{
				if (tmp.IsNotOffline())
					m_vecLinkUser.push_back(CLinkdUidOp(tmp.m_uUid, tmp.m_uLinkdId, ENUM_LOGIN));
				else
					m_vecLinkUser.push_back(CLinkdUidOp(tmp.m_uUid, tmp.m_uLinkdId, ENUM_LOGOUT));
			}

			// imlinkd不相同，先通知imonindex在旧imlinkd下线
			void CheckAndSetUserInfo(const server::imonline::CUserOnlineInfoNew& stNew, const server::imonline::CUserOnlineInfoNew& stOld)
			{
				if (stOld.Valid() && !stNew.SameLinkd(stOld.m_uLinkdId))
				{
					m_vecLinkUser.push_back(CLinkdUidOp(stOld.m_uUid, stOld.m_uLinkdId, ENUM_LOGOUT));
				}
				m_vecLinkUser.push_back(CLinkdUidOp(stNew.m_uUid, stNew.m_uLinkdId, ENUM_LOGIN));
			}

			virtual void marshal(sox::Pack &p) const 
			{
				p.push_uint8(m_emFrontNo);
				sox::marshal_container(p, m_vecLinkUser);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				m_emFrontNo = (EImFrontEndNo)p.pop_uint8();
				sox::unmarshal_container(p, std::back_inserter(m_vecLinkUser));
			}
		};

	
		// 组内第一个imonline启动时，通知onlineindex 刷新列表

		struct PSS_NotifyFlushLinkdUser : public sox::Voidmable
		{
			enum {uri = (3 << 8) | protocol::IMONINDEX_SVID};
		};

		// 后门

		struct PSS_GetUserByLinkd : public sox::Marshallable
		{
			enum {uri = (4 << 8) | protocol::IMONINDEX_SVID};

			std::vector<uint32_t> m_vecLinkd;

			PSS_GetUserByLinkd() {}
			PSS_GetUserByLinkd(const std::vector<uint32_t>& vecLInkd) : m_vecLinkd(vecLInkd) {}

			virtual void marshal(sox::Pack &p) const 
			{
				sox::marshal_container(p, m_vecLinkd);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				sox::unmarshal_container(p, std::back_inserter(m_vecLinkd));
			}
		};

		struct PSS_GetUserByLinkdRes : public sox::Marshallable
		{
			enum {uri = (5 << 8) | protocol::IMONINDEX_SVID};

			std::map<uint32_t, std::set<uint32_t> >  m_mapLinkd2Uids;
			
			virtual void marshal(sox::Pack &p) const 
			{
				sox::marshal_container(p, m_mapLinkd2Uids);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				sox::unmarshal_container(p, std::inserter(m_mapLinkd2Uids, m_mapLinkd2Uids.begin()));
			}
		};


		// 批量操作，以imlinkd 为单位

		struct PSS_NotifyUserLinkdChangeByLinkd : public sox::Marshallable
		{
			enum {uri = (6 << 8) | protocol::IMONINDEX_SVID};

			PSS_NotifyUserLinkdChangeByLinkd() {}
			PSS_NotifyUserLinkdChangeByLinkd(EImFrontEndNo emFront, protocol::E_IMSTATUS emStatus, uint32_t uLinkd)
				: m_emFront(emFront), m_emStatus(emStatus), m_uLinkdId(uLinkd)
			{
			}
			PSS_NotifyUserLinkdChangeByLinkd(EImFrontEndNo emFront, protocol::E_IMSTATUS emStatus, uint32_t uLinkd, const std::vector<uint32_t>& vecUids)
				: m_emFront(emFront), m_emStatus(emStatus), m_uLinkdId(uLinkd), m_vecUids(vecUids)
			{
			}

			EImFrontEndNo         m_emFront;
			protocol::E_IMSTATUS  m_emStatus;
			uint32_t              m_uLinkdId;
			std::vector<uint32_t> m_vecUids;

			virtual void marshal(sox::Pack &p) const 
			{
				p.push_uint8(m_emFront);
				p.push_uint8(m_emStatus);
				p << m_uLinkdId;
				sox::marshal_container(p, m_vecUids);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				m_emFront = (EImFrontEndNo)p.pop_uint8();
				m_emStatus = (protocol::E_IMSTATUS)p.pop_uint8();
				p >> m_uLinkdId;
				sox::unmarshal_container(p, std::back_inserter(m_vecUids));
			}			
		};

	}
}

#endif
