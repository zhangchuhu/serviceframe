#ifndef PSINFO_H_
#define PSINFO_H_
#include "common/iproperty.h"
#include "common/protocol/const.h"
#include "common/protocol/psessionbase.h"
#include "pslist.h"
#include <iterator>
#include <vector>

namespace protocol{
namespace session{
//uri 64 - 95
/*
struct PSyncSInfo: public PSessionSid{
	enum { uri = (64 << 8) | SESSION_SVID };
};
*/
struct PSessionInfo: public sox::Properties{
	
};

struct PUpdateChanelInfo : public PSessionInfo{
	enum { uri = (65 << 8) | SDB_SVID };
	uint32_t sid;
	uint32_t updator;
	virtual void marshal(sox::Pack &p) const{
		p << sid << updator;
		PSessionInfo::marshal(p);
		
	}
	virtual void unmarshal(const sox::Unpack &p){
		p >> sid >> updator;
		PSessionInfo::unmarshal(p);
	}
};

struct POnChanelInfoUpdated: public PUpdateChanelInfo{
	
};


struct PUpdateChanelMember: public sox::Marshallable{
	enum { uri = (66 << 8) | SDB_SVID };

	enum RolerOp{
	    NULOP,
		ADD,
		REMOVE,
		CHANGE
	};

	uint32_t channelId;
	uint32_t uid;
	ChannelRoler roler; // new roler of this member
	RolerOp op;
	uint32_t admin; //server fill
	virtual void marshal(sox::Pack &p) const{
		p  << channelId << uid << admin;
		p.push_uint16(roler).push_uint16(op);

	}
	virtual void unmarshal(const sox::Unpack &p){
		p >> channelId >> uid >> admin;
		roler = (ChannelRoler)p.pop_uint16();
		op = (RolerOp)p.pop_uint16();
	}

};

struct POnMemberUpdated: public PUpdateChanelMember{
	std::string nick;
	protocol::uinfo::SEX sex;

	virtual void marshal(sox::Pack &p) const{
		PUpdateChanelMember::marshal(p);
		p << nick;
		p.push_uint16(sex);
	}
	virtual void unmarshal(const sox::Unpack &p){
		PUpdateChanelMember::unmarshal(p);
		p >> nick;
		sex = (uinfo::SEX)p.pop_uint16();
	}
};

struct POnMemberUpdatedErr: public PUpdateChanelMember{

};

struct PSDBDismissSubChannel: public PSessionSid{
	enum { uri = (67 << 8) | SDB_SVID };
	uint32_t uid;
	uint32_t resCode;
	virtual void marshal(sox::Pack &pk) const{
		PSessionSid::marshal(pk);
		pk << uid << resCode;
	}
	virtual void unmarshal(const sox::Unpack &p){
		PSessionSid::unmarshal(p);
		p >> uid >> resCode;
	};
};


struct PUpdateBanID: public sox::Marshallable{
	enum {uri = (68 << 8) | SDB_SVID};
	uint32_t uid;
	bool isBan;
	virtual void marshal(sox::Pack &p) const{
		p << uid;
		p.push_uint8(isBan);
	}

	virtual void unmarshal(const sox::Unpack &p){
		p >> uid;
		isBan = (p.pop_uint8() != 0);
	}
};

struct PUpdateBanIDRes: public PUpdateBanID{
};

struct PUpdateBanIp: public sox::Marshallable{
	enum {uri = (69 << 8) | SDB_SVID};
	uint32_t uid;
	bool isBan;
	virtual void marshal(sox::Pack &p) const{
		p << uid;
		p.push_uint8(isBan);
	}

	virtual void unmarshal(const sox::Unpack &p){
		p >> uid;
		isBan = (p.pop_uint8() != 0);
	}
}; 

struct PUpdateBanIpRes: public PUpdateBanIp{
	enum {uri = (69 << 8) | SDB_SVID};
	std::string ip;
	bool isBan;
	virtual void marshal(sox::Pack &p) const{
		p << ip;
		p.push_uint8(isBan);
	}

