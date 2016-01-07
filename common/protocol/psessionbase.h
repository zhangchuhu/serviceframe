#ifndef PSESSIONBASE_H_
#define PSESSIONBASE_H_
#include "common/packet.h"
#include "common/iproperty.h"
#include "common/protocol/const.h"
#include "ChannelRoler.h"

namespace protocol {
namespace session {
	enum GuildType{
		gt_game = 0,
		gt_fun,
		gt_other,
		gt_education
	};

	enum VoiceQC{
		quality,
		realtime
	};

struct PSessionSid : public sox::Marshallable {
	uint32_t sid;
	PSessionSid() {}
	PSessionSid(uint32_t s): sid(s){}
	virtual void marshal(sox::Pack &p) const {
		p << sid;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> sid;
	}
};

///////////////////////////////////////////////////////////////////
typedef std::map<int, std::set<uint32_t> > is_map_t;

struct ChannelUserMap : public sox::Marshallable
{
    is_map_t us; 
		virtual void marshal(sox::Pack & p) const {   
        p.push_uint32(uint32_t(us.size()));
        for(is_map_t::const_iterator it1=us.begin(); it1!=us.end(); it1++) {   
            std::set<uint32_t> const &refset=it1->second;
            p.push_uint32(uint32_t( refset.size() ) );
            for(std::set<uint32_t>::const_iterator it2=refset.begin(); it2!=refset.end(); it2++) {   
                p.push_uint32( uint32_t(it1->first) );
                p << *it2;
            }   
        }   
    }   
		virtual void unmarshal(const sox::Unpack &p) {   
        for (uint32_t count1 = p.pop_uint32(); count1 > 0; --count1) {   
            for (uint32_t count2 = p.pop_uint32(); count2 > 0; --count2) {   
                uint32_t key;
                uint32_t value;
                p >> key;
                std::set<uint32_t> &refset=us[key];
                p >> value;
                refset.insert(value);
            }   
        }   
    }   
		virtual std::ostream & trace(std::ostream & os) const {   
        return os; 
    }   	
};

///////////////////////////////////////////////////////////////////	
struct ChannelQueueMar : public sox::Marshallable {
	bool mute;
	bool disable;
	uint32_t ring;
	uint32_t count;
	uint32_t validring;
protected:
	std::list<uint32_t> userlist;
public:
	ChannelQueueMar():mute(false),disable(false),ring(300),count(0),validring(300){}	

	virtual void marshal(sox::Pack & p) const {
		p << mute << disable << ring << count << validring;
		sox::marshal_container(p, userlist);
	}

	virtual void unmarshal(const sox::Unpack & p) {
		p >> mute >> disable >> ring >> count >> validring;
		sox::unmarshal_container(p, std::back_inserter(userlist));
	}


	const std::list<uint32_t> &getUserList() const{return userlist;}

	std::list<uint32_t> &getUserListRef() {return userlist;}

        void setUserList(const std::list<uint32_t> &c) { userlist = c;}
};

typedef std::map< uint32_t, ChannelQueueMar > channelqueuemar_map_t;
///////////////////////////////////////////////////////////////////	

struct SidMapProperties : public sox::Marshallable {
	sox::sid2props_map_t mapp;

	virtual void marshal(sox::Pack & p) const {
		sox::marshal_container(p, mapp);
	}

	virtual void unmarshal(const sox::Unpack &p) {
		sox::unmarshal_container(p, sox::properties::replace_iterator(mapp));
	}
};

struct Member : public sox::Marshallable {
	std::string uid;
	uint32_t sid;
	ChannelRoler role;
	Member(){}
	Member(const std::string &u, uint32_t s, ChannelRoler r): uid(u), sid(s), role(r){}
	virtual void marshal(sox::Pack & p) const {
		p << uid << sid;
		p.push_uint16((uint8_t)role);
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> uid >> sid;
		role = (ChannelRoler)p.pop_uint16();
	}
};


struct MRolerAndCh: public sox::Marshallable{
	uint32_t cid;
	ChannelRoler roler;
	MRolerAndCh(){}
	MRolerAndCh(uint32_t c, ChannelRoler r):cid(c), roler(r){}

	bool operator == (const MRolerAndCh& dest) const {
		return (cid == dest.cid && roler == dest.roler);
	}

	virtual void marshal(sox::Pack &p) const {
		p << cid;
		p.push_uint16(roler);
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> cid;
		roler = (ChannelRoler)p.pop_uint16();
	}
};

struct MRolerAndChUser: public MRolerAndCh{
	uint32_t uid;
	MRolerAndChUser(){}
	MRolerAndChUser(uint32_t u, uint32_t c, ChannelRoler r):MRolerAndCh(c, r), uid(u){}

	virtual void marshal(sox::Pack &p) const {
		p << uid;
		MRolerAndCh::marshal(p);
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> uid;
		MRolerAndCh::unmarshal(p);
	}
};

enum ItemType{
	MAN,
	CHANNEL
};

enum ChannelStyle{
	FreeStyle,
	ChairStyle,
	MicrophoneStyle
};

enum MediaTransportStyle{
	FREE_MEDIA,
	FORCE_SINGLE_MEDIA,
	FORCE_DOUBLE_MEDIA
};

struct PListMini : public sox::Marshallable {
	uint32_t uid;
	std::string nick;
	protocol::uinfo::SEX gender;
	virtual void marshal(sox::Pack &pk) const {
		pk << uid << nick;
		pk.push_uint8(gender);
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid >> nick;
		gender = (protocol::uinfo::SEX)up.pop_uint8();
	}
};

struct BanPCItem_wstring
{
	uint32_t		m_id;
	std::wstring	m_ip;
	std::wstring	m_uid_nick;	// 被封人的昵称
};

struct SMember{
	uint32_t tid;                           //
	uint32_t sid;                           //
	uint32_t uid;
	ChannelRoler roler;
	uint32_t jifen;                         //原始积分
	int     addtime;
	std::string strAddTime;
	SMember():roler(NORMAL), jifen(0){
	}

	void adjust(const SMember &sm){
		if(roler < sm.roler){
			roler = sm.roler;
		}
		if(jifen < sm.jifen){
			jifen = sm.jifen;
		}
/*		if(addtime > sm.addtime){
			addtime = sm.addtime;
		}*/
	}

	uint32_t getCalcedJifen() const{
		return jifen / 60;
	}
};

}
}
#endif /*PSESSIONBASE_H_*/
