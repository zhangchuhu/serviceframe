#ifndef _PIMONLINEDB_H___
#define _PIMONLINEDB_H___

#include "int_types.h"
#include "core/base_svid.h"
#include "protocol/const.h"
#include "packet.h"

#include <vector>

namespace server{
	namespace imonline{

		struct SUserStatus : public sox::Marshallable{
			uint32_t uid;
			protocol::E_IMSTATUS stat;

			virtual void marshal(sox::Pack &p) const {
				p << uid;
				p.push_uint8(stat);
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> uid;
				stat = (protocol::E_IMSTATUS)p.pop_uint8();
			}
		};

		struct PUserIMStatChanged : public sox::Marshallable{
			enum {uri = (1 << 8 | protocol::IMONLINED_SVID)};

			PUserIMStatChanged(){}
			PUserIMStatChanged(uint32_t id, uint32_t linkid, protocol::E_IMSTATUS st) 
				: uid(id), linkd_id(linkid), stat(st) {}

				uint32_t uid;
				uint32_t linkd_id;
				protocol::E_IMSTATUS stat;

				virtual void marshal(sox::Pack &p) const{
					p << uid << linkd_id;
					p.push_uint8(stat);
				}
				virtual void unmarshal(const sox::Unpack &p){
					p >> uid >> linkd_id;
					stat = (protocol::E_IMSTATUS)p.pop_uint8();
				}
		};

		//struct PSS_ChangeStatusRes : public sox::Marshallable {
		//	enum {uri = (3 << 8) | protocol::IMONLINED_SVID};

		//	uint32_t uid;
		//	protocol::E_IMSTATUS status;

		//	virtual void marshal(sox::Pack &pk) const {
		//		pk << uid;
		//		pk.push_uint16(status);
		//	}
		//	virtual void unmarshal(const sox::Unpack &up) {
		//		up >> uid;
		//		status = (protocol::E_IMSTATUS)up.pop_uint16();
		//	}
		//};

		//IMLINK_D to IMONLINEDB
		struct PRecoverUserStatus : public sox::Marshallable{
			enum {uri = (2 << 8) | protocol::IMONLINED_SVID};
			PRecoverUserStatus(){}

			uint32_t linkd_id;
			std::vector<SUserStatus> buddies_stat;

			virtual void marshal(sox::Pack &p) const {
				p << linkd_id;
				marshal_container(p, buddies_stat);
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> linkd_id;
				sox::unmarshal_container(p, std::back_inserter(buddies_stat));
			}
		};

		//用户登陆通知,专用于通知IMOnline(旧协议，兼容旧的protocol)
		struct PSS_LoginNotifyIMOnline : public sox::Marshallable
		{
			enum {uri = (4 << 8 | protocol::IMONLINED_SVID)};

			PSS_LoginNotifyIMOnline(){}
			PSS_LoginNotifyIMOnline(uint32_t id, uint32_t linkid, protocol::E_IMSTATUS estat) : uid(id), linkd_id(linkid), stat(estat)
			{
			}

			uint32_t uid;
			uint32_t linkd_id;
			protocol::E_IMSTATUS stat;

			virtual void marshal(sox::Pack &p) const
			{
				p << uid << linkd_id;
				p.push_uint8(stat);
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> uid >> linkd_id;
				stat = (protocol::E_IMSTATUS)p.pop_uint8();
			}
		};

		//用户登陆通知消息响应
		struct PSS_LoginNotifyIMOnlineRes : public sox::Marshallable
		{
			enum {uri = (5 << 8 | protocol::IMONLINED_SVID)};

			PSS_LoginNotifyIMOnlineRes(){}
			PSS_LoginNotifyIMOnlineRes(uint32_t id) : uid(id)
			{
			}

			uint32_t uid;
			virtual void marshal(sox::Pack &p) const
			{
				p << uid;
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> uid ;
			}
		};

		//IMLINK_D to IMONLINEDB
		//该版本用于返回UID索引
		struct PRecoverUserList : public sox::Marshallable
		{
			enum {uri = (6 << 8) | protocol::IMONLINED_SVID};
			PRecoverUserList(){}

			uint32_t uImLinkdId;
			std::vector<uint32_t> vecUid;