	virtual void unmarshal(const sox::Unpack &p){
		p >> ip;
		isBan = (p.pop_uint8() != 0);
	}
};

struct PGetBans: public sox::Voidmable{
	enum {uri = (70 << 8 | SDB_SVID)};
};

struct POnGetBans: public sox::Voidmable{
	enum {uri = (70 << 8 | SDB_SVID)};
	std::vector<PListMini> ids;
	std::vector<std::string> ips;
	virtual void marshal(sox::Pack &p) const{
		sox::marshal_container(p, ids);
		sox::marshal_container(p, ips);
	}

	virtual void unmarshal(const sox::Unpack &p){
		sox::unmarshal_container(p, std::back_inserter(ids));
		sox::unmarshal_container(p, std::back_inserter(ips));
	}
};

struct PSyncSInfo: public sox::Marshallable{
	enum {uri = (71 << 8 | protocol::SDB_SVID)};	
	uint32_t sid;
	void marshal(sox::Pack & pk) const{
		pk << sid;
	}
	void unmarshal(const sox::Unpack &up){
		up >> sid;
	}
};

struct PSyncSInfoRes: public sox::Properties{
	enum {uri = (71 << 8 | protocol::SDB_SVID)};	
};

struct PGetGuildInfo: public sox::Marshallable{
	enum {uri = (72 << 8 | protocol::SDB_SVID)};
	uint32_t uid;
	void marshal(sox::Pack & pk) const{
		pk << uid;
	}
	void unmarshal(const sox::Unpack &up){
		up >> uid;
	}
};

struct GuildInfo: public sox::Marshallable{
	uint32_t channelId; //别名
	std::string channelName;
	ChannelRoler roler;

	void marshal(sox::Pack & pk) const{
		pk << channelId << channelName << (uint16_t) roler;
	}
	void unmarshal(const sox::Unpack &up){
		up >> channelId >> channelName;
		roler = (ChannelRoler)up.pop_uint16();
	}
};

struct PGetGuildInfoRes: public sox::Marshallable{
	enum {uri = (72 << 8 | protocol::SDB_SVID)};
	uint32_t uid;
	std::vector<GuildInfo> infos;

	void marshal(sox::Pack & pk) const{
		pk << uid;
		sox::marshal_container(pk, infos);
	}
	void unmarshal(const sox::Unpack &up){
		up >> uid;
		sox::unmarshal_container(up, std::back_inserter(infos));
	}
};

struct PLeaveGuild2: public sox::Marshallable{
	enum {uri = (73 << 8 | protocol::SDB_SVID)};
	uint32_t sid;
	void marshal(sox::Pack & pk) const{
		pk << sid;
	}
	void unmarshal(const sox::Unpack &up){
		up >> sid;
	}
};

struct GuildInfo2: public GuildInfo{
	bool isSettle;
	uint32_t realSid; //真实id
	void marshal(sox::Pack & pk) const{
		GuildInfo::marshal(pk);
		pk.push_uint8(isSettle);
		pk << realSid;
	}
	void unmarshal(const sox::Unpack &up){
		GuildInfo::unmarshal(up);
		isSettle = up.pop_uint8() != 0;
		up >> realSid;
	}
};

struct GuildInfo3: public GuildInfo2{
	uint32_t jifen;
	void marshal(sox::Pack & pk) const{
		GuildInfo2::marshal(pk);
		pk << jifen;
	}
	void unmarshal(const sox::Unpack &up){
		GuildInfo2::unmarshal(up);
		up >> jifen;
	}
};

struct PGetGuildInfo2: public PGetGuildInfo{
	enum {uri = (74 << 8 | protocol::SDB_SVID)};
};

struct PGetGuildInfoRes2: public sox::Marshallable{
	enum {uri = (74 << 8 | protocol::SDB_SVID)};
	uint32_t uid;
	std::vector<GuildInfo2> infos;
	uinfo::SEX sex;
	bool bCooldown;
	void marshal(sox::Pack & pk) const{
		pk << uid;
		sox::marshal_container(pk, infos);
		pk.push_uint8(sex);
		pk.push_uint8(bCooldown);
	}
	void unmarshal(const sox::Unpack &up){
		up >> uid;
		sox::unmarshal_container(up, std::back_inserter(infos));
		sex = (protocol::uinfo::SEX)up.pop_uint8();
		bCooldown = up.pop_uint8() != 0;
	}
};

struct PGetGuildInfo3: public PGetGuildInfo{
	enum {uri = (81 << 8 | protocol::SDB_SVID)};
};

struct PGetGuildInfoRes3: public sox::Marshallable{
	enum {uri = (82 << 8 | protocol::SDB_SVID)};
	uint32_t uid;
	std::vector<GuildInfo3> infos;
	uinfo::SEX sex;
	bool bCooldown;
	void marshal(sox::Pack & pk) const{
		pk << uid;
		sox::marshal_container(pk, infos);
		pk.push_uint8(sex);
		pk.push_uint8(bCooldown);
	}
	void unmarshal(const sox::Unpack &up){
		up >> uid;
		sox::unmarshal_container(up, std::back_inserter(infos));
		sex = (protocol::uinfo::SEX)up.pop_uint8();
		bCooldown = up.pop_uint8() != 0;
	}
};


struct PSyncCatalog: public sox::Voidmable{
	enum {uri = (1 << 8 | protocol::CATALOG_SVID)};
};

struct PSyncCatalogRes : public sox::Marshallable {
	enum {uri = (1 << 8 | protocol::CATALOG_SVID)};
	std::vector<protocol::slist::SListItem> items;
	virtual void marshal(sox::Pack &p) const {
		marshal_container(p, items);
	}
	virtual void unmarshal(const sox::Unpack &p) {
		unmarshal_container(p, std::back_inserter(items));
	}
};

struct PUpdateBanPC : public sox::Marshallable{
	enum {uri = (74 << 8) | SDB_SVID};

	uint32_t	uid;	// 被操作者
	bool		isBan;	
	uint32_t	id;		// ban_pc表的id，删除记录时有效

	virtual void marshal(sox::Pack &p) const{
		p << uid;
		p.push_uint8(isBan);
		p << id;
	}

	virtual void unmarshal(const sox::Unpack &p){
		p >> uid;
		isBan = (p.pop_uint8() != 0);
		p >> id;
	}
}; 

struct PUpdateBanPCRes: public PUpdateBanPC{
	enum {uri = (74 << 8) | SDB_SVID};
	std::string ip;

	virtual void marshal(sox::Pack &p) const
	{
		PUpdateBanPC::marshal(p);
		p << ip;
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		PUpdateBanPC::unmarshal(p);
		p >> ip;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////
// 获取ban列表
struct PGetBans_banPC : public sox::Voidmable{
	enum {uri = (75 << 8 | SDB_SVID)};
};

struct BanPCItem
{
	uint32_t	m_id;
	std::string m_ip;
	std::string m_uid_nick;	// 被封人的昵称
};

inline const sox::Unpack& operator >> (const sox::Unpack& up, BanPCItem& banPCItem)
{
	up >> banPCItem.m_id;
	up >> banPCItem.m_ip;
	up >> banPCItem.m_uid_nick;
	return up;
}

inline sox::Pack& operator << (sox::Pack& p, const BanPCItem& banPCItem)
{
	p << banPCItem.m_id;
	p << banPCItem.m_ip;
	p << banPCItem.m_uid_nick;
	return p;
}
// 服务器返回的结果
struct POnGetBans_banPC : public sox::Marshallable
{
	enum {uri = (75 << 8 | SDB_SVID)};
	std::vector<PListMini> ids;
	std::vector<BanPCItem> pcs;

	virtual void marshal(sox::Pack &p) const{
		sox::marshal_container(p, ids);
		sox::marshal_container(p, pcs);
	}

	virtual void unmarshal(const sox::Unpack &up){
		sox::unmarshal_container(up, std::back_inserter(ids));
		sox::unmarshal_container(up, std::back_inserter(pcs));
	}
};

struct PSyncSubSessionOrder : public sox::Marshallable
{
	enum {uri = (76 << 8 | SDB_SVID)};

	uint32_t topmost_sid;

	virtual void marshal(sox::Pack &p) const{
		p << topmost_sid;
	}

	virtual void unmarshal(const sox::Unpack &up){
		up >> topmost_sid;
	}
};

struct Sid_Order
{
	uint32_t	m_sid;
	uint32_t	m_order;
};

inline const sox::Unpack& operator >> (const sox::Unpack& up, Sid_Order& sid2_order)
{
	up >> sid2_order.m_sid;
	up >> sid2_order.m_order;
	return up;
}

inline sox::Pack& operator << (sox::Pack& p, const Sid_Order& sid2_order)
{
	p << sid2_order.m_sid;
	p << sid2_order.m_order;
	return p;
}

struct PSyncSubSessionOrderRes : public protocol::session::PSyncSubSessionOrder
{
	enum {uri = (77 << 8 | SDB_SVID)};

	std::vector<Sid_Order> subsession_order;

	virtual void marshal(sox::Pack &p) const{
		protocol::session::PSyncSubSessionOrder::marshal(p);
		sox::marshal_container(p, subsession_order);
	}

	virtual void unmarshal(const sox::Unpack &up){
		protocol::session::PSyncSubSessionOrder::unmarshal(up);
		sox::unmarshal_container(up, std::back_inserter(subsession_order));
	}
};


struct PSettleGuild2: public sox::Marshallable{
	enum {uri = (78 << 8 | SDB_SVID)};
	uint32_t sid;
	uint32_t uid;

	virtual void marshal(sox::Pack &p) const{
		p << sid << uid;
	}

	virtual void unmarshal(const sox::Unpack &up){
		up >> sid >> uid;
	}
};

struct PSettleGuildRes2: public PSettleGuild2{
	enum {uri = (78 << 8 | SDB_SVID)};
	uint16_t resCode;
	virtual void marshal(sox::Pack &p) const{
		PSettleGuild2::marshal(p);
		p << resCode;
	}

	virtual void unmarshal(const sox::Unpack &up){
		PSettleGuild2::unmarshal(up);
		up >> resCode;
	}
};

struct PUpdateBanID2 :public PUpdateBanID
{
	enum {uri = (79 << 8 | SDB_SVID)};
	std::string reason;
	virtual void marshal(sox::Pack &p) const{
		PUpdateBanID::marshal(p);
		p << reason;
	}

	virtual void unmarshal(const sox::Unpack &up){
		PUpdateBanID::unmarshal(up);
		up >> reason;
	}

};

struct PUpdateBanPC2 :public PUpdateBanPC
{
	enum {uri = (80 << 8 | SDB_SVID)};
	std::string reason;
	virtual void marshal(sox::Pack &p) const{
		PUpdateBanPC::marshal(p);
		p << reason;
	}

	virtual void unmarshal(const sox::Unpack &up){
		PUpdateBanPC::unmarshal(up);
		up >> reason;
	}

};

struct PSyncLeaveList: public sox::Marshallable{
	enum{uri = 245 << 8 | SDB_SVID};
	std::string uid; //保留为空
	virtual void marshal(sox::Pack & pk) const{
		pk << uid;
	}
	virtual void unmarshal(const sox::Unpack &up){
		up >> uid;
	}
};

struct LeaveItem: public sox::Marshallable{
	std::string name;
	uint32_t sid;
	uint32_t asid;
	ChannelRoler oldRoler;
	uint32_t jifen;
	uint32_t leaveTime;			
	uint32_t clearJifenTime;
	std::string adminOldName;		
	uint32_t leaveOpUid;
	virtual void marshal(sox::Pack & pk) const{
		pk << name << sid << asid << jifen << leaveTime << clearJifenTime << adminOldName << leaveOpUid;
		pk.push_uint32(oldRoler);
	}
	virtual void unmarshal(const sox::Unpack &up){
		up >> name >> sid >> asid >> jifen >> leaveTime >> clearJifenTime >> adminOldName >> leaveOpUid;
		oldRoler = (ChannelRoler)up.pop_uint32();
	}
};

struct PSyncLeaveListRes: public sox::Marshallable{
	enum{uri = 246 << 8 | SDB_SVID};
	uint32_t uid; //服务器必须回填
	std::vector<LeaveItem> items;

	virtual void marshal(sox::Pack & pk) const{
		pk << uid;
		sox::marshal_container(pk, items);

	}
	virtual void unmarshal(const sox::Unpack &up){
		up >> uid;
		sox::unmarshal_container(up, std::back_inserter(items));
	}
};
////////////////////////////////////////////////////////////////////////////////////////////

}
}
#endif /*PSINFO_H_*/