			virtual void marshal(sox::Pack &p) const 
			{
				p << uImLinkdId;
				marshal_container(p, vecUid);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> uImLinkdId;
				sox::unmarshal_container(p, std::back_inserter(vecUid));
			}
		};

		//用户登陆通知,专用于通知IMOnline(新协议)
		struct PSS_LoginNotifyIMOnline2 : public PSS_LoginNotifyIMOnline
		{
			enum {uri = (7 << 8 | protocol::IMONLINE2_SVID)};

			PSS_LoginNotifyIMOnline2(){}
			PSS_LoginNotifyIMOnline2(uint32_t id, uint32_t linkid, protocol::E_IMSTATUS estat) : PSS_LoginNotifyIMOnline(id, linkid,estat)
			{
			}
		};

		// imonlined to imonlicne_cached
		struct PSS_DeleteByImlinkd : public sox::Marshallable
		{
			enum {uri = (138 << 8) | protocol::IMONLINED_SVID};

			uint32_t m_imlinkd;
			std::vector<SUserStatus> m_statusInfo;

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_imlinkd;
				marshal_container(p, m_statusInfo);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_imlinkd;
				sox::unmarshal_container(p, std::back_inserter(m_statusInfo));
			}
		};

		//---------------------------------------------------------------------------
		//批量通知状态 imonline2 -> imonline_cache

		struct SUserIMStatChange : public sox::Marshallable
		{
			uint32_t m_Uid;
			uint32_t m_ImLinkdId;
			protocol::E_IMSTATUS m_emState;

			SUserIMStatChange() {};
			SUserIMStatChange(uint32_t Uid, uint32_t ImLinkdId, protocol::E_IMSTATUS emState)
			{
				m_Uid = Uid;
				m_ImLinkdId = ImLinkdId;
				m_emState = emState;
			}

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_Uid << m_ImLinkdId;
				p.push_uint8(m_emState);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_Uid >> m_ImLinkdId;
				m_emState = (protocol::E_IMSTATUS)p.pop_uint8();
			}
		};

		struct PSS_BatchUserIMStatChange : public sox::Marshallable
		{
			enum {uri = (10 << 8) | protocol::IMONLINE2_SVID};

			uint32_t m_ImOcId;
			uint32_t m_PacketId;
			std::vector<SUserIMStatChange> m_vecUserIMStat;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_ImOcId << m_PacketId;
				sox::marshal_container(p, m_vecUserIMStat);
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_ImOcId >> m_PacketId;
				sox::unmarshal_container(p, std::back_inserter(m_vecUserIMStat));
			}
		};

		struct PSS_BatchUserIMStatChangeRes : public sox::Marshallable
		{
			enum {uri = (11 << 8) | protocol::IMONLINE2_SVID};

			PSS_BatchUserIMStatChangeRes() {};
			PSS_BatchUserIMStatChangeRes(uint32_t ImOcId, uint32_t PacketId)
			{
				m_ImOcId = ImOcId;
				m_PacketId = PacketId;
			}

			uint32_t m_ImOcId;
			uint32_t m_PacketId;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_ImOcId << m_PacketId;
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_ImOcId >> m_PacketId;
			}
		};

		//---------------------------------------------------------------------

		//add by kdjie, 2010.3.19

		//用于imchat2 -> imonline2 修复该用户状态
		struct PSS_RepairImStats : public sox::Marshallable
		{
			enum {uri = (14 << 8) | protocol::IMONLINE2_SVID};

			PSS_RepairImStats() {};
			PSS_RepairImStats(uint32_t uUid, uint32_t uImLinkdId, protocol::E_IMSTATUS emStats)
			{
				m_uUid = uUid;
				m_uImLinkdId = uImLinkdId;
				m_emStats = emStats;
			}

			uint32_t m_uUid;
			uint32_t m_uImLinkdId;
			protocol::E_IMSTATUS m_emStats;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid << m_uImLinkdId;
				p.push_uint8(m_emStats);
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid >> m_uImLinkdId;
				m_emStats = (protocol::E_IMSTATUS)p.pop_uint8();
			}
		};

		//add end
	}
}


#endif
