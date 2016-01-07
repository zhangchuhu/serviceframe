#ifndef PCOMMON_SESSION_H_
#define PCOMMON_SESSION_H_
#include "common/core/base_svid.h"
#include "common/protocol/const.h"
#include "psmanager.h"
#include "common/protocol/psessionbase.h"
#include "common/protocol/psinfo.h"
#include "common/protocol/psmemberlist.h"
#include "common/protocol/plogin.h"
#include "common/protocol/zip_tpl.h"
#include "common/protocol/KickOffType.h"
#include <vector>
#include <iterator>
#include <set>
#include <map>
namespace protocol {
namespace session {

inline uint64_t to64Uid(uint32_t uid32)
{
  return ((static_cast<uint64_t>(uid32))&0x00000000FFFFFFFFLL);
}

enum UserTerminalType
{
  UTT_UNKNOWN     = 0,
  UTT_DESKTOP     = 1,
  UTT_MOBILEPHONE = 2,
  UTT_WEBYY       = 3,
  UTT_WEBYYXL     = 4,
};

enum Svc2ClientStatType
{
  S2C_DEFAULT_TYPE = 0,
  SVC_SUCC_RATE    = 1,
};

//uri 32 - 63
struct PRealJoinChannel : public sox::Marshallable {
	enum {uri = (32 << 8 | protocol::SESSION_SVID)};
	uint32_t uid;
	std::string sha1Pass;
	uint32_t sid;
	uint32_t ssid;
	virtual void marshal(sox::Pack &p) const {
		p << uid << sha1Pass <<  sid <<  ssid;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> uid >> sha1Pass >> sid >>  ssid;
	}
};

struct Partner : public sox::Marshallable {
	uint32_t pid;
	uint32_t uid;
	std::string nick;
	std::string sign;
	uinfo::SEX gender;
	uint32_t uinfoJifen;
	uint32_t smemberJifen;
	Partner() : uinfoJifen(0), smemberJifen(0){
	}
	Partner(uint32_t u, const std::string &n, const std::string &s, uinfo::SEX g,
			uint32_t p, uint32_t uinfoj,uint32_t smbj) : pid(p), 
		uid(u), nick(n), sign(s), gender(g), uinfoJifen(uinfoj), smemberJifen(smbj) {
	}
	virtual void marshal(sox::Pack &p) const {
		p << uid << nick << sign << pid << uinfoJifen << smemberJifen;
		p.push_uint8(gender);
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> uid >> nick >> sign >> pid >> uinfoJifen >> smemberJifen;
		gender = (uinfo::SEX)p.pop_uint8();
	}
};

struct RolerPartner: public Partner{
	uint32_t channel;
	ChannelRoler roler;
	virtual void marshal(sox::Pack &p) const {
		Partner::marshal(p);
		p.push_uint32(channel).push_uint16(roler);
	}
	virtual void unmarshal(const sox::Unpack &p) {
		Partner::unmarshal(p);
		channel = p.pop_uint32();
		roler = (ChannelRoler)p.pop_uint16();
	}
};

struct AuthCode: public sox::Marshallable{
		uint32_t sess_from_role;
		uint32_t sess_to_role;
		uint32_t auth_code;
		virtual void marshal(sox::Pack & p) const {
				p << sess_from_role << sess_to_role << auth_code;
		}

		virtual void unmarshal(const sox::Unpack &p) {
				p >> sess_from_role >>  sess_to_role >> auth_code;
		}
};

struct PRealJoinChannelRes : public sox::Marshallable {
	enum {uri = (32 << 8 | protocol::SESSION_SVID)};
	SidMapProperties baseInfo;
	ChannelUserMap disablevoice;
	ChannelUserMap disabletext;
	std::vector<Partner> partners;
	std::vector<uint32_t> subs;
	std::vector<MRolerAndChUser> rolers;
	std::vector<AuthCode> authes;
	std::vector<uint32_t> chTextDisabled;
	std::vector<uint32_t> settles;
	virtual void marshal(sox::Pack &p) const {
		p << baseInfo;
		p << disablevoice;
		p << disabletext;
		sox::marshal_container(p, partners);
		sox::marshal_container(p, subs);
		sox::marshal_container(p, rolers);
		sox::marshal_container(p, authes);
		sox::marshal_container(p, chTextDisabled);
		sox::marshal_container(p, settles);
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> baseInfo;
		p >> disablevoice;
		p >> disabletext;
		sox::unmarshal_container(p, std::back_inserter(partners));
		sox::unmarshal_container(p, std::back_inserter(subs));
		sox::unmarshal_container(p, std::back_inserter(rolers));
		sox::unmarshal_container(p, std::back_inserter(authes));
		sox::unmarshal_container(p, std::back_inserter(chTextDisabled));
		sox::unmarshal_container(p, std::back_inserter(settles));
	}
};

struct PCreateSubChannel : public PCreateChanel {
	enum {uri = (33 << 8 | protocol::SESSION_SVID)};
	PCreateSubChannel() {
	}
	PCreateSubChannel(const std::string &n, bool l, bool p,
		const std::string &h, const std::string &pwd, uint32_t parent, VoiceQC /*q*/) :
	PCreateChanel(n, l, p, h, CTL, pwd, parent) {
	}
};

struct POnSubChannelAdd : public sox::Marshallable {
	enum {uri = (33 << 8 | protocol::SESSION_SVID)};
	uint32_t sid;
	PSessionInfo info;
	uint32_t creator;

	virtual void marshal(sox::Pack &p) const {
		p << sid << info << creator;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> sid >> info >> creator;
	}
};
//create sub channel as a list add;

struct PDismissSubChannel : public PDismissChanel {
	enum {uri = (34 << 8 | protocol::SESSION_SVID)};
};

struct POnSubChannelRemove : public PSessionSid {
	enum {uri = (34 << 8 | protocol::SESSION_SVID)};

	uint32_t pid;
	uint32_t uid;

	virtual void marshal(sox::Pack &p) const {
		protocol::session::PSessionSid::marshal(p);
		p << pid << uid;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		protocol::session::PSessionSid::unmarshal(p);
		p >> pid >> uid;
	}
};
//dismiss sub channel as a list remove;

struct PLeaveSession : public PSessionSid {
	enum {uri = (35 << 8 | protocol::SESSION_SVID)};
};


struct PChatText : public sox::Marshallable {
	enum {uri = (36 << 8 | protocol::SESSION_SVID)};
	uint32_t from;
	uint32_t sid;
	std::string chat;
	virtual void marshal(sox::Pack &pak) const {
		pak << from << sid << chat;
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		pak >> from >> sid >> chat;
	}
};

struct POneChatText : public sox::Marshallable {
	enum {uri = (37 << 8 | protocol::SESSION_SVID)};
	uint32_t from;
	uint32_t to;
	std::string chat;
	virtual void marshal(sox::Pack &pak) const {
		pak << from << to << chat;
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		pak >> from >> to >> chat;
	}
};

struct PPartChatVoice: public sox::Marshallable{
	uint32_t seq;
	uint32_t timeStamp;

	virtual void marshal(sox::Pack &pak) const {
		pak << seq << timeStamp;
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		pak >> seq >> timeStamp;
	}
};

struct PChatVoice : /*public sox::Marshallable*/public PPartChatVoice{
	enum {uri = (38 << 8 | protocol::SESSION_SVID)};
	uint32_t from;
	uint32_t sid;
	std::string chat;

	virtual void marshal(sox::Pack &pak) const {
		PPartChatVoice::marshal(pak);
		pak << from << sid << chat;
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		PPartChatVoice::unmarshal(pak);
		pak >> from >> sid >> chat;
	}
};

struct PChatQualityVoice: public PChatVoice
{
     enum {uri = (179 << 8 | protocol::SESSION_SVID)};
};

struct PChatQualityVoiceRes : public PChatQualityVoice{
    enum {uri = (180 << 8 | protocol::SESSION_SVID)};
    uint32_t lastseq;

    virtual void marshal(sox::Pack &pak) const {
        PChatQualityVoice::marshal(pak);
        pak << lastseq ;
    }
    virtual void unmarshal(const sox::Unpack &pak) {
        PChatQualityVoice::unmarshal(pak);
        pak >> lastseq ;
    }
};
struct POneChatVoice : public PPartChatVoice {
	enum {uri = (39 << 8 | protocol::SESSION_SVID)};
	uint32_t from;
	uint32_t to;
	std::string chat;

	virtual void marshal(sox::Pack &pak) const {
		PPartChatVoice::marshal(pak);
		pak << from << to << chat;
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		PPartChatVoice::unmarshal(pak);
		pak >> from >> to >> chat;
	}
};

struct PUdpLogin : public sox::Marshallable {
	enum {uri = (40 << 8 | protocol::SESSION_SVID)};
	uint32_t uid;
	std::string cookie;
	uint32_t sid;
	virtual void marshal(sox::Pack &pak) const {
		pak << uid << cookie << sid;
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		pak >> uid >> cookie >> sid;
	}
	;
};

struct PUdpLoginRes : public sox::Voidmable {
	enum {uri = (40 << 8 | protocol::SESSION_SVID)};
};

struct PUdpPing : public sox::Marshallable {
	enum {uri = (41 << 8 | protocol::SESSION_SVID)};
	uint32_t uid;
	uint32_t sid;
	uint32_t stampc;
	PUdpPing(uint32_t pSid = 0, uint32_t pUid = 0, uint32_t pStampc = 0) : uid(pUid), sid(pSid), stampc(pStampc){}

	virtual void marshal(sox::Pack &pak) const {
		pak << uid << sid << stampc; 
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		pak >> uid >> sid >> stampc;
	}
};


struct PChangeStyle : public sox::Marshallable {
	enum {uri = (42 << 8 | protocol::SESSION_SVID)};
	uint32_t sid;
	ChannelStyle style;
	uint32_t who;
	uint32_t microtime;
	virtual void marshal(sox::Pack &pak) const {
		pak << sid << who << microtime;
		pak.push_uint8(style);
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		pak >> sid >> who >> microtime;
		style = (ChannelStyle)pak.pop_uint8();
	}
};

struct PChangeStyleRes : public PChangeStyle {

};

struct PSessKickOff : public POnMemberRemove {
	enum {uri = (43 << 8) | SESSION_SVID};
	std::string reason;
	uint32_t secs;
	virtual void marshal(sox::Pack &p) const {
		POnMemberRemove::marshal(p);
		p << reason << secs;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		POnMemberRemove::unmarshal(p);
		p >> reason >> secs;
	}
};

struct POnSessKickOff : public PSessKickOff {
	enum {uri = (43 << 8) | SESSION_SVID};
	uint32_t admin;
	uint32_t toCh;
	virtual void marshal(sox::Pack &p) const {
		PSessKickOff::marshal(p);
		p << admin << toCh;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		PSessKickOff::unmarshal(p);
		p >> admin >> toCh;
	}
};

struct PTuoRen : public sox::Marshallable {
	enum {uri = (44 << 8) | SESSION_SVID};
	uint32_t uid;
	uint32_t from;
	uint32_t to;

	virtual void marshal(sox::Pack &p) const {
		p << uid << from << to;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> uid >> from >> to;
	}
};

struct POnTuoren: public PTuoRen{
	enum {uri = (44 << 8) | SESSION_SVID};
	uint32_t admin;
	virtual void marshal(sox::Pack &pak) const {
		PTuoRen::marshal(pak);
		pak << admin;
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		PTuoRen::unmarshal(pak);
		pak >> admin;
	}
};

struct PRingBusy : public sox::Voidmable {
	enum {uri = (45 << 8 | protocol::SESSION_SVID)};
};

struct PSetKeyActive : public sox::Marshallable {
	enum {uri = (46 << 8 | protocol::SESSION_SVID)};
	uint32_t target;
	uint32_t sid;
	virtual void marshal(sox::Pack &pak) const {
		pak << target << sid;
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		pak >> target >> sid;
	}
};

struct PNotifySession : public sox::Marshallable {
	enum {uri = (47 << 8 | protocol::SESSION_SVID)};
	uint32_t type;
	std::string msg;
	virtual void marshal(sox::Pack &pk) const {
		pk << type << msg;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> type >> msg;
	}
};

struct PDisableVoice : public sox::Marshallable {
	enum {uri = (48 << 8 | protocol::SESSION_SVID)};
	uint32_t uid;
	bool disable;
	uint32_t sid;
	uint32_t admin;
	virtual void marshal(sox::Pack &pk) const {
		pk << uid << disable << sid << admin;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid >> disable >> sid >> admin;
	}
};

struct PDisableText : public PDisableVoice {
	enum {uri = (49 << 8 | protocol::SESSION_SVID)};
	virtual void marshal(sox::Pack &pk) const {
		PDisableVoice::marshal(pk);
	}
	virtual void unmarshal(const sox::Unpack &up) {
		PDisableVoice::unmarshal(up);
	}
};

struct PJoinQueue : public sox::Marshallable {
	enum {uri = (50 << 8 | protocol::SESSION_SVID)};
	uint32_t uid;
	virtual void marshal(sox::Pack &pk) const {
		pk << uid;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid;
	}
};

struct PLeaveQueue : public sox::Marshallable {
	enum {uri = (51 << 8 | protocol::SESSION_SVID)};
	uint32_t uid;
	virtual void marshal(sox::Pack &pk) const {
		pk << uid;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid;
	}
};

struct PKickOffQueue : public sox::Marshallable {
	enum {uri = (52 << 8 | protocol::SESSION_SVID)};
	uint32_t admin;//服务器填写admin 
	uint32_t uid; 
	virtual void marshal(sox::Pack &pk) const {
		pk << uid << admin;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid >> admin;
	}
};



struct PDoubleTimeQueue : public sox::Marshallable {
	enum {uri = (53 << 8 | protocol::SESSION_SVID)};
	uint32_t uid;
	uint32_t admin;//服务器填写admin 
	uint32_t time;
	virtual void marshal(sox::Pack &pk) const {
		pk << uid << time << admin;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid >> time >> admin;
	}
};

struct PMuteQueue : public sox::Marshallable {
	enum {uri = (54 << 8 | protocol::SESSION_SVID)};
	uint32_t uid;
	uint32_t time;
	bool mute;
	virtual void marshal(sox::Pack &pk) const {
		pk << uid << time << mute;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid >> time >> mute;
	}
};

struct PMoveQueue : public sox::Marshallable {
	enum {uri = (55 << 8 | protocol::SESSION_SVID)};
	uint32_t uid;
	bool down;
	virtual void marshal(sox::Pack &pk) const {
		pk << uid << down;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid >> down;
	}
};

struct PTurnQueue : public sox::Marshallable {
	enum {uri = (56 << 8 | protocol::SESSION_SVID)};
	uint32_t uid;
	uint32_t time;
	virtual void marshal(sox::Pack &pk) const {
		pk << uid << time;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid >> time;
	}
};

struct PTimeOutQueue : public sox::Marshallable {
	enum {uri = (57 << 8 | protocol::SESSION_SVID)};
	uint32_t uid;
	virtual void marshal(sox::Pack &pk) const {
		pk << uid;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid;
	}
};

struct PDisableQueue : public sox::Marshallable {
	enum {uri = (58 << 8 | protocol::SESSION_SVID)};
	uint32_t uid; //服务器转换为 admim
	bool disable;
	virtual void marshal(sox::Pack &pk) const {
		pk << uid << disable;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid >> disable;
	}
};

struct PSyncTime : public sox::Marshallable {
	enum {uri = (59 << 8 | protocol::SESSION_SVID)};
	uint32_t startTime;
	virtual void marshal(sox::Pack &pk) const {
		pk << startTime;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> startTime;
	}
};

struct PSyncTimeRes : public PSyncTime {
	enum {uri = (59 << 8 | protocol::SESSION_SVID)};
	uint32_t serverTime;
	virtual void marshal(sox::Pack &pk) const {
		pk << startTime << serverTime;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> startTime >>serverTime;
	}
};

struct PTuoRenQueue : public sox::Marshallable {
	enum {uri = (60 << 8 | protocol::SESSION_SVID)};
	uint32_t uid;
	uint32_t admin;
	virtual void marshal(sox::Pack &pk) const {
		pk << uid << admin;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid >> admin;
	}
};

struct PRolerMini: public PListMini{
	ChannelRoler role;
	virtual void marshal(sox::Pack &pk) const {
		PListMini::marshal(pk);
		pk.push_uint16(role);
	}
	virtual void unmarshal(const sox::Unpack &up) {
		PListMini::unmarshal(up);
		role = (ChannelRoler)up.pop_uint16();
	}
};


struct PSyncManagers : public sox::Voidmable {
	enum {uri = (62 << 8 | protocol::SESSION_SVID)};
	uint32_t sid;
	virtual void marshal(sox::Pack &pk) const {
		pk << sid;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> sid;
	}
};

struct POnSyncManagers : public sox::Marshallable {
	enum {uri = (62 << 8 | protocol::SESSION_SVID)};
	std::vector<PRolerMini> managers;
	uint32_t sid;
	virtual void marshal(sox::Pack &pk) const {
		sox::marshal_container(pk, managers);
		pk << sid;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		sox::unmarshal_container(up, std::back_inserter(managers));
		up >> sid;
	}
};

// 采用PSessionSyncInfo2后，这条指令可删除
struct PSessionSyncInfo : public sox::Marshallable {
	enum {uri = (2 << 8 | protocol::SMANAGER_SVID)};
	uint32_t serverId;
	std::vector<protocol::slist::SListItem> infos;

	virtual void marshal(sox::Pack &p) const {
		p << serverId;
		sox::marshal_container(p, infos);
	}

	virtual void unmarshal(const sox::Unpack &p) {
		infos.clear();
		p >> serverId;
		sox::unmarshal_container(p, std::back_inserter(infos));
	}
};

struct PSessionSyncInfo2 : public sox::Marshallable {
	enum {uri = (5 << 8 | protocol::SMANAGER_SVID)};
	uint32_t serverId;
	std::vector<protocol::slist::SListItem2> infos;

	virtual void marshal(sox::Pack &p) const {
		p << serverId;
		sox::marshal_container(p, infos);
	}

	virtual void unmarshal(const sox::Unpack &p) {
		infos.clear();
		p >> serverId;
		sox::unmarshal_container(p, std::back_inserter(infos));
	}
};

struct PSessionSyncInfoGet : public sox::Marshallable {
	enum {uri = (3 << 8 | protocol::SMANAGER_SVID)};
	uint32_t top;
	std::vector<protocol::slist::SListItem> items;
	virtual void marshal(sox::Pack &p) const {
		p << top;
		marshal_container(p, items);
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> top;
		unmarshal_container(p, std::back_inserter(items));
	}
};



struct PReconnenct: public sox::Marshallable{
	enum {uri = (64 << 8 | protocol::SESSION_SVID)};
	std::string cookie;
	uint32_t sid;
	//uint32_t ssid;
	virtual void marshal(sox::Pack &p) const {
		p << cookie << sid;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> cookie >> sid;
	}
};

/*struct PInnerUdpLogined: public sox::Marshallable{
	enum{uri = (65 << 8 | protocol::SESSION_SVID)};
	std::string uid;
	virtual void marshal(sox::Pack &p) const {
		p << uid;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> uid;
	}
};*/

struct PStopVoice: public sox::Voidmable{
	enum{uri = (66 << 8 | protocol::SESSION_SVID)};
	uint32_t uid;
	uint32_t sid;
	virtual void marshal(sox::Pack &p) const{
		p << uid << sid;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> uid >> sid;
	}
};


struct PEnableAllText: public sox::Marshallable{
	enum{uri = (67 << 8 | protocol::SESSION_SVID)};
	uint32_t sid;
	bool enable;
	uint32_t admin;
	virtual void marshal(sox::Pack &p) const{
		p << sid << admin << (uint16_t)enable;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> sid >> admin;
		enable = (p.pop_uint16() != 0);
	}
};


struct PSetRoomKeyActive: public sox::Marshallable{
	enum{uri = (68 << 8 | protocol::SESSION_SVID)};
	uint32_t sid;
	uint32_t admin;
	virtual void marshal(sox::Pack &p) const{
		p << sid << admin;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> sid >> admin;
	}
};

struct PEnableAllVoice: public PEnableAllText{
	enum{uri = (69 << 8 | protocol::SESSION_SVID)};
};

struct PKickAllQueue : public sox::Marshallable {
	enum{uri = (70 << 8 | protocol::SESSION_SVID)};
	uint32_t admin;
	virtual void marshal(sox::Pack &p) const {
		p << admin;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> admin;
	}
};

struct PTopQueue : public sox::Marshallable {
	enum{uri = (71 << 8 | protocol::SESSION_SVID)};
	uint32_t uid;

	virtual void marshal(sox::Pack &p) const {
		p << uid;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> uid;
	}
};


struct PLeaveGuild: public PUpdateChanelMember{
	enum{uri = (72 << 8 | protocol::SESSION_SVID)};

};

struct PReSendVoice: public sox::Marshallable{
	enum{uri = (73 << 8 | protocol::SESSION_SVID)};
	uint32_t uid;
	uint32_t seq;

	virtual void marshal(sox::Pack &p) const {
		p << uid << seq;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> uid >> seq;
	}
};

struct PReconnectSess: public sox::Marshallable{
	enum{uri = (74 << 8 | protocol::SESSION_SVID)};
	std::string cookie;
	virtual void marshal(sox::Pack &p) const {
		p << cookie;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> cookie;
	}
};

struct PDirectKickOff: public PSessKickOff{
	enum{uri = (75 << 8 | protocol::SESSION_SVID)};
};

struct PBroadcastVoice: public PChatVoice{
	enum{uri = (76 << 8 | protocol::SESSION_SVID)};

};

struct PBroadcastVoice2: public PBroadcastVoice{
	enum{uri = (200 << 8 | protocol::SESSION_SVID)};
	uint32_t rolerMask;
	virtual void marshal(sox::Pack &p) const {
		PBroadcastVoice::marshal(p);
		p << rolerMask;
		
	}
	virtual void unmarshal(const sox::Unpack &up) {
		PBroadcastVoice::unmarshal(up);
		up >> rolerMask;
	}
};


struct PBroadcastText: public PChatText{
	enum{uri = (77 << 8 | protocol::SESSION_SVID)};

};

struct PBroadcastText2: public PBroadcastText{
	enum{uri = (201 << 8 | protocol::SESSION_SVID)};
	uint32_t rolerMask;
	virtual void marshal(sox::Pack &p) const {
		PBroadcastText::marshal(p);
		p << rolerMask;

	}
	virtual void unmarshal(const sox::Unpack &up) {
		PBroadcastText::unmarshal(up);
		up >> rolerMask;
	}
};

struct PTcpPing : public sox::Marshallable {
	enum {uri = (78 << 8 | protocol::SESSION_SVID)};
	uint32_t uid;
	uint32_t sid;
	uint32_t stampc;
	PTcpPing(uint32_t pSid = 0, uint32_t pUid = 0, uint32_t pStampc = 0) : uid(pUid), sid(pSid), stampc(pStampc){}

	virtual void marshal(sox::Pack &pak) const {
		pak << uid << sid << stampc; 
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		pak >> uid >> sid >> stampc;
	}
};

struct PTcpPingRes: public protocol::login::PPlus{
	enum{uri = (78 << 8 | protocol::SESSION_SVID)};

};

struct PUdpClose: public sox::Voidmable{
	enum{uri = (79 << 8 | protocol::SESSION_SVID)};
};

struct PJoinQueueRes: public sox::Marshallable{
	enum{uri = (80 << 8 | protocol::SESSION_SVID)};
	std::vector<uint32_t> uids;
	virtual void marshal(sox::Pack &p) const {
		sox::marshal_container(p, uids);
	}
	virtual void unmarshal(const sox::Unpack &up) {
		sox::unmarshal_container(up, std::back_inserter(uids));
	}
};


struct PJoinSession2: public sox::Marshallable{
	enum{uri = (81 << 8 | protocol::SESSION_SVID)};
	uint32_t uid;
	uint32_t sid;
	std::string cookie;
	uint32_t subSid;

	virtual void marshal(sox::Pack &p) const {
		p << uid << sid << cookie << subSid;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid >> sid >> cookie >> subSid;
	}
};

typedef ZipMarshal<PRealJoinChannelRes, 82 << 8 | protocol::SESSION_SVID> PJoinChannelZipRes;

struct PJoinSession2_20: public sox::Marshallable{
	enum{uri = (84 << 8 | protocol::SESSION_SVID)};
	uint32_t uid;
	uint32_t sid;
	std::string cookie;
	uint32_t subSid;
	std::string sha_passwd;
	std::string account;

	virtual void marshal(sox::Pack &p) const {
		p << uid << sid << cookie << subSid << sha_passwd << account;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid >> sid >> cookie >> subSid >> sha_passwd >> account;
	}
};

struct PJoinSession2_30: public sox::Marshallable{
	enum{uri = (86 << 8 | protocol::SESSION_SVID)};
	uint32_t uid;
	uint32_t sid;
	std::string cookie;
	uint32_t subSid;
	std::string sha_passwd;
	std::string account;
	std::string nickname;
	std::string sign;
	uinfo::SEX gender;

	virtual void marshal(sox::Pack &p) const {
		p << uid << sid << cookie << subSid << sha_passwd << account;
		p << nickname;
		p << sign;
		p.push_uint8(gender);
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid >> sid >> cookie >> subSid >> sha_passwd >> account;
		up >> nickname;
		up >> sign;
		gender = (uinfo::SEX)up.pop_uint8();
	}
};

// 登陆时带上机器码pcInfo，频道中封机器用到。
struct PJoinSession2_40 : public sox::Marshallable
{
	enum{uri = (87 << 8 | protocol::SESSION_SVID)};
	uint32_t uid;
	uint32_t sid;
	std::string cookie;
	uint32_t subSid;
	std::string sha_passwd;
	std::string account;
	std::string nickname;
	std::string sign;
	uinfo::SEX gender;
	std::string pcInfo;

	virtual void marshal(sox::Pack &p) const {
		p << uid << sid << cookie << subSid << sha_passwd << account;
		p << nickname;
		p << sign;
		p.push_uint8(gender);
		p << pcInfo;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid >> sid >> cookie >> subSid >> sha_passwd >> account;
		up >> nickname;
		up >> sign;
		gender = (uinfo::SEX)up.pop_uint8();
		up >> pcInfo;
	}
};

struct PNewCookie: public sox::Marshallable{
	enum{uri = (83 << 8 | protocol::SESSION_SVID)};
	std::string cookie;

	virtual void marshal(sox::Pack &p) const {
		p << cookie;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> cookie;
	}
};


struct PNewRolers: public sox::Marshallable{
	enum{uri = (84 << 8 | protocol::SESSION_SVID)};
	uint32_t uid;
	uint32_t admin;
	uint32_t jifen;
	std::vector<protocol::session::MRolerAndCh> rolers;
	virtual void marshal(sox::Pack & p) const {
		p << uid << admin << jifen;
		sox::marshal_container(p, rolers);
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> uid >> admin >> jifen;
		sox::unmarshal_container(p, std::back_inserter(rolers));
	}
};

struct PSettleGuild: public PSettleGuild2{
	enum{uri = (85 << 8| protocol::SESSION_SVID)};
};



struct PUserJoin: public sox::Marshallable{
 enum{uri = (92 << 8 | protocol::SESSION_SVID)};
 uint32_t uid;
 uint32_t sid;
 virtual void marshal(sox::Pack &p) const {
  p << uid << sid;
 }
 virtual void unmarshal(const sox::Unpack &up) {
  up >> uid >> sid;
 }
};

struct PTcpProxyPingRes: public PTcpPingRes{
	enum{uri = (131 << 8 | protocol::SESSION_SVID)};
};

struct PUdpProxyPingRes: public protocol::login::PPlus{
	enum{uri = (132 << 8 | protocol::SESSION_SVID)};
};
struct PUdpProxyLoginRes : public sox::Voidmable {
	enum {uri = (133 << 8 | protocol::SESSION_SVID)};
};

struct PUdpProxyClose: public sox::Voidmable{
	enum{uri = (134 << 8 | protocol::SESSION_SVID)};
	uint32_t uid;
	virtual void marshal(sox::Pack &pk) const {
		pk << uid;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid;
	}
};
struct PExchangeKey_ProxyRes : public sox::Marshallable {
	enum {uri = (135 << 8) | protocol::SESSION_SVID};
	std::string encSessionKey;

	virtual void marshal(sox::Pack &pk) const {
		pk << encSessionKey;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> encSessionKey;
	}
};

struct PTcpLogin : public sox::Marshallable {
	enum {uri = (136 << 8 | protocol::SESSION_SVID)};
	uint32_t uid;
	std::string cookie;
	uint32_t sid;
	virtual void marshal(sox::Pack &pak) const {
		pak << uid << cookie << sid;
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		pak >> uid >> cookie >> sid;
	}
};

// 登陆时带上机器码pcInfo，频道中封机器用到。
struct PProxyData : public sox::Marshallable
{
  enum{uri = (137 << 8 | protocol::SESSION_SVID)};
  std::vector<uint32_t> tUserlist; 
  std::string data;
  uint32_t topSid;
  virtual void marshal(sox::Pack &pak) const {
    sox::marshal_container(pak, tUserlist);
    //pak << data;
		pak.push_varstr32(data.data(), data.length());
		pak << topSid;
  }
  virtual void unmarshal(const sox::Unpack &pak) {
    sox::unmarshal_container(pak, std::back_inserter(tUserlist));
    //pak >> data;
    data = pak.pop_varstr32();
    pak >> topSid;
  }
};

struct PProxyData2 : public sox::Marshallable
{
  enum{uri = (175 << 8 | protocol::SESSION_SVID)};
  std::vector<uint32_t> uids;   // 转发目标uid列表
  std::vector<uint32_t> sids;  // 转发目标sid列表
  std::vector<uint32_t> excepts; // 排除的用户。 该列表中的用户不转发
  std::string data;  // 需要前端转发的消息
  uint32_t topSid;
  virtual void marshal(sox::Pack &pak) const {
    marshal_container(pak, uids);
    marshal_container(pak, sids);
    marshal_container(pak, excepts);
    pak.push_varstr32(data.data(), data.length());
    pak << topSid;
  }
  virtual void unmarshal(const sox::Unpack &pak) {
    unmarshal_container(pak, std::back_inserter(uids));
    unmarshal_container(pak, std::back_inserter(sids));
    unmarshal_container(pak, std::back_inserter(excepts));
    data = pak.pop_varstr32();
    pak >> topSid;
  }
};


struct PProxyData3 : public PProxyData2
{
  enum{uri = (304 << 8 | protocol::SESSION_SVID)};
  uint32_t topSid;
  virtual void marshal(sox::Pack &pak) const {
    pak << topSid;
    PProxyData2::marshal(pak);

  }
  virtual void unmarshal(const sox::Unpack &pak) {
    pak >> topSid;
    PProxyData2::unmarshal(pak);
  }
};

//siming:for proxy 
struct PProxyChatVoice :public PChatVoice {
	enum {uri = (138 << 8 | protocol::SESSION_SVID)};
  std::vector<uint32_t> tUserlist; 

  void setVoiceData(PChatVoice& ct)
  {
    seq = ct.seq;
    timeStamp = ct.timeStamp;
    from = ct.from;
    sid = ct.sid;
    chat = ct.chat;
  }

  void setUserList(std::set<uint32_t>& userlist)
  {
    std::copy(userlist.begin(), userlist.end(), std::back_inserter(tUserlist));
  }

	virtual void marshal(sox::Pack &pak) const {
		PChatVoice::marshal(pak);
		sox::marshal_container(pak, tUserlist);
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		PChatVoice::unmarshal(pak);
    sox::unmarshal_container(pak, std::back_inserter(tUserlist));
	}
};





struct PSessionDirect: public sox::Marshallable{
	enum { uri = (139 << 8) | protocol::SESSION_SVID };
	uint32_t sid;
	uint32_t asid;
	uint32_t ip;
	std::vector<uint16_t> ports;
	void marshal(sox::Pack &pk) const{
		pk << sid << asid << ip;
		sox::marshal_container(pk, ports);
	}

	void unmarshal(const sox::Unpack &up){
		up >> sid >> asid >> ip;
		sox::unmarshal_container(up, std::back_inserter(ports));
	}
};
//proxy ping session
struct PTcpPing3: public PUdpPing{
	enum{uri = (140 << 8 | protocol::SESSION_SVID)};
};

struct PTcpPing4: public PUdpPing{
	enum{uri = (142 << 8 | protocol::SESSION_SVID)};
  uint32_t users;

	virtual void marshal(sox::Pack &pak) const {
		PUdpPing::marshal(pak);
		pak << users ;
	}

	virtual void unmarshal(const sox::Unpack &pak) {
		PUdpPing::unmarshal(pak);
    pak >> users;
  }
};

struct PTcpPing4Res: public PTcpPing4{
  enum{uri = (174 << 8 | protocol::SESSION_SVID)};
};


struct PUserMessage: public sox::Marshallable{
  enum {uri = (150 << 8) | SESSION_SVID};
  std::string msg;

  virtual void marshal(sox::Pack &pak) const {
    pak << msg;
  }
  virtual void unmarshal(const sox::Unpack &unpak) {
    unpak >> msg;
  }
};

struct PSessionBroadcast: public sox::Marshallable{
  enum { uri = (151 << 8) | protocol::SESSION_SVID };
  std::vector<std::string> packets;
  void marshal(sox::Pack &pk) const{
    sox::marshal_container(pk, packets);
  }
  void unmarshal(const sox::Unpack &up){
    sox::unmarshal_container(up, std::back_inserter(packets));
  }
};

struct PP2pPing : public sox::Marshallable 
{
  enum {uri = (152 << 8 | protocol::SESSION_SVID)};
  std::vector<uint32_t> rtts;
  
  virtual void marshal(sox::Pack &pak) const {
    sox::marshal_container(pak, rtts); 
  }
  
  virtual void unmarshal(const sox::Unpack &unpak) {
    sox::unmarshal_container(unpak, std::back_inserter(rtts));
  }
};

// 客户端检查到当前游戏类型后通知Session
struct PUserGaming: public sox::Marshallable{
  enum {uri = (153 << 8) | SESSION_SVID};
  uint32_t uid;  // 用户ID
  uint16_t game;  // 游戏类型

  virtual void marshal(sox::Pack &pak) const {
    pak << uid << game;
  }

  virtual void unmarshal(const sox::Unpack &unpak) {
    unpak >> uid >> game;
  }
};


// 服务器通知客户端用户当前游戏信息，该消息可以携带多个用户的游戏信息
struct PNotifyUserGaming: public sox::Marshallable{
  enum {uri = (154 << 8) | SESSION_SVID};
  // <用户ID - uid, 游戏类型 - game>数组
  std::vector<std::pair<uint32_t, uint16_t> > usergamings;

  virtual void marshal(sox::Pack &pak) const {
    sox::marshal_container(pak, usergamings);
  }

  virtual void unmarshal(const sox::Unpack &unpak) {
    sox::unmarshal_container(unpak, std::back_inserter(usergamings));
  }
};

// 客户端检查到当前游戏类型后通知Session
struct PUserGaming2: public sox::Marshallable{
  enum {uri = (155 << 8) | SESSION_SVID};
  uint32_t uid;  // 用户ID
  uint32_t game;  // 游戏类型
  uint32_t reserve1;
  virtual void marshal(sox::Pack &pak) const {
    pak << uid << game << reserve1;
  }
  virtual void unmarshal(const sox::Unpack &unpak) {
    unpak >> uid >> game >> reserve1;
  }
};


// 服务器通知客户端用户当前游戏信息，该消息可以携带多个用户的游戏信息
struct PNotifyUserGaming2: public sox::Marshallable{
  enum {uri = (156 << 8) | SESSION_SVID};
  // <用户ID - uid, 游戏类型 - game>数组
  std::vector<std::pair<uint32_t, uint32_t> > usergamings;
  virtual void marshal(sox::Pack &pak) const {
    sox::marshal_container(pak, usergamings);
  }
  virtual void unmarshal(const sox::Unpack &unpak) {
    sox::unmarshal_container(unpak, std::back_inserter(usergamings));
  }
};


struct PUserMessage2: public sox::Marshallable{
    enum {uri = (157 << 8) | SESSION_SVID};
    uint32_t targetSid;
    uint32_t uid;
    std::string msg;

    virtual void marshal(sox::Pack &pak) const {
        pak << targetSid << uid << msg;
    }

    virtual void unmarshal(const sox::Unpack &unpak) {
        unpak >> targetSid >> uid >> msg;
    }
};


struct PP2pPing2 : public sox::Marshallable {
    enum {uri = (159 << 8 | protocol::SESSION_SVID)};
    uint32_t cursid;  // 当前频道或子频道ID
    std::vector<uint32_t> rtts;
	bool isNewVersion;
    virtual void marshal(sox::Pack &pak) const {
        pak << cursid;
        sox::marshal_container(pak, rtts); 
		pak << isNewVersion;
    }
    virtual void unmarshal(const sox::Unpack &unpak) {
        unpak >> cursid;
        sox::unmarshal_container(unpak, std::back_inserter(rtts));
		if(unpak.empty()){
			isNewVersion = false;
		}else{
			unpak >> isNewVersion;
		}
    }
};


struct PUserGaming3: public sox::Marshallable{
  enum {uri = (160 << 8) | SESSION_SVID};
  uint32_t uid;  // 用户ID
  uint32_t game;  // 游戏类型
  uint32_t reserve1;
  virtual void marshal(sox::Pack &pak) const {
    pak << uid << game << reserve1;
  }
  virtual void unmarshal(const sox::Unpack &unpak) {
    unpak >> uid >> game >> reserve1;
  }
};

struct PLeaveQueueBatch : public sox::Marshallable {
    enum {uri = (158 << 8 | protocol::SESSION_SVID)};
    std::vector<uint32_t> uids;
    virtual void marshal(sox::Pack &pk) const {
        sox::marshal_container(pk, uids); 
    }


    virtual void unmarshal(const sox::Unpack &up) {
        sox::unmarshal_container(up, std::back_inserter(uids));
    }
};

enum MeasureType { MEASURE_RTT = 1 };
struct MeasureValueType : public sox::Marshallable {
	uint64_t hopId;
	uint32_t value;
	MeasureValueType(uint64_t id = 0, uint32_t v = 0)
		: hopId(id), value(v)
	{
	}
	virtual void marshal(sox::Pack &pak) const {
		pak << hopId << value;
	}
	virtual void unmarshal(const sox::Unpack &unpak) {
		unpak >> hopId >> value;
	}
};
struct PP2pPing3 : public sox::Marshallable {
	enum {uri = (161 << 8 | protocol::SESSION_SVID)};
	uint32_t cursid;  // 当前频道或子频道ID
	uint32_t uid;     // 说话用户的uid
	uint32_t serial;  // 序号
	uint8_t measureType;
	std::vector<MeasureValueType> measureValues;

	virtual void marshal(sox::Pack &pak) const {
		pak << cursid << uid << serial << measureType;
		marshal_container(pak, measureValues);
	}
	virtual void unmarshal(const sox::Unpack &unpak) {
		unpak >> cursid >> uid >> serial >> measureType;
		unmarshal_container(unpak, std::back_inserter(measureValues));
	}
};

struct PClientSetJiedai: public sox::Marshallable{
	enum { uri = (165 << 8) | protocol::SESSION_SVID };
	bool bset; // true set; false remove
	uint32_t subChannelId;
	virtual void marshal(sox::Pack & p) const {
		p << bset << subChannelId;
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> bset >> subChannelId;
	}
};

struct PClientSetJiedaiRes: public sox::Marshallable{
	enum { uri = (166 << 8) | protocol::SESSION_SVID };
	bool bset;		//一直为真，通过subChannelId来判断是设置还是取消接待
	uint32_t subChannelId;
	uint32_t admin;
	virtual void marshal(sox::Pack & p) const {
		p << bset << subChannelId << admin;
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> bset >> subChannelId >> admin;
	}
};

struct  PJiedaiInfo : public sox::Marshallable 
{
	enum {uri = (167 << 8) | protocol::SESSION_SVID};
	uint32_t sid;
	uint32_t currentSid;
	virtual void marshal(sox::Pack &pak) const {
		pak << sid << currentSid;
	}
	virtual void unmarshal(const sox::Unpack &unpak) {
		unpak >> sid >> currentSid;
	}

};

enum textstatus{ENABLEALL = 1, DISABLEALL = 2, DISABLEVISTOR = 3};

struct PSetChannelText : public sox::Marshallable 
{
	enum {uri = (168 << 8) | protocol::SESSION_SVID};
	uint32_t sid;
	uint32_t admin;		//发到session时不填，session发回来时填上
	textstatus status;	//status目前可以赋3个值，1表示允许所有人打字，2表示禁止所有人打字，3表示禁止游客打字
	virtual void marshal(sox::Pack &pak) const {
		pak << sid << admin;
		pak.push_uint32(status);
	}
	virtual void unmarshal(const sox::Unpack &unpak) {
		unpak >> sid >> admin;
		status = (textstatus)unpak.pop_uint32();
	}
};

struct PChannelTextInfo : public sox::Marshallable {
	enum {uri = (169 << 8) | protocol::SESSION_SVID};
	std::vector<uint32_t> disableVisitorTextChs;
	virtual void marshal(sox::Pack &p) const {
		sox::marshal_container(p, disableVisitorTextChs);
	}
	virtual void unmarshal(const sox::Unpack &p) {
		sox::unmarshal_container(p, std::back_inserter(disableVisitorTextChs));
	}
};

//用户权限操作密码验证
struct PcheckChannelPasswd :public sox::Marshallable 
{
    enum{uri = (170 << 8) | protocol::SESSION_SVID};
    std::string passwd;
    virtual void marshal(sox::Pack &p) const
    {
        p << passwd;
    }
    virtual void unmarshal(const sox::Unpack &Up)
    {
        Up >> passwd;
    }
};
//enum passwdStatus{PASSWDRIGHT,PASSWDWRONG};
struct PCheckChannelPasswdRes : public sox::Marshallable 
{
    enum{uri = (171 << 8) | protocol::SESSION_SVID};
    bool lsAuthPasswd;
    virtual void marshal(sox::Pack &pak) const {
        pak << lsAuthPasswd;
    }
    virtual void unmarshal(const sox::Unpack &unpak) {
        unpak >> lsAuthPasswd;
    }
};
enum channelAuth{REMOVE, ADD, UPDATE, UPDATE_NAME};



struct PSessionData : public sox::Marshallable
{
  enum{uri = (190 << 8 | protocol::SESSION_SVID)};
  uint32_t uid;
  uint32_t sid;
  std::string rawMessage;
  virtual void marshal(sox::Pack &pak) const {
    pak << uid << sid;
    pak.push_varstr32(rawMessage.data(), rawMessage.length());

  }
  virtual void unmarshal(const sox::Unpack &pak) {
    pak >> uid >> sid;
    rawMessage = pak.pop_varstr32();
  }
};

struct PLeaveProxy : public sox::Marshallable
{
  enum{uri = (191 << 8 | protocol::SESSION_SVID)};
  uint32_t uid;
  uint32_t sid;
  virtual void marshal(sox::Pack &pak) const {
    pak << uid << sid;
  }
  virtual void unmarshal(const sox::Unpack &pak) {
    pak >> uid >> sid;
  }
};

struct PDropUser : public sox::Marshallable
{
  enum{uri = (172 << 8 | protocol::SESSION_SVID)};
  uint32_t uid; 
  virtual void marshal(sox::Pack &pak) const {
    pak << uid;
  }
  virtual void unmarshal(const sox::Unpack &pak) {
    pak >> uid;
  }
};

struct PJoinSessionFromProxy : public PJoinSession2_40
{
  enum{uri = (173 << 8 | protocol::SESSION_SVID)};
};

struct IpInfo: public sox::Marshallable{
  uint32_t ip;
  std::vector<uint16_t> tcpPorts;
  std::vector<uint16_t> udpPorts;

  void marshal(sox::Pack & pk) const{
    pk << ip;
    sox::marshal_container(pk, tcpPorts);
    sox::marshal_container(pk, udpPorts);
  }
  void unmarshal(const sox::Unpack &up){
    up >> ip;
    sox::unmarshal_container(up, std::back_inserter(tcpPorts));
    sox::unmarshal_container(up, std::back_inserter(udpPorts));
  }
};

struct PSessionDirect2: public sox::Marshallable{
  enum { uri = (174 << 8) | protocol::SESSION_SVID };
  uint32_t sid;
  uint32_t asid;
  std::vector<IpInfo> ipInfo;
  void marshal(sox::Pack &pk) const{
    pk << sid << asid;
    sox::marshal_container(pk, ipInfo);
  }

  void unmarshal(const sox::Unpack &up){
    up >> sid >> asid;
    sox::unmarshal_container(up, std::back_inserter(ipInfo));
  }
};

//======================================================================================
#if 0
struct PProxyData2 : public sox::Marshallable
{
  enum{uri = (175 << 8 | protocol::SESSION_SVID)};
  std::vector<uint32_t> uids;
  std::vector<uint32_t> sids;
  std::vector<uint32_t> excepts;
  std::string data;
  virtual void marshal(sox::Pack &pak) const {
    marshal_container(pak, uids);
    marshal_container(pak, sids);
    marshal_container(pak, excepts);
    pak.push_varstr32(data.data(), data.length());

  }
  virtual void unmarshal(const sox::Unpack &pak) {
    unmarshal_container(pak, std::back_inserter(uids));
    unmarshal_container(pak, std::back_inserter(sids));
    unmarshal_container(pak, std::back_inserter(excepts));
    data = pak.pop_varstr32();
  }
};
#endif
enum icontype{GAMEICON = 0, ACTICON = 1, IMID = 2};

// 服务器通知客户端用户当前尾灯信息，该消息可以携带多个用户的尾灯信息
struct PNotifyUserGaming3: public sox::Marshallable{
    enum {uri = (176 << 8) | SESSION_SVID};
    // <用户ID - uid, 游戏类型 - game>数组
    std::vector<std::pair<uint32_t, uint32_t> > usergamings;
    // <用户ID - uid, 活动类型 - act>数组
    std::vector<std::pair<uint32_t, uint32_t> > useracts;
    virtual void marshal(sox::Pack &pak) const {
        sox::marshal_container(pak, usergamings);
        sox::marshal_container(pak, useracts);
    }
    virtual void unmarshal(const sox::Unpack &unpak) {
        sox::unmarshal_container(unpak, std::back_inserter(usergamings));
        sox::unmarshal_container(unpak, std::back_inserter(useracts));
    }
};

struct PSyncUserList : public sox::Marshallable
{
  enum{uri = (177 << 8 | protocol::SESSION_SVID)};
  virtual void marshal(sox::Pack& /*pak*/) const
  {
  }
  virtual void unmarshal(const sox::Unpack& /*pak*/)
  {
  }
};


struct PSyncUserListRes : public PPushSessData
{
  enum{uri = (178 << 8 | protocol::SESSION_SVID)};
};

struct PChannelDestroy : public sox::Marshallable
{
  enum{uri = (181 << 8 | protocol::SESSION_SVID)};
  uint32_t topSid;
  std::vector<uint32_t> sids;
  virtual void marshal(sox::Pack &pak) const {
    pak << topSid;
    marshal_container(pak, sids);
  }
  virtual void unmarshal(const sox::Unpack &pak) {
    pak >> topSid;
    unmarshal_container(pak, std::back_inserter(sids));
  }
};

struct PGetSettleInfo : public sox::Voidmable
{
  enum{uri = (182 << 8 | protocol::SESSION_SVID)};
};

struct SettleInfo: public sox::Marshallable{
  uint32_t topSid;
  std::vector<uint32_t> settles;

  void marshal(sox::Pack & pk) const{
    pk << topSid;
    marshal_container(pk, settles);
  }
  void unmarshal(const sox::Unpack &up){
    up >> topSid;
    unmarshal_container(up, std::back_inserter(settles));
  }
};

struct PSettleInfo : public sox::Marshallable
{
  enum{uri = (183 << 8 | protocol::SESSION_SVID)};
  uint32_t topSid;
  std::vector<uint32_t> settles;

  void marshal(sox::Pack & pk) const{
    pk << topSid;
    marshal_container(pk, settles);
  }
  void unmarshal(const sox::Unpack &up){
    up >> topSid;
    unmarshal_container(up, std::back_inserter(settles));
  }
};

///////////////////////////////////////////////////////////////////////////
// SessionDaemon 相关消息
///////////////////////////////////////////////////////////////////////////

// Session向SessionDaemon注册
struct PRegisterSrv: public sox::Marshallable{
  enum{uri = (185 << 8 | protocol::SESSION_SVID)};
  uint16_t port[4];  // Session服务端口
  uint32_t reserver1;
  uint32_t reserver2;
  virtual void marshal(sox::Pack &p) const {
    for(int i=0;i<4;i++)p<<port[i];
    p << reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    for(int i=0;i<4;i++)up>>port[i];
    up >> reserver1 >> reserver2;
  }
};

struct PRegisterSrvRes : public sox::Marshallable
{
  enum{uri = (186 << 8 | protocol::SESSION_SVID)};
  uint32_t ctlIp;  // Session电信Ip
  uint32_t cncIp;  // Session网通Ip
  uint32_t reserver1;
  uint32_t reserver2;
  virtual void marshal(sox::Pack &pak) const {
    pak << ctlIp << cncIp << reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &pak) {
    pak >> ctlIp >> cncIp >> reserver1 >> reserver2;
  }
};

struct ChannelInfo : public sox::Marshallable {
  uint32_t sid;
  uint32_t ctlUsers;
  uint32_t cncUsers;
  uint32_t reserver1;
  uint32_t reserver2;
  ChannelInfo():sid(0), ctlUsers(0), cncUsers(0)
    , reserver1(0), reserver2(0) {}
    virtual void marshal(sox::Pack &pak) const {
      pak << sid << ctlUsers << cncUsers << reserver1 << reserver2;
    }
    virtual void unmarshal(const sox::Unpack &pak) {
      pak >> sid >> ctlUsers >> cncUsers >> reserver1 >> reserver2;
    }
};

// SessionDaemon Ping包
struct PSessDaemonPing : public sox::Marshallable
{
  enum{uri = (187 << 8 | protocol::SESSION_SVID)};
  std::vector<ChannelInfo> sinfos;
  virtual void marshal(sox::Pack &pak) const {
    marshal_container(pak, sinfos);;
  }
  virtual void unmarshal(const sox::Unpack &pak) {
    unmarshal_container(pak, std::back_inserter(sinfos));
  }
};

struct PSessDaemonPingRes : public sox::Marshallable
{
  enum{uri = (188 << 8 | protocol::SESSION_SVID)};
  uint32_t reserver1;
  uint32_t reserver2;
  virtual void marshal(sox::Pack &pak) const {
    pak << reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &pak) {
    pak >> reserver1 >> reserver2;
  }
};

// 前端向SessionDaemon注册
struct PSubscribeSrv : public sox::Marshallable
{
  enum{uri = (189 << 8 | protocol::SESSION_SVID)};
  uint32_t reserver1;
  uint32_t reserver2;
  virtual void marshal(sox::Pack &pak) const {
    pak << reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &pak) {
    pak >> reserver1 >> reserver2;
  }
};

struct PRequireProxyRes : public sox::Marshallable
{
  enum{uri = (192 << 8 | protocol::SESSION_SVID)};
  uint32_t count;   // 通知前端数
  uint32_t reserver1;
  uint32_t reserver2;
  virtual void marshal(sox::Pack &pak) const {
    pak << count << reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &pak) {
    pak >> count >> reserver1 >> reserver2;
  }
};

// SessionDaemon通知前端
struct PSessionRequire : public sox::Marshallable
{
  enum{uri = (193 << 8 | protocol::SESSION_SVID)};
  uint8_t netType;  // Session网络类型
  uint32_t ip;      // 前端IP
  uint16_t port[4]; // 端口
  uint32_t reserver1;
  uint32_t reserver2;
  virtual void marshal(sox::Pack &pak) const {
    pak.push_uint8(netType);
    pak << ip;
    for(int i=0;i<4;i++) pak << port[i];
    pak << reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &pak) {
    netType = pak.pop_uint8();
    pak >> ip;
    for(int i=0;i<4;i++) pak >> port[i];
    pak >> reserver1 >> reserver2;
  }
};

struct PSubscribeSrvRes : public sox::Marshallable
{
  enum{uri = (196 << 8 | protocol::SESSION_SVID)};
  uint32_t ctlIp;  // 前端电信Ip
  uint32_t cncIp;  // 前端网通Ip
  uint32_t reserver1;
  uint32_t reserver2;
  virtual void marshal(sox::Pack &pak) const {
    pak << ctlIp << cncIp << reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &pak) {
    pak >> ctlIp >> cncIp >> reserver1 >> reserver2;
  }
};

// Session向SessionProxy请求前端
struct PRequireProxy : public sox::Marshallable
{
  enum{uri = (197 << 8 | protocol::SESSION_SVID)};
  uint8_t netType;  // 请求的前端网络类型
  uint32_t reserver1;
  uint32_t reserver2;
  virtual void marshal(sox::Pack &pak) const {
    pak.push_uint8(netType);
    pak << reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &pak) {
    netType = pak.pop_uint8();
    pak >> reserver1 >> reserver2;
  }
};

struct PGetGameInfo : public sox::Voidmable
{
  enum{uri = (198 << 8 | protocol::SESSION_SVID)};
};

struct GameInfo: public sox::Marshallable{
  uint32_t topSid;
  std::string msg;

  void marshal(sox::Pack & pk) const{
    pk << topSid;
    pk.push_varstr32(msg.data(), msg.length());
  }
  void unmarshal(const sox::Unpack &up){
    up >> topSid;
    msg = up.pop_varstr32();
  }
};

struct PUpdatGameInfo : public sox::Marshallable
{
  enum{uri = (199 << 8 | protocol::SESSION_SVID)};
  std::vector<GameInfo> gameInfos;
  virtual void marshal(sox::Pack &pk) const {
    marshal_container(pk, gameInfos);
  }
  virtual void unmarshal(const sox::Unpack &up) {
    unmarshal_container(up, std::back_inserter(gameInfos));
  }
};

struct PGetChannelList : public sox::Marshallable {
  enum{uri = (300 << 8 | protocol::SESSION_SVID)};
  virtual void marshal(sox::Pack &) const {}
  virtual void unmarshal(const sox::Unpack &) {}
};

struct PChannelList : public sox::Marshallable {
  typedef std::set<uint32_t> sid_set;
  enum{uri = (301 << 8 | protocol::SESSION_SVID)};
  sid_set sidList;
  virtual void marshal( sox::Pack & p ) const {
    marshal_container(p, sidList);
  }
  virtual void unmarshal( const sox::Unpack & up ) {
    unmarshal_container(up, std::insert_iterator<sid_set>(sidList, sidList.end()));
  }
};

struct PUserGaming4: public sox::Marshallable
{
	 enum{uri = (307 << 8 | protocol::SESSION_SVID)};
	 uint32_t uid;
	 std::map<uint16_t, uint32_t>  userGameMap;
	 virtual void marshal(sox::Pack &p) const {
		 p << uid;
		 sox::marshal_container(p, userGameMap);
	 }
	 virtual void unmarshal(const sox::Unpack &p) {
		 p >> uid;
		 sox::unmarshal_container(p, std::inserter(userGameMap, userGameMap.begin()));
	 }
};
struct  UserInfoVector: public sox::Marshallable
{
	std::vector<std::pair<uint16_t, uint32_t> > userMessage;
	virtual void marshal(sox::Pack &pak) const {
		sox::marshal_container(pak, userMessage);
	}
	virtual void unmarshal(const sox::Unpack &unpak) {
		sox::unmarshal_container(unpak, std::back_inserter(userMessage));
	}
};
struct PNotifyUserMessage: public sox::Marshallable
{
	enum {uri = (308 << 8) | SESSION_SVID};
	std::map<uint32_t , UserInfoVector> userMessageMap;
	virtual void marshal(sox::Pack &p) const {
		sox::marshal_container(p, userMessageMap);
	}
	virtual void unmarshal(const sox::Unpack &p) {
		sox::unmarshal_container(p, std::inserter(userMessageMap, userMessageMap.begin()));
	}
};

struct POnSessKickOff2: public POnSessKickOff
{
	enum {uri = (309 << 8) | SESSION_SVID};
	KickOffType  type;

	virtual void marshal(sox::Pack &p) const{
		POnSessKickOff::marshal(p);
		p.push_uint32(type);
	}

	virtual void unmarshal(const sox::Unpack &up){
		POnSessKickOff::unmarshal(up);
		type = (KickOffType)up.pop_uint32();
	}
};

struct ChannelRefuseType: public sox::Marshallable
{
	enum {uri = (310 << 8) | SESSION_SVID};
	KickOffType type; 

	virtual void marshal(sox::Pack &p) const{
		p.push_uint32(type);
	}

	virtual void unmarshal(const sox::Unpack &up){
		type = (KickOffType)up.pop_uint32();
	}

};

struct PUpdateChannelMarshal : public sox::Marshallable
{
  enum{uri = (2000 << 8 | protocol::SESSION_SVID)};
  uint32_t topSid;
  std::string msg;
  virtual void marshal(sox::Pack &pak) const {
    pak << topSid;
    pak.push_varstr32(msg.data(), msg.length());
  }
  virtual void unmarshal(const sox::Unpack &pak) {
    pak >> topSid;
    msg = pak.pop_varstr32();
  }
};

struct PJoinSessionRes : public sox::Marshallable
{
  enum{uri = (2001 << 8 | protocol::SESSION_SVID)};
  uint32_t topSid;
  std::vector<uint32_t> successUids;
  std::vector<uint32_t> failureUids;
  std::vector<uint32_t> sids;
  virtual void marshal(sox::Pack &pak) const {
    pak << topSid;
    marshal_container(pak, successUids);
    marshal_container(pak, failureUids);
    marshal_container(pak, sids);
  }
  virtual void unmarshal(const sox::Unpack &pak) {
    pak >> topSid;
    unmarshal_container(pak, std::back_inserter(successUids));
    unmarshal_container(pak, std::back_inserter(failureUids));
    unmarshal_container(pak, std::back_inserter(sids));
  }
};

struct PUpdateChannelMarshal2 : public PUpdateChannelMarshal
{
  enum{uri = (2002 << 8 | protocol::SESSION_SVID)};
  // ?°??ê?·?Dèòa??D?·?????°ü
  uint8_t needReParse;
  // ê?·?′?é?disableVisitorTextChsD??￠
  uint8_t useDisableVisitorText;
  std::vector<uint32_t> disableVisitorTextChs;
  virtual void marshal(sox::Pack &pak) const {
    PUpdateChannelMarshal::marshal(pak);
    pak.push_uint8(needReParse);
    pak.push_uint8(useDisableVisitorText);
    marshal_container(pak, disableVisitorTextChs);
  }
  virtual void unmarshal(const sox::Unpack &pak) {
    PUpdateChannelMarshal::unmarshal(pak);
    needReParse = pak.pop_uint8();
    useDisableVisitorText = pak.pop_uint8();
    unmarshal_container(pak, std::back_inserter(disableVisitorTextChs));
  }
};

struct PSyncUserListRes2 : public PPushSessData
{
  enum{uri = (2003 << 8 | protocol::SESSION_SVID)};
  uint32_t topSid;
  virtual void marshal(sox::Pack &pak) const {
    PPushSessData::marshal(pak);
    pak << topSid;
  }
  virtual void unmarshal(const sox::Unpack &unpak) {
    PPushSessData::unmarshal(unpak);
    unpak >> topSid;
  }
};

//隐藏session相关协议
struct PSessionSyncInfo3 : public sox::Marshallable {
	enum {uri = (311 << 8 | protocol::SMANAGER_SVID)};
	uint32_t serverId;
	std::vector<protocol::slist::SListItem2> infos;
	std::string sessionResPack;
	uint32_t timeStamp;//时间戳
	virtual void marshal(sox::Pack &p) const {
		p << serverId << sessionResPack << timeStamp;
		sox::marshal_container(p, infos);
	}

	virtual void unmarshal(const sox::Unpack &p) {
		infos.clear();
		p >> serverId >> sessionResPack >> timeStamp;
		sox::unmarshal_container(p, std::back_inserter(infos));
	}
};
struct PSessionSyncInfo4 : public PSessionSyncInfo3 {
	enum {uri = (5316 << 8 | protocol::SMANAGER_SVID)};
	uint32_t sessionGroupId;
	uint32_t oldMasterId;
	uint32_t newMasterId;

	virtual void marshal(sox::Pack &p) const {
    PSessionSyncInfo3::marshal(p);
		p << sessionGroupId;
		p << oldMasterId;
		p << newMasterId;
	}

	virtual void unmarshal(const sox::Unpack &p) {
    PSessionSyncInfo3::unmarshal(p);
		p >> sessionGroupId;
		p >> oldMasterId;
		p >> newMasterId;
	}
};

struct PProxySyncInfo : public sox::Marshallable{
	enum {uri = (314 << 8 | protocol::SMANAGER_SVID)};
	uint32_t serverId;
	std::string sessionResPack;
	uint32_t timeStamp;//时间戳
	virtual void marshal(sox::Pack &p) const {
		p << serverId << sessionResPack << timeStamp;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> serverId >> sessionResPack >> timeStamp;
	}

};
struct PProxySyncInfo2 : public PProxySyncInfo {
	enum {uri = (5315 << 8 | protocol::SMANAGER_SVID)};
	uint32_t sessionGroupId;

  virtual void marshal(sox::Pack &p) const {
    PProxySyncInfo::marshal(p);
		p << sessionGroupId;
	}
	virtual void unmarshal(const sox::Unpack &p) {
    PProxySyncInfo::unmarshal(p);
		p >> sessionGroupId;
	}
};

struct proxyInfo2 : public sox::Marshallable
{
	uint32_t dip;
	uint32_t wip;
	uint16_t availableLoads;
	ISPType     serverIsp;
	std::vector<uint16_t> tcp;
	std::vector<uint16_t> udp;

	virtual void marshal(sox::Pack &p) const {
		p << dip << wip << availableLoads ;
		p.push_uint8((uint8_t) serverIsp);
		p.push_uint8((uint8_t) tcp.size());
		for(uint32_t i = 0; i < tcp.size(); ++i)
			p << tcp[i];
		p.push_uint8((uint8_t) udp.size());
		for(uint32_t i = 0; i < udp.size(); ++i)
			p << udp[i];
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> dip >> wip >> availableLoads;
		serverIsp = (ISPType)p.pop_uint8();
		uint8_t tlen;
		tlen = p.pop_uint8();
		for(uint32_t i = 0; i < tlen; ++i)
		{
			uint16_t tcp_i;
			p >> tcp_i;
			tcp.push_back(tcp_i);
		}
		uint8_t ulen;
		ulen = p.pop_uint8();
		for(uint32_t i = 0; i < ulen; ++i)
		{
			uint16_t udp_i;
			p >> udp_i;
			udp.push_back(udp_i);
		}
	}
};

struct proxyInfo : public sox::Marshallable
{
	uint32_t ip;
	uint16_t availableLoads;
	ISPType  serverIsp;
	std::vector<uint16_t> tcp;
	std::vector<uint16_t> udp;

	bool operator < (const proxyInfo& dest) const{
		return (this->availableLoads < dest.availableLoads);
	}

	virtual void marshal(sox::Pack &p) const {
		p << ip << availableLoads ;
		p.push_uint8((uint8_t) serverIsp);
		p.push_uint8((uint8_t) tcp.size());
		for(uint32_t i = 0; i < tcp.size(); ++i)
			p << tcp[i];
		p.push_uint8((uint8_t) udp.size());
		for(uint32_t i = 0; i < udp.size(); ++i)
			p << udp[i];
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> ip >> availableLoads ;
		serverIsp = (ISPType)p.pop_uint8();
		uint8_t tlen;
		tlen = p.pop_uint8();
		for(uint32_t i = 0; i < tlen; ++i)
		{
			uint16_t tcp_i;
			p >> tcp_i;
			tcp.push_back(tcp_i);
		}
		uint8_t ulen;
		ulen = p.pop_uint8();
		for(uint32_t i = 0; i < ulen; ++i)
		{
			uint16_t udp_i;
			p >> udp_i;
			udp.push_back(udp_i);
		}
	}
};


struct proxyInfo3 : public sox::Marshallable
{
  uint32_t ip;
  uint16_t availableLoads;
  ISPType serverIsp;
  AreaType areaType;
  std::vector<uint16_t> tcp;
  std::vector<uint16_t> udp;

  virtual void marshal(sox::Pack &p) const {
    p << ip << availableLoads ;
    p.push_uint8(serverIsp);
    p.push_uint32(areaType);
    p.push_uint8(tcp.size());
    for(uint32_t i = 0; i < tcp.size(); ++i)
      p << tcp[i];
    p.push_uint8(udp.size());
    for(uint32_t i = 0; i < udp.size(); ++i)
      p << udp[i];
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> ip >> availableLoads ;
    serverIsp = (ISPType)p.pop_uint8();
    areaType = (AreaType)p.pop_uint32();
    uint8_t tlen;
    tlen = p.pop_uint8();
    for(uint32_t i = 0; i < tlen; ++i)
    {
      uint16_t tcp_i;
      p >> tcp_i;
      tcp.push_back(tcp_i);
    }
    uint8_t ulen;
    ulen = p.pop_uint8();
    for(uint32_t i = 0; i < ulen; ++i)
    {
      uint16_t udp_i;
      p >> udp_i;
      udp.push_back(udp_i);
    }
  }
};

enum PVersion{NO_PROXY, SINGLE_PROXY, MULTIPLE_PROXY, MULTIPLE_PROXY_NEW};
struct PSessionRes : public sox::Marshallable
{
    enum {uri = (312 << 8 | protocol::SESSION_SVID)};
    uint32_t ip;
    PVersion protocolVersion;
    std::vector<proxyInfo> proxyVectorInfo;
    std::vector<proxyInfo2> proxyVectorInfo2;
    std::vector<uint16_t> ports;
    std::vector<std::pair<uint32_t, uint32_t> > sessionVersion;
    std::string url;
    std::vector<proxyInfo3> proxyVectorInfo3;
    std::vector<proxyInfo2> proxyVectorInfo4;  
    std::vector<proxyInfo3> proxyVectorInfo5;  //大频道实时订阅用户信息模式前端信息. 
    virtual void marshal(sox::Pack &p) const {
        p << ip;
        p.push_uint8(protocolVersion);
        sox::marshal_container(p, proxyVectorInfo);
        sox::marshal_container(p, proxyVectorInfo2);
        sox::marshal_container(p, ports);
        sox::marshal_container(p, sessionVersion);
        p << url;
        sox::marshal_container(p, proxyVectorInfo3);
        sox::marshal_container(p, proxyVectorInfo4);
        sox::marshal_container(p, proxyVectorInfo5);
    }
    virtual void unmarshal(const sox::Unpack &p) {
        p >> ip;
        proxyVectorInfo.clear();
        protocolVersion = (PVersion)p.pop_uint8();
        sox::unmarshal_container(p, std::back_inserter(proxyVectorInfo));
        sox::unmarshal_container(p, std::back_inserter(proxyVectorInfo2));
        sox::unmarshal_container(p, std::back_inserter(ports));
        if(p.size() > 0)
        {
            sox::unmarshal_container(p, std::back_inserter(sessionVersion));
        }
        if(p.size() > 0)
        {
            p >> url;
        }
        if(p.size() > 0)
        {
            sox::unmarshal_container(p, std::back_inserter(proxyVectorInfo3));
        }
        if(p.size() > 0)
        {
            sox::unmarshal_container(p, std::back_inserter(proxyVectorInfo4));
        }
        if(p.size() > 0)
        {
            sox::unmarshal_container(p, std::back_inserter(proxyVectorInfo5));
        }
    }
};


struct POnUpdateSessionJifen: public sox::Marshallable{
	enum{uri = (313 << 8 | protocol::SDB_SVID)};
	uint32_t sid;
	uint32_t uid;
	uint32_t smemberjifen;

	virtual void marshal(sox::Pack &p) const{
		p << sid << uid << smemberjifen;
	}
	virtual void unmarshal(const sox::Unpack &p){
		p >> sid >> uid >> smemberjifen;
	}
};

//===============================================================
//======================================================================================
struct ProxyInfo: public sox::Marshallable{
  uint64_t proxyId;
  uint32_t ip;
  uint16_t tcpPort2Prx;
  uint16_t udpPort2Prx;
  uint32_t reserver1;
  uint32_t reserver2;

  void marshal(sox::Pack & pk) const{
    pk << proxyId << ip << tcpPort2Prx 
      << udpPort2Prx << reserver1 << reserver2;
  }
  void unmarshal(const sox::Unpack &up){
    up >> proxyId >> ip >> tcpPort2Prx 
      >> udpPort2Prx >> reserver1 >> reserver2;
  }
};

struct PDropProxy: public sox::Marshallable{
  enum{uri = (2007 << 8 | protocol::SESSION_SVID)};
  uint64_t proxy_id;
  uint32_t reserver1;
  uint32_t reserver2;
  virtual void marshal(sox::Pack &pk) const {
    pk << proxy_id;
    pk << reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> proxy_id;
    up >> reserver1 >> reserver2;
  }
};

//======================================================================================
struct PInitMaiXu : public sox::Marshallable {
  enum {uri = (2008 << 8 | protocol::SESSION_SVID)};
  uint32_t uid;
  uint32_t sid;
  virtual void marshal(sox::Pack &pk) const {
    pk << uid << sid;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> uid >> sid;
  }
};

//======================================================================================
struct PGetChannelMarshal : public sox::Marshallable {
  enum {uri = (2009 << 8 | protocol::SESSION_SVID)};
  uint32_t topSid;
  virtual void marshal(sox::Pack &pk) const {
    pk << topSid;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> topSid;
  }
};

//======================================================================================
struct PGetChannelUserList : public sox::Marshallable {
  enum {uri = (2010 << 8 | protocol::SESSION_SVID)};
  uint32_t topSid;
  virtual void marshal(sox::Pack &pk) const {
    pk << topSid;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> topSid;
  }
};

//======================================================================================
struct PAddProxy: public sox::Marshallable{
  enum{uri = (2028 << 8 | protocol::SESSION_SVID)};
  std::vector<ProxyInfo> proxyInfos;
  uint32_t reserver1;
  uint32_t reserver2;
  virtual void marshal(sox::Pack &pk) const {
    marshal_container(pk, proxyInfos);
    pk << reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    unmarshal_container(up, std::back_inserter(proxyInfos));
    up >> reserver1 >> reserver2;
  }
};

//=====================================================================================
struct PBackupServer: public sox::Marshallable{
  enum{uri = (2029 << 8 | protocol::SESSION_SVID)};
  uint32_t serverId;
  uint16_t port;
  uint32_t reserver1;
  uint32_t reserver2;
  virtual void marshal(sox::Pack &pk) const {
    pk << serverId << port << reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> serverId >> port >> reserver1 >> reserver2;
  }
};

//======================================================================================
struct PBackupServerRes: public sox::Marshallable{
  enum{uri = (2030 << 8 | protocol::SESSION_SVID)};
  std::set<uint32_t> channels;
  uint32_t serverId;
  uint16_t port;
  uint32_t reserver1;
  uint32_t reserver2;
  virtual void marshal(sox::Pack &pk) const {
    marshal_container(pk, channels);
    pk << serverId << port << reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    sox::unmarshal_container(up, std::inserter(channels, channels.begin()));
    up >> serverId >> port >> reserver1 >> reserver2;
  }
};

struct PBackupData: public sox::Marshallable{
  enum {uri = (2031 << 8) | SESSION_SVID};
  uint32_t sid;
  uint32_t uid;
  uint64_t proxyId;
  uint16_t status;
  std::string key;
  std::string to;
  std::string msg;

  virtual void marshal(sox::Pack &pak) const {
    pak << sid << uid << proxyId << status << key << to;
    pak.push_varstr32(msg.data(), msg.length()); // must use varstr32 here, or else msg will overflow the buffer
  }
  virtual void unmarshal(const sox::Unpack &unpak) {
    unpak >> sid >> uid >> proxyId >> status >> key >> to;
    msg = unpak.pop_varstr32();
  }
};

//======================================================================================
struct PListenPort5: public sox::Marshallable{
  enum{uri = (2032 << 8 | protocol::SESSION_SVID)};
  uint32_t ip;
  uint16_t tcp[4];
  uint16_t udp[4];
  uint16_t tcpPort2Prx;
  uint16_t udpPort2Prx;
  uint64_t proxyId;
  uint32_t reserver1;
  uint32_t reserver2;
  virtual void marshal(sox::Pack &p) const {
    p << ip;
    for(int i=0;i<4;i++)p<<tcp[i];
    for(int i=0;i<4;i++)p<<udp[i];
    p << tcpPort2Prx << udpPort2Prx << proxyId << reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> ip;
    for(int i=0;i<4;i++)up>>tcp[i];
    for(int i=0;i<4;i++)up>>udp[i];
    up >> tcpPort2Prx >> udpPort2Prx >> proxyId >> reserver1 >> reserver2;
  }
};

//======================================================================================
struct PListenPort5Res: public sox::Marshallable{
  enum{uri = (2033 << 8 | protocol::SESSION_SVID)};
  uint32_t serverId;
  std::vector<ProxyInfo> proxyInfos;
  uint32_t reserver1;
  uint32_t reserver2;
  virtual void marshal(sox::Pack &pk) const {
    marshal_container(pk, proxyInfos);
    pk << serverId << reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    unmarshal_container(up, std::back_inserter(proxyInfos));
    up >> serverId >> reserver1 >> reserver2;
  }
};

//======================================================================================
struct PSyncChannelData : public sox::Marshallable {
  enum {uri = (2034 << 8 | protocol::SESSION_SVID)};
  uint32_t topSid;
  virtual void marshal(sox::Pack &pk) const {
    pk << topSid;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> topSid;
  }
};

//======================================================================================
struct PSyncChannelRunTimeInfo : public sox::Marshallable {
  enum {uri = (2035 << 8 | protocol::SESSION_SVID)};
  uint32_t topSid;
  virtual void marshal(sox::Pack &pk) const {
    pk << topSid;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> topSid;
  }
};

//======================================================================================
struct Partner2 : public sox::Marshallable{
  uint32_t uid;
  std::string nick;
  std::string sign;
  uint32_t uinfoJifen;
  uint32_t smemberJifen;
  protocol::uinfo::SEX gender;
  uint32_t current_channel;  //current channel
  uint8_t  isVisitor;
  std::string pcInfo;
  uint32_t operate_count_time;
  uint32_t ip;
  uint32_t loginStamp;
  uint32_t serialNo;
  std::string cookie;
  bool  isFakeUser; // ?¨?μμàê±μ?Dé?aó??§
  ISPType isp;
  std::set<uint64_t> signalProxy;
  std::set<uint64_t> mediaProxy;
  virtual void marshal(sox::Pack &p) const {
    p << uid << nick << sign << uinfoJifen << smemberJifen << current_channel 
      << pcInfo << operate_count_time << ip << loginStamp << serialNo << cookie;
    p.push_uint8(isFakeUser);
    p.push_uint8(gender);
    p.push_uint8(isVisitor);
    p.push_uint8(isp);
    marshal_container(p, signalProxy);
    marshal_container(p, mediaProxy);
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> uid >> nick >> sign >> uinfoJifen >> smemberJifen >> current_channel 
      >> pcInfo >> operate_count_time >> ip >> loginStamp >> serialNo >> cookie;;
    isFakeUser = p.pop_uint8() != 0;
    gender = (uinfo::SEX)p.pop_uint8();
    isVisitor = p.pop_uint8();
    isp = (ISPType)p.pop_uint8();
    sox::unmarshal_container(p, std::inserter(signalProxy, signalProxy.begin()));
    sox::unmarshal_container(p, std::inserter(mediaProxy, mediaProxy.begin()));
  }
};

struct PSyncChannelRunTimeInfoRes : public sox::Marshallable {
  enum {uri = (2036 << 8 | protocol::SESSION_SVID)};
  uint32_t topSid;
  std::vector<Partner2> users;
  channelqueuemar_map_t maixuQueue;
  std::set<uint32_t> settles;
  ChannelUserMap disabletext;
  ChannelUserMap disablevoice;
  std::set<uint32_t> disableTextChs;
  std::set<uint32_t> disableVisitorsTextChs;
  std::set<uint32_t> disableVoiceChs;
  std::set<uint64_t> cncMediaProxy; // added by zrw
  std::set<uint64_t> ctlMediaProxy;
  std::map<uint32_t, std::vector<MRolerAndCh> > roles;
  virtual void marshal(sox::Pack &pak) const {
    pak << topSid;
    sox::marshal_container(pak, users);
    sox::marshal_container(pak, maixuQueue);
    sox::marshal_container(pak, settles);
    disabletext.marshal(pak);
    disablevoice.marshal(pak);
    sox::marshal_container(pak, disableTextChs);
    sox::marshal_container(pak, disableVisitorsTextChs);
    sox::marshal_container(pak, disableVoiceChs);
    sox::marshal_container(pak, cncMediaProxy);
    sox::marshal_container(pak, ctlMediaProxy);
    //sox::marshal_container(pak, roles);
    pak.push_uint32(uint32_t(roles.size())); // use uint32 ...
    for (std::map<uint32_t, std::vector<MRolerAndCh> >::const_iterator it = roles.begin(); it != roles.end(); ++it)
    {
      pak << it->first;
      sox::marshal_container(pak, it->second);
    }
  }
  virtual void unmarshal(const sox::Unpack &pak) {
    pak >> topSid;
    users.reserve(pak.peek_uint32());
    sox::unmarshal_container(pak, std::back_inserter(users));
    sox::unmarshal_container(pak, std::inserter(maixuQueue, maixuQueue.begin()));
    sox::unmarshal_container(pak, std::inserter(settles, settles.begin()));
    disabletext.unmarshal(pak);
    disablevoice.unmarshal(pak);
    sox::unmarshal_container(pak, std::inserter(disableTextChs, disableTextChs.begin()));
    sox::unmarshal_container(pak, std::inserter(disableVisitorsTextChs, disableVisitorsTextChs.begin()));
    sox::unmarshal_container(pak, std::inserter(disableVoiceChs, disableVoiceChs.begin()));
    sox::unmarshal_container(pak, std::inserter(cncMediaProxy, cncMediaProxy.begin()));
    sox::unmarshal_container(pak, std::inserter(ctlMediaProxy, ctlMediaProxy.begin()));
    //sox::unmarshal_container(pak, std::back_inserter(roles));
    for (uint32_t count = pak.pop_uint32(); count > 0; --count)
    {
      uint32_t sid = 0;
      pak >> sid;
      std::vector<MRolerAndCh>& ref = roles[sid];
      sox::unmarshal_container(pak, std::back_inserter(ref));
    }
  }
};

//======================================================================================
struct PAddBackupSession: public sox::Marshallable{
  enum{uri = (2037 << 8 | protocol::SESSION_SVID)};
  uint32_t serverId;
  uint32_t sessionIp;
  uint16_t sessionPort;
  bool isMaster;
  uint32_t reserver1;
  uint32_t reserver2;
  virtual void marshal(sox::Pack &pk) const {
    pk << serverId << sessionIp << sessionPort << isMaster << reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> serverId >> sessionIp>> sessionPort >> isMaster >> reserver1 >> reserver2;
  }
};
//======================================================================================
struct PDropBackupSession: public sox::Marshallable{
  enum{uri = (2038 << 8 | protocol::SESSION_SVID)};
  uint32_t serverId;
  uint32_t reserver1;
  uint32_t reserver2;
  virtual void marshal(sox::Pack &pk) const {
    pk << serverId <<  reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> serverId >> reserver1 >> reserver2;
  }
};

//======================================================================================
struct PActiveSessionServer: public sox::Marshallable{
  enum{uri = (2039 << 8 | protocol::SESSION_SVID)};
  uint32_t serverId;
  uint32_t reserver1;
  uint32_t reserver2;
  virtual void marshal(sox::Pack &pk) const {
    pk << serverId <<  reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> serverId >> reserver1 >> reserver2;
  }
};

//======================================================================================
struct PProxyDelegateReq: public sox::Marshallable{
  enum{uri = (2040 << 8 | protocol::SESSION_SVID)};
  uint64_t proxyId;
  uint32_t reserver1;
  uint32_t reserver2;
  virtual void marshal(sox::Pack &pk) const {
    pk << proxyId <<  reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> proxyId >> reserver1 >> reserver2;
  }
};

//======================================================================================
struct PSessDelegateNtf: public PProxyDelegateReq{
  enum{uri = (2041 << 8 | protocol::SESSION_SVID)};
};

struct PSessProxyDrop: public PDropProxy{
  enum{uri = (2042 << 8 | protocol::SESSION_SVID)};
};

//======================================================================================
struct PChannelDestroyNtf: public sox::Marshallable{
  enum{uri = (2043 << 8 | protocol::SESSION_SVID)};
  uint32_t sid;
  uint32_t reserver1;
  uint32_t reserver2;
  virtual void marshal(sox::Pack &pk) const {
    pk << sid <<  reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> sid >> reserver1 >> reserver2;
  }
};

//======================================================================================
struct PPeerSessPing: public sox::Marshallable{
  enum{uri = (2044 << 8 | protocol::SESSION_SVID)};
  uint32_t seq;
  uint32_t serverId;
  uint32_t reserver1;
  uint32_t reserver2;
  virtual void marshal(sox::Pack &pk) const {
    pk << seq << serverId <<  reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> seq >> serverId >> reserver1 >> reserver2;
  }
};

struct PPeerSessPingRes: public PPeerSessPing{
  enum{uri = (2045 << 8 | protocol::SESSION_SVID)};
};

//======================================================================================
struct PSessCleanupPrx: public sox::Marshallable{
  enum{uri = (2046 << 8 | protocol::SESSION_SVID)};
  uint64_t proxyId;
  uint32_t reserver1;
  uint32_t reserver2;
  virtual void marshal(sox::Pack &pak) const {
    pak << proxyId <<  reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> proxyId >> reserver1 >> reserver2;
  }
};

//======================================================================================
struct ChannelProxyList: public sox::Marshallable{
  uint32_t sid;
  std::vector<uint64_t> proxylist;
  virtual void marshal(sox::Pack &pk) const{
    pk << sid;
    marshal_container(pk, proxylist);
  }
  virtual void unmarshal(const sox::Unpack &up){
    up >> sid;
    unmarshal_container(up, std::back_inserter(proxylist));
  }
};

struct PPushChannelProxyData: public sox::Marshallable{
  enum{uri = (2050 << 8 | protocol::SESSION_SVID)};
  std::vector<ChannelProxyList> sidProxys;
  virtual void marshal(sox::Pack &pk) const{
    marshal_container(pk, sidProxys);
  }
  virtual void unmarshal(const sox::Unpack &up){
    unmarshal_container(up, std::back_inserter(sidProxys));
  }
};

struct ProxyChannelList: public sox::Marshallable{
  uint64_t proxyid;
  std::vector<uint32_t> sidAdds;
  std::vector<uint32_t> sidRemoves;
  virtual void marshal(sox::Pack &pk) const{
    pk << proxyid;
    marshal_container(pk, sidAdds);
    marshal_container(pk, sidRemoves);
  }
  virtual void unmarshal(const sox::Unpack &up){
    up >> proxyid;
    unmarshal_container(up, std::back_inserter(sidAdds));
    unmarshal_container(up, std::back_inserter(sidRemoves));
  }
};

struct PPushProxyChannelData: public sox::Marshallable{
  enum{uri = (2051 << 8 | protocol::SESSION_SVID)};
  std::vector<ProxyChannelList> proxySids;
  virtual void marshal(sox::Pack &pk) const{
    marshal_container(pk, proxySids);
  }
  virtual void unmarshal(const sox::Unpack &up){
    unmarshal_container(up, std::back_inserter(proxySids));
  }
};

struct PPeerProxyData: public sox::Marshallable{
  enum{uri = (2052 << 8 | protocol::SESSION_SVID)};
  uint64_t proxyId;
  std::string msg;

  virtual void marshal(sox::Pack &pak) const {
    pak << proxyId;
    pak.push_varstr32(msg.data(), msg.length()); // must use varstr32 here, or else msg will overflow the buffer
  }
  virtual void unmarshal(const sox::Unpack &unpak) {
    unpak >>proxyId;
    msg = unpak.pop_varstr32();
  }
};

// to be removed, after client upgraded
struct PSyncChannelSerial: public sox::Marshallable{
  enum{uri = (2053 << 8 | protocol::SESSION_SVID)};
  std::set<uint32_t> sids;

  virtual void marshal(sox::Pack &pk) const {
    marshal_container(pk, sids);
  }
  virtual void unmarshal(const sox::Unpack &up) {
    sox::unmarshal_container(up, std::inserter(sids, sids.begin()));
  }
};

struct PSyncChannelSerialRes: public sox::Marshallable
{
  enum {uri = (2054 << 8) | SESSION_SVID};
  std::map<uint32_t , uint32_t> sid2serials;
  virtual void marshal(sox::Pack &p) const {
    sox::marshal_container(p, sid2serials);
  }
  virtual void unmarshal(const sox::Unpack &p) {
    sox::unmarshal_container(p, std::inserter(sid2serials, sid2serials.begin()));
  }
};

//======================================================================================
struct PProxyData4 : public sox::Marshallable
{
  enum{uri = (2055 << 8 | protocol::SESSION_SVID)};
  std::vector<uint32_t> uids;
  std::vector<uint32_t> sids;
  std::vector<uint32_t> excepts;
  std::string data;
  uint32_t topSid;
  uint32_t broadcastSid;
  virtual void marshal(sox::Pack &pak) const {
    marshal_container(pak, uids);
    marshal_container(pak, sids);
    marshal_container(pak, excepts);
    pak.push_varstr32(data.data(), data.length());
    pak << topSid << broadcastSid;
  }
  virtual void unmarshal(const sox::Unpack &pak) {
    unmarshal_container(pak, std::back_inserter(uids));
    unmarshal_container(pak, std::back_inserter(sids));
    unmarshal_container(pak, std::back_inserter(excepts));
    data = pak.pop_varstr32();
    pak >> topSid >> broadcastSid;
  }
};

//======================================================================================
struct PSyncChannelDataRes : public sox::Marshallable {
  enum {uri = (2056 << 8 | protocol::SESSION_SVID)};
  uint32_t topSid;
  std::string channelMarshal;
  virtual void marshal(sox::Pack &pk) const {
    pk << topSid;
    pk.push_varstr32(channelMarshal.data(), channelMarshal.length());
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> topSid;
    channelMarshal = up.pop_varstr32();
  }
};

struct PSessSerialPack : public sox::Marshallable {
  enum{uri = (2057 << 8 | protocol::SESSION_SVID)};
  uint32_t fromSrvId;
  uint32_t serial;
  std::string data;
  virtual void marshal(sox::Pack &p) const {
    p << fromSrvId << serial;
    p.push_varstr32(data.data(), data.length());
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> fromSrvId >> serial;
    data = up.pop_varstr32();
  }
};

//======================================================================================
struct PListenPort6: public sox::Marshallable{
  enum{uri = (2058 << 8 | protocol::SESSION_SVID)};
  uint32_t ip;
  uint16_t tcp[4];
  uint16_t udp[4];
  uint16_t tcpPort2Prx;
  uint16_t udpPort2Prx;
  uint64_t proxyId;
  std::set<uint32_t> existedSids;
  uint32_t proxyType;
  uint32_t groupId;
  uint32_t reserver1;
  uint32_t reserver2;
  virtual void marshal(sox::Pack &p) const {
    p << ip;
    for(int i=0;i<4;i++)p<<tcp[i];
    for(int i=0;i<4;i++)p<<udp[i];
    p << tcpPort2Prx << udpPort2Prx << proxyId << reserver1 << reserver2 << proxyType << groupId;
    marshal_container(p, existedSids);
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> ip;
    for(int i=0;i<4;i++)up>>tcp[i];
    for(int i=0;i<4;i++)up>>udp[i];
    up >> tcpPort2Prx >> udpPort2Prx >> proxyId >> reserver1 >> reserver2 >> proxyType >> groupId;
    unmarshal_container(up, std::inserter(existedSids, existedSids.begin()));

  }
};

//======================================================================================
struct PListenPort6Res: public sox::Marshallable{
  enum{uri = (2059 << 8 | protocol::SESSION_SVID)};
  uint32_t serverId;
  std::vector<ProxyInfo> proxyInfos;
  bool cleanup;
  uint32_t reserver1;
  uint32_t reserver2;
  virtual void marshal(sox::Pack &pk) const {
    marshal_container(pk, proxyInfos);
    pk << serverId << cleanup << reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    unmarshal_container(up, std::back_inserter(proxyInfos));
    up >> serverId >> cleanup >> reserver1 >> reserver2;
  }
};

//======================================================================================
struct PUpdatePrxChannelJifen: public sox::Marshallable{
  enum{uri = (2060 << 8 | protocol::SESSION_SVID)};
  uint32_t topSid;
  std::string jifen;
  virtual void marshal(sox::Pack &pk) const {
    pk << topSid << jifen;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> topSid >> jifen;
  }
};

struct PSyncSidProxyList: public sox::Marshallable
{
	enum{uri = (2061 << 8 | protocol::SESSION_SVID)};
	std::vector<uint32_t> sids;
  virtual void marshal(sox::Pack &pk) const
	{
    sox::marshal_container(pk, sids);
	}
  virtual void unmarshal(const sox::Unpack &up)
	{
    sox::unmarshal_container(up, std::back_inserter(sids));
	}
};

struct POnSyncSidProxyList: public sox::Marshallable
{
  enum{uri = (2062 << 8 | protocol::SESSION_SVID)};
  std::map<uint32_t, std::set<uint64_t> > sidProxyList;
  virtual void marshal(sox::Pack &pk) const
  {
    pk.push_uint32(sidProxyList.size());
    for(std::map<uint32_t, std::set<uint64_t> >::const_iterator it = sidProxyList.begin();it != sidProxyList.end();++it)
    {
      pk.push_uint32(it->first);
      sox::marshal_container(pk, it->second);
    }
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    for(uint32_t i = up.pop_uint32();i > 0;--i)
    {
      uint32_t key = up.pop_uint32();
      std::set<uint64_t>& proxyList = sidProxyList[key];
      sox::unmarshal_container(up, std::inserter(proxyList, proxyList.begin()));
    }
  }
};

struct PProxySerialPack : public PSessSerialPack {
  enum{uri = (2063 << 8 | protocol::SESSION_SVID)};
};

struct PProxySidToSync: public sox::Marshallable
{
  enum{uri = (2064 << 8 | protocol::SESSION_SVID)};
  std::set<uint32_t> sids;
  virtual void marshal(sox::Pack &pk) const
  {
    sox::marshal_container(pk, sids);
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    sox::unmarshal_container(up, std::inserter(sids, sids.begin()));
  }
};

struct PGetProxySidList: public sox::Voidmable
{
  enum{uri = (2065 << 8 | protocol::SESSION_SVID)};
};


struct PListenPort7: public PListenPort6
{
  enum{uri = (2066 << 8 | protocol::SESSION_SVID)};
};

struct PClearMediaProxy: public sox::Marshallable
{
  enum{uri = (2067 << 8 | protocol::SESSION_SVID)};
  uint32_t topSid;
  virtual void marshal(sox::Pack &pk) const
  {
    pk << topSid;
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> topSid;
  }
};

struct PClearMediaProxyRes: public PClearMediaProxy
{
  enum{uri = (2068 << 8 | protocol::SESSION_SVID)};
};

//======================================================================================
struct PSyncChannelData2 : public sox::Marshallable {
  enum {uri = (2069 << 8 | protocol::SESSION_SVID)};
  uint32_t topSid;
  std::string md5;
  virtual void marshal(sox::Pack &pk) const {
    pk << topSid << md5;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> topSid >> md5;
  }
};

struct PProxyEndPointsChanged : public sox::Marshallable {
  enum{ uri= ( 2070 << 8 | protocol::SESSION_SVID ) };
  uint32_t ip;
  ::std::vector<uint16_t> tcpPorts;
  ::std::vector<uint16_t> udpPorts;
  uint64_t proxyId;
  uint32_t reserved1;
  uint32_t reserved2;

  virtual void marshal( sox::Pack & p ) const {
    p << ip;
    marshal_container( p, tcpPorts );
    marshal_container( p, udpPorts );
    p << proxyId << reserved1 << reserved2;
  }

  virtual void unmarshal( const sox::Unpack & up ) {
    up >> ip;
    unmarshal_container( up, ::std::back_inserter( tcpPorts ) );
    unmarshal_container( up, ::std::back_inserter( udpPorts ) );
    up >> proxyId >> reserved1 >> reserved2;
  }
};

struct PProxySerialPack2: public sox::Marshallable
{
  enum{uri = (2077 << 8 | protocol::SESSION_SVID)};
  uint32_t fromSrvId;
  uint32_t serial;
  uint32_t instance;
  std::string data;
  virtual void marshal(sox::Pack &pk) const
  {
    pk << fromSrvId << serial << instance;
    pk.push_varstr32(data.data(), data.length());
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> fromSrvId >> serial >> instance;
    data = up.pop_varstr32();
  }
};

struct PMediaStyleChanged: public sox::Marshallable
{
  enum{uri = (2078 << 8 | protocol::SESSION_SVID)};
  MediaTransportStyle style;
  virtual void marshal(sox::Pack& pk) const
  {
    pk.push_uint32(uint32_t(style));
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    style = (MediaTransportStyle)up.pop_uint32();
  }
};

struct PAllowDoubleVoice: public sox::Marshallable
{
  enum{uri = (2079 << 8 | protocol::SESSION_SVID)};
  bool allow;
  virtual void marshal(sox::Pack& pk) const
  {
    pk << allow;
  }
  virtual void unmarshal(const sox::Unpack& up)
  {
    up >> allow;
  }
};

//======================================================================================
enum ExceptionReason
{
  SYN_ATTACK_FOUND = 1,
  DOS_ATTACK_FOUND = 2,
  CPU_OVERLOAD = 4,
  MEM_OVERLOAD = 8,
};

struct PProxyException2: public sox::Marshallable
{
  enum{ uri = (505 << 8 | protocol::SESSION_SVID ) };
  ExceptionReason reason;
  uint64_t proxy_id;
  virtual void marshal( sox::Pack& pk ) const
  {
    pk.push_uint16( reason );
    pk << proxy_id;
  }
  virtual void unmarshal( const sox::Unpack& up )
  {
    reason = (ExceptionReason)up.pop_uint16();
    up >> proxy_id;
  }
};

struct PProxyRecover2: public sox::Marshallable
{
  enum{ uri = (506 << 8 | protocol::SESSION_SVID ) };
  ExceptionReason reason;
  uint64_t proxy_id;
  virtual void marshal( sox::Pack& pk ) const
  {
    pk.push_uint16( reason );
    pk << proxy_id;
  }
  virtual void unmarshal( const sox::Unpack& up )
  {
    reason = (ExceptionReason)up.pop_uint16();
    up >> proxy_id;
  }
};

//======================================================================================
// SessionDaemon2 Ping°ü
struct PSessDaemonPing2 : public sox::Marshallable
{
  enum{uri = (509 << 8 | protocol::SESSION_SVID)};
  uint16_t isMaster;// 1 master session ; 0 slave session
  uint32_t peerSessSrvId;
  std::vector<ChannelInfo> sinfos;
  uint32_t reserver1;
  uint32_t reserver2;

  virtual void marshal(sox::Pack &pak) const {
    pak << isMaster << peerSessSrvId << reserver1 << reserver2;
    marshal_container(pak, sinfos);;
  }
  virtual void unmarshal(const sox::Unpack &pak) {
    pak >> isMaster >> peerSessSrvId >> reserver1 >> reserver2;
    unmarshal_container(pak, std::back_inserter(sinfos));
  }
};

struct PSessionEventReport: public sox::Marshallable
{
  enum{ uri = (511 << 8 | protocol::SESSION_SVID ) };
  uint32_t serverId;
  std::string report;

  virtual void marshal(sox::Pack &pk) const
  {
    pk << serverId;
    pk.push_varstr32(report.data(), report.length());
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> serverId;
    report = up.pop_varstr32();
  }
};

//======================================================================================
// Session?òSessionDaemon×￠2á
struct PRegisterSrv2: public sox::Marshallable{
  enum{uri = (513 << 8 | protocol::SESSION_SVID)};
  uint16_t ports[4];  // Session·t?????ú
  uint32_t serverId;// serverId
  uint32_t controlPort;// ????ì¨???ú
  uint32_t sessionSvnV;//svn session°?±?o?
  std::string sessionV;//session3ìDò??×?
  uint32_t reserver1; 
  uint32_t reserver2;
  virtual void marshal(sox::Pack &p) const {
    for(int i=0; i < 4; i++)p << ports[i];
    p << serverId << controlPort << sessionSvnV << sessionV;
    p << reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    for(int i=0; i<4 ; i++)up >> ports[i];
    up >> serverId >> controlPort >> sessionSvnV >> sessionV;
    up >> reserver1 >> reserver2;
  }
};


//======================================================================================
// SessionDaemon3 Ping°ü
struct PSessDaemonPing3 : public sox::Marshallable
{
  enum{uri = (514 << 8 | protocol::SESSION_SVID)};
  uint16_t isMaster;// 1 master session ; 0 slave session
  uint32_t peerSessSrvId;
  std::vector<ChannelInfo> sinfos;
  uint32_t doubleUser;
  uint32_t totalUser;
  uint32_t backupStatus; // session ±?·Y×′ì?, 1,±?·Yíê3é 0 ±?·Y?D 
  uint32_t reserver1;
  uint32_t reserver2;

  virtual void marshal(sox::Pack &pak) const {
    pak << isMaster << peerSessSrvId << doubleUser << totalUser;
    pak << backupStatus << reserver1 << reserver2;
    marshal_container(pak, sinfos);;
  }
  virtual void unmarshal(const sox::Unpack &pak) {
    pak >> isMaster >> peerSessSrvId >> doubleUser >> totalUser;
    pak >> backupStatus >> reserver1 >> reserver2;
    unmarshal_container(pak, std::back_inserter(sinfos));
  }
};

//===========================双路前端============================

//==========new protocol support double Link with serial ============

struct PSerialPack : public sox::Marshallable {
	enum{uri = (800 << 8 | protocol::SESSION_SVID)};
	uint32_t serial;
	std::string data;
	virtual void marshal(sox::Pack &p) const {
		p << serial << data;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> serial >> data;
	}
};

struct PLoginSignalProxy : public sox::Marshallable {
	enum{uri = (801 << 8 | protocol::SESSION_SVID)};
	uint32_t uid;
	uint32_t sid;
	uint32_t subSid;
	uint32_t clientVersion;
	std::string cookie;
	std::string sha_passwd;
	std::string account;
	std::string nickname;
	std::string sign;
	std::string pcInfo;
	uinfo::SEX gender;

	virtual void marshal(sox::Pack &p) const {
		p << uid << sid << subSid << clientVersion << cookie;
		p <<sha_passwd << account << nickname << sign << pcInfo;
		p.push_uint8(gender);
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid >> sid >> subSid >> clientVersion >> cookie;
		up >> sha_passwd >> account >> nickname >> sign >> pcInfo;
		gender = (uinfo::SEX)up.pop_uint8();
	}
};

enum SrvSessInstanceStatus
{
  E_SRV_SESSION_NEW = 0,    //服务器session实例第一次被创建
  E_SRV_SESSION_EXIST,      //服务器session实例已经存在. 
  E_SRV_SESSION_UNKNOWN                                    
};

enum LoginRetryProc
{
  E_LOGIN_SUCCESS = 0, // 登录成功，不需要重连
  E_DONT_RETRY = 1, // 保留
  E_RETRY_NORMAL = 2, // 按现有机制重连
  E_RETRY_THIS_ONLY = 3, // 指定只重连本前端，不重新进频道
  E_RETRY_OTHER_IMMD = 4, // 指定重连其他前端，不重新进频道
  E_RETRY_PRESERVE
};

struct PLoginSignalProxyRes : public sox::Marshallable {
  enum{uri = (802 << 8 | protocol::SESSION_SVID)};
  
  enum KickOffType 
  {
    KICK_OFF, 
    BAN_ID, 
    BAN_IP, 
    BAN_PC,
    LOGIN_SUCCESS, 
    SIMPLE_KICK,
    CHANNEL_FULL      = 10, // 频道人数达到上限
    CHANNEL_CONGEST   = 11, // 进频道拥塞（同时进频道人数太多）
    CHANNEL_NOT_EXIST = 12, // 频道不存在
    CHANNEL_FROZEN    = 13,
    CHANNEL_LOCKED    = 14,
    AUTH_FAIL         = 15, // 验证失败
  };
  
  uint32_t sid;
  uint32_t uid;
  uint32_t serverVersion;
  uint32_t loginStamp;
  KickOffType loginStatus;
  std::string errorInfo;
  SrvSessInstanceStatus serverStatus;
  LoginRetryProc retryType; // 重连操作控制
  uint16_t retryFreq; // 重连频率控制，单位s

  PLoginSignalProxyRes(): 
    serverStatus(E_SRV_SESSION_UNKNOWN),
    retryType(E_LOGIN_SUCCESS),
    retryFreq(10) {}

  virtual void marshal(sox::Pack &p) const {
    p << sid << uid << serverVersion << loginStamp;
    p.push_uint8(loginStatus);
    p << errorInfo;
    p.push_uint8(serverStatus);
    p.push_uint8(retryType);
    p << retryFreq;
  }

  virtual void unmarshal(const sox::Unpack &up) {
    up >> sid >> uid >> serverVersion >> loginStamp;
    loginStatus = (KickOffType)up.pop_uint8();
    up >> errorInfo;
    if (!up.empty())
    {
      serverStatus = (SrvSessInstanceStatus)up.pop_uint8();
    }
    if (!up.empty())
    {
       retryType = (LoginRetryProc)up.pop_uint8();
       up >> retryFreq;
    }
  }
};

struct PLoginMediaProxy : public sox::Marshallable {
	enum{uri = (803 << 8 | protocol::SESSION_SVID)};
	uint32_t uid;
	uint32_t sid;
	uint32_t subSid;
	std::string cookie;
	uint32_t loginStamp;  //预留
	PLoginMediaProxy(uint32_t pSid = 0, uint32_t pUid = 0, std::string pCookie = "", uint32_t pLoginStamp = 0, uint32_t pSubSid = 0)
		: uid(pUid), sid(pSid), subSid(pSubSid), cookie(pCookie), loginStamp(pLoginStamp)
	{
	}
	virtual void marshal(sox::Pack &p) const {
		p << uid << sid  << subSid << cookie << loginStamp;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid >> sid >> subSid >> cookie >> loginStamp;
	}
};

struct PLoginMediaProxyRes : public sox::Marshallable {
	enum{uri = (804 << 8 | protocol::SESSION_SVID)};
	uint32_t sid;
	uint32_t uid;
	virtual void marshal(sox::Pack &p) const {
		p << sid << uid;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> sid >> uid;
	}
};

struct PGetChannelInfo : public sox::Marshallable {
	enum{uri = (805 << 8 | protocol::SESSION_SVID)};
	uint32_t sid;
	uint32_t uid;
	PGetChannelInfo(uint32_t pSid = 0, uint32_t pUid = 0) : sid(pSid), uid(pUid){}
	virtual void marshal(sox::Pack &p) const {
		p << sid << uid;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> sid >> uid;
	}
};

struct PGetChannelInfoRes : public sox::Marshallable {
	enum {uri = (806 << 8 | protocol::SESSION_SVID)};
	uint32_t topSid;                  // 顶层频道id
	uint32_t receptionSid;            // 接待频道id
	SidMapProperties baseInfo;        // 频道属性
	ChannelUserMap disablevoice;      // 被禁言的用户列表
	ChannelUserMap disabletext;       // 被禁止打字的用户列表
	std::vector<uint32_t> subs;       // 频道id列表
	std::vector<AuthCode> authes;     // 权限对抗变
	std::vector<uint32_t> chTextDisabled; // 禁止打字的频道
	std::vector<uint32_t> disableVisitorTextChs; // 禁止游客打字的频道
	virtual void marshal(sox::Pack &p) const {
		p << topSid << receptionSid;
		p << baseInfo;
		p << disablevoice;
		p << disabletext;
		sox::marshal_container(p, subs);
		sox::marshal_container(p, authes);
		sox::marshal_container(p, chTextDisabled);
		sox::marshal_container(p, disableVisitorTextChs);
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> topSid >> receptionSid;
		up >> baseInfo;
		up >> disablevoice;
		up >> disabletext;
		sox::unmarshal_container(up, std::back_inserter(subs));
		sox::unmarshal_container(up, std::back_inserter(authes));
		sox::unmarshal_container(up, std::back_inserter(chTextDisabled));
		sox::unmarshal_container(up, std::back_inserter(disableVisitorTextChs));
	}
};

struct PGetAllUserData : public sox::Marshallable {
	enum{uri = (807 << 8 | protocol::SESSION_SVID)};
	uint32_t sid;
	uint32_t uid;
	PGetAllUserData(uint32_t pSid = 0, uint32_t pUid = 0) : sid(pSid), uid(pUid){}
	virtual void marshal(sox::Pack &p) const {
		p << sid << uid;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> sid >> uid;
	}
};

struct PLeaveSignalProxy : public sox::Marshallable{
	enum{uri = (808 << 8 | protocol::SESSION_SVID)};
	uint32_t uid;
	uint32_t sid;
	PLeaveSignalProxy(uint32_t pSid = 0, uint32_t pUid = 0) : uid(pUid), sid(pSid){}
	virtual void marshal(sox::Pack &p) const {
		p << uid << sid;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid >> sid;
	}
};

struct PLeaveMediaProxy : public sox::Marshallable {
	enum{uri = (809 << 8 | protocol::SESSION_SVID)};
	uint32_t uid;
	uint32_t sid;
	uint32_t loginStamp;
	PLeaveMediaProxy(uint32_t pSid = 0, uint32_t pUid = 0, uint32_t pLoginStamp = 0)
		:uid(pUid), sid(pSid), loginStamp(pLoginStamp){
	}
	virtual void marshal(sox::Pack &p) const {
		p << uid << sid << loginStamp;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid >> sid >> loginStamp;
	}
};

struct PLeaveMediaProxyUdp : public sox::Marshallable {
	enum{uri = (888 << 8 | protocol::SESSION_SVID)};
	uint32_t uid;
	uint32_t sid;
	uint32_t loginStamp;
	PLeaveMediaProxyUdp(uint32_t pSid = 0, uint32_t pUid = 0, uint32_t pLoginStamp = 0)
		:uid(pUid), sid(pSid), loginStamp(pLoginStamp){
		}
		virtual void marshal(sox::Pack &p) const {
			p << uid << sid << loginStamp;
		}
		virtual void unmarshal(const sox::Unpack &up) {
			up >> uid >> sid >> loginStamp;
		}
};

struct PSessionDirect3: public sox::Marshallable{
	enum {uri = (810 << 8) | protocol::SESSION_SVID };
	uint32_t sid;
	uint32_t asid;
	std::vector<IpInfo> signalProxyInfo;
	std::vector<IpInfo> mediaProxyInfo;
	std::vector<IpInfo> otherProxyInfo;
	std::vector<IpInfo> superProxyInfo;
	void marshal(sox::Pack &p) const{
		p << sid << asid;
		sox::marshal_container(p, signalProxyInfo);
		sox::marshal_container(p, mediaProxyInfo);
		sox::marshal_container(p, otherProxyInfo);
		sox::marshal_container(p, superProxyInfo);
	}

	void unmarshal(const sox::Unpack &up){
		up >> sid >> asid;
		sox::unmarshal_container(up, std::back_inserter(signalProxyInfo));
		sox::unmarshal_container(up, std::back_inserter(mediaProxyInfo));
		sox::unmarshal_container(up, std::back_inserter(otherProxyInfo));
		sox::unmarshal_container(up, std::back_inserter(superProxyInfo));
	}
};

struct PSessionProxyUpdate: public sox::Marshallable {
	enum {uri = (811 << 8 | protocol::SESSION_SVID)};
	bool isAdd;
	bool bSignalProxy;
	bool bMediaProxy;
	bool bSuperProxy;
	IpInfo proxyAddrInfo;
	virtual void marshal(sox::Pack &p) const {
		p << isAdd << bSignalProxy << bMediaProxy << bSuperProxy;
		p << proxyAddrInfo;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> isAdd >> bSignalProxy >> bMediaProxy >> bSuperProxy;
		up >> proxyAddrInfo;
	}
};

struct PSessionProxyUpdate2: public sox::Marshallable {
	enum {uri = (2206 << 8 | protocol::SESSION_SVID)};
	bool isAdd;
	bool bSignalProxy;
	bool bMediaProxy;
	bool bSuperProxy;
	proxyInfo proxyAddrInfo;
	virtual void marshal(sox::Pack &p) const {
		p << isAdd << bSignalProxy << bMediaProxy << bSuperProxy;
		p << proxyAddrInfo;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> isAdd >> bSignalProxy >> bMediaProxy >> bSuperProxy;
		up >> proxyAddrInfo;
	}
};

struct PSignalProxyPing : public sox::Marshallable {
	enum {uri = (812 << 8 | protocol::SESSION_SVID)};
	uint32_t uid;
	uint32_t sid;
	uint32_t stampc;

	PSignalProxyPing(uint32_t pSid = 0, uint32_t pUid = 0, uint32_t pStampc = 0)
		:uid(pUid), sid(pSid), stampc(pStampc){
	}
	virtual void marshal(sox::Pack &pak) const {
		pak << uid << sid << stampc;
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		pak >> uid >> sid >> stampc;
	}
};

struct PSignalProxyPingRes: public sox::Marshallable{
	enum{uri = (813 << 8 | protocol::SESSION_SVID)};
	uint32_t stampc;
	uint32_t stamps;
	uint32_t sessionRtt;

	virtual void marshal(sox::Pack &pk) const {
		pk << stampc << stamps << sessionRtt;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> stampc >> stamps >> sessionRtt;
	}
};

struct PMediaProxyPing : public sox::Marshallable {
	enum {uri = (814 << 8 | protocol::SESSION_SVID)};
	uint32_t uid;
	uint32_t sid;
	uint32_t stampc;
	uint32_t loginStamp;
	PMediaProxyPing(uint32_t pSid = 0, uint32_t pUid = 0, uint32_t pLoginStamp = 0, uint32_t pStampc = 0)
		:uid(pUid), sid(pSid), stampc(pStampc), loginStamp(pLoginStamp){
	}

	virtual void marshal(sox::Pack &pak) const {
		pak << uid << sid << stampc << loginStamp;
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		pak >> uid >> sid >> stampc >> loginStamp;
	}
};

struct PMediaProxyPingRes: public sox::Marshallable{
	enum{uri = (815 << 8 | protocol::SESSION_SVID)};
	uint32_t stampc;
	uint32_t stamps;
	uint32_t proxyRtt;

	virtual void marshal(sox::Pack &pk) const {
		pk << stampc << stamps << proxyRtt;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> stampc >> stamps >> proxyRtt;
	}
};

struct PMediaProxyRealRtt: public sox::Marshallable{
	enum{uri = (839 << 8 | protocol::SESSION_SVID)};
	uint32_t sid;
	uint32_t uid;
	uint32_t proxyRtt;

	PMediaProxyRealRtt(uint32_t pSid = 0, uint32_t pUid = 0, uint32_t pProxyRtt = 0)
		:sid(pSid), uid(pUid), proxyRtt(pProxyRtt)
	{}

	virtual void marshal(sox::Pack &pk) const {
		pk << sid << uid << proxyRtt;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> sid >> uid >> proxyRtt;
	}
};

struct PMediaProxyCheck : public sox::Marshallable {
	enum {uri = (816 << 8 | protocol::SESSION_SVID)};
	uint32_t uid;
	uint32_t sid;
	uint32_t stampc;
	uint32_t loginStamp;
	PMediaProxyCheck(uint32_t pSid = 0, uint32_t pUid = 0, uint32_t pLoginStamp = 0, uint32_t pStampc = 0)
		:uid(pUid), sid(pSid), stampc(pStampc), loginStamp(pLoginStamp){
	}

	virtual void marshal(sox::Pack &pak) const {
		pak << uid << sid << stampc << loginStamp;
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		pak >> uid >> sid >> stampc >> loginStamp;
	}
};

struct PMediaProxyCheckRes: public PMediaProxyPingRes {
	enum{uri = (817 << 8 | protocol::SESSION_SVID)};
};

struct PIpPacked : public sox::Marshallable
{
	enum{uri = (818 << 8 | protocol::SESSION_SVID)};
	uint32_t ip;
	std::string rawMessage;
	virtual void marshal(sox::Pack &pak) const {
		pak << ip;
		pak.push_varstr32(rawMessage.data(), rawMessage.length());

	}
	virtual void unmarshal(const sox::Unpack &pak) {
		pak >> ip;
		rawMessage = pak.pop_varstr32();
	}
};

struct PGetMicrophoneList : public sox::Marshallable{
	enum{uri = (819 << 8 | protocol::SESSION_SVID)};
	uint32_t topSid;
	uint32_t curSid;
	uint32_t uid;
	PGetMicrophoneList(uint32_t pTopSid = 0, uint32_t pCurSid = 0, uint32_t pUid = 0)
		:topSid(pTopSid), curSid(pCurSid), uid(pUid){
	}
	virtual void marshal(sox::Pack &pak) const {
		pak << topSid << curSid << uid;
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		pak >> topSid >> curSid >> uid;
	}
};

struct PGetMediaProxyInfo : public sox::Marshallable {

	enum{uri = (820 << 8 | protocol::SESSION_SVID)};
	uint32_t uid;
	virtual void marshal(sox::Pack &p) const {
		p << uid;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid;
	}
};

struct POnGetMediaProxyInfo : public sox::Marshallable {
	enum{uri = (821 << 8 | protocol::SESSION_SVID)};
	std::vector<IpInfo> mediaProxyInfo;
	void marshal(sox::Pack &p) const{
		sox::marshal_container(p, mediaProxyInfo);;
	}

	void unmarshal(const sox::Unpack &up){
		sox::unmarshal_container(up, std::back_inserter(mediaProxyInfo));
	}
};

struct PSessionDirect4: public sox::Marshallable{
	enum {uri = (822 << 8) | protocol::SESSION_SVID };
	uint32_t sid;
	uint32_t asid;
	std::vector<IpInfo> signalProxyInfo;
	void marshal(sox::Pack &p) const{
		p << sid << asid;
		sox::marshal_container(p, signalProxyInfo);
	}

	void unmarshal(const sox::Unpack &up){
		up >> sid >> asid;
		sox::unmarshal_container(up, std::back_inserter(signalProxyInfo));
	}
};

struct PGetSessionInfo : public sox::Marshallable {
	enum{uri = (823 << 8 | protocol::SESSION_SVID)};
	uint32_t sid;
	uint32_t uid;
	PGetSessionInfo(uint32_t pSid = 0, uint32_t pUid = 0) : sid(pSid), uid(pUid){}
	virtual void marshal(sox::Pack &p) const {
		p << sid << uid;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> sid >> uid;
	}
};

struct PGetSignalProxyInfo : public sox::Marshallable {
	enum{uri = (824 << 8 | protocol::SESSION_SVID)};
	uint32_t uid;
	virtual void marshal(sox::Pack &p) const {
		p << uid;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid;
	}
};

struct POnGetSignalProxyInfo: public sox::Marshallable {
	enum{uri = (825 << 8) | protocol::SESSION_SVID};
	std::vector<IpInfo> signalProxyInfo;
	void marshal(sox::Pack &p) const{
		sox::marshal_container(p, signalProxyInfo);;
	}
	void unmarshal(const sox::Unpack &up){
		sox::unmarshal_container(up, std::back_inserter(signalProxyInfo));
	}
};

struct PLoginMediaProxyTcp : public PLoginMediaProxy {
  enum{uri = (826 << 8 | protocol::SESSION_SVID)};
};

struct PLoginMediaProxyUdp : public PLoginMediaProxy {
  enum{uri = (827 << 8 | protocol::SESSION_SVID)};
};

struct PLoginMediaProxyTcpRes : public PLoginMediaProxyRes {
  enum{uri = (828 << 8 | protocol::SESSION_SVID)};
};

struct PLoginMediaProxyUdpRes : public PLoginMediaProxyRes {
  enum{uri = (829 << 8 | protocol::SESSION_SVID)};
};

struct PDoubleVoice: public sox::Marshallable{
	enum{uri = (830 << 8 | protocol::SESSION_SVID)};
	bool start;
	virtual void marshal(sox::Pack &p) const{
		p << start;
	}
	virtual void unmarshal(const sox::Unpack &up){
		up >> start;
	}
};

struct PLoginSignalProxy2 : public PLoginSignalProxy {
	enum{uri = (831 << 8 | protocol::SESSION_SVID)};

  uint32_t loginMode; // 0 - 正常登陆模式，1 - 重连模式(已有用户信息), 9 - 旁路测试模式
  uint32_t imid;      // yy号
  std::string ticket;
  std::string antiRes;
  uint32_t sessGroupId;
  std::string clientReleaseVer;   //客户端发布版本, e.g.. 5.7.0.0.
  uint8_t  clientType;            //0 - pc, 1 - mobile
  uint32_t lcid;                  //zh-CN: 2052; zh-TW: 1028; en-US: 1033; pt-BR: 1046

  PLoginSignalProxy2() : loginMode(0),imid(0),sessGroupId(0),clientType(0),lcid(0) {}

  virtual void marshal(sox::Pack &p) const 
  {
    PLoginSignalProxy::marshal(p);
    p << loginMode;
    p << imid;
    p << ticket;
    p << antiRes;
    p << sessGroupId;
    p << clientReleaseVer;
    p << clientType;
    p << lcid;
  }
  virtual void unmarshal(const sox::Unpack &up) 
  {
    PLoginSignalProxy::unmarshal(up);
    if( !up.empty() )
    {
      up >> loginMode;
    }
    if( !up.empty() )
    {
      up >> imid;
    }
    if( !up.empty() )
    {
      up >> ticket;
    }
    if( !up.empty() )
    {
      up >> antiRes;
    }
    if( !up.empty() )
    {
      up >> sessGroupId;
    }
    if(!up.empty())
    {
      up >> clientReleaseVer;
    }
    if( !up.empty() )
    {
      up >> clientType;
    }
    if( !up.empty() )
    {
      up >> lcid;
    }
  }	
};

typedef ZipMarshal<PGetChannelInfoRes, 832 << 8 | protocol::SESSION_SVID> PGetChannelInfoZipRes;

struct PChangeMediaProxy: public sox::Voidmable{
	enum{uri = (833 << 8 | protocol::SESSION_SVID)};
};

struct PSerialPack2 : public sox::Marshallable {
	enum{uri = (834 << 8 | protocol::SESSION_SVID)};
	uint32_t serial;
	std::string data;
	virtual void marshal(sox::Pack &p) const {
		p << serial;
		p.push_varstr32(data.data(), data.length());
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> serial;
		data = up.pop_varstr32();
	}
};

struct PForceSingleVoice: public sox::Marshallable{
	enum{uri = (836 << 8 | protocol::SESSION_SVID)};
	bool force;
	virtual void marshal(sox::Pack &p) const{
		p << force;
	}
	virtual void unmarshal(const sox::Unpack &up){
		up >> force;
	}
};

struct PForceConnectMediaProxy: public sox::Marshallable{
	enum{uri = (837 << 8 | protocol::SESSION_SVID)};
	std::vector<IpInfo> mediaProxyInfo;
	void marshal(sox::Pack &p) const{
		sox::marshal_container(p, mediaProxyInfo);;
	}

	void unmarshal(const sox::Unpack &up){
		sox::unmarshal_container(up, std::back_inserter(mediaProxyInfo));
	}
};

enum ProxyParameterKey{LOSS_RATE_FOR_DOUBLE, LOSS_RATE_FOR_SINGLE};
struct PSetProxyParameter : public sox::Marshallable{
	enum{uri = (838 << 8 | protocol::SESSION_SVID)};
	std::map<uint32_t , uint32_t> proxyParameterMap;
	virtual void marshal(sox::Pack &p) const {
		sox::marshal_container(p, proxyParameterMap);
	}
	virtual void unmarshal(const sox::Unpack &p) {
		sox::unmarshal_container(p, std::inserter(proxyParameterMap, proxyParameterMap.begin()));
	}
};



//======================================================================================
//xmp: 拆分用户进入频道过程的协议

//进入频道：替换原PJoinSession2_40，与其区别为：服务器应答的PPushSessData包含不完全用户列表，后面紧跟PPushPartnerIDs
//Client=>Session, Client=>Proxy
struct PJoinSession3_00 : public PJoinSession2_40
{
	enum{uri = (850 << 8 | protocol::SESSION_SVID)};
};

//服务拒绝客户端进入：在服务过载状态下，收到PJoinSession3_00时，作出此应答
//Proxy=>Client
struct PDenyAccessSession : public sox::Marshallable
{
	enum{uri = (851 << 8 | protocol::SESSION_SVID)};
	uint32_t req_uri;
	uint32_t req_seq; //-1: invalid seq
	uint32_t reason;
	uint32_t retry_delay; //unit is sec.  //0: random delay, -1: don't retry
	virtual void marshal(sox::Pack &pak) const {
		pak << req_uri;
		pak << reason;
		pak << retry_delay;
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		pak >> req_uri;
		pak >> reason;
		pak >> retry_delay;
	}
};

/*
//客户端查询频道内用户列表：此为新增逻辑，参见PJoinSession3_00说明
//Proxy=>Client
//注：此协议暂不使用！
struct PQuerySessData : public sox::Marshallable
{
	enum{uri = (852 << 8 | protocol::SESSION_SVID)};
	uint32_t uid;
	uint32_t sid;
	uint32_t topSid;
	virtual void marshal(sox::Pack &pak) const {
		pak << uid;
		pak << sid;
		pak << topSid;
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		pak >> uid;
		pak >> sid;
		pak >> topSid;
	}
};
*/

//服务器告知客户端用户列表：在3_00中，PushSessData包含不完全用户列表，后面紧跟PPushPartnerIDs。后续客户端可使用PQuerySpecPartnerInfos来查询用户详细信息
//Proxy=>Client
struct PPushPartnerIDs : public sox::Marshallable
{
	enum {uri = (853 << 8) | protocol::SESSION_SVID};
	std::map<uint32_t, std::vector<uint32_t> > partners; //map<sid, vector<uid> >
	virtual void marshal(sox::Pack &pak) const {
		pak.push_uint32((uint32_t)partners.size());
		for (std::map<uint32_t, std::vector<uint32_t> >::const_iterator it = partners.begin(); it != partners.end(); ++it)
		{
			pak.push_uint32((*it).first);
			marshal_container(pak, (*it).second);
		}
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		uint32_t ch_cnt = pak.pop_uint32();
		for (uint32_t count = ch_cnt; count > 0; --count)
		{
			uint32_t sid = pak.pop_uint32();
			unmarshal_container(pak, std::back_inserter(partners[sid]));
		}
	}
};

//客户端向服务器查询指定用户详细信息
//Client=>Proxy
struct PQuerySpecPartnerInfos : public sox::Marshallable
{
	enum {uri = (854 << 8) | protocol::SESSION_SVID};
	uint32_t uid;
	uint32_t sid;
	uint32_t topSid;

	uint32_t seq; //请求的本地唯一序列号，服务器在应答中带回

	//通常，以下三个集合每次只用一个，另外两个置空
	uint32_t mode; //1: uid列表, 2: uid范围列表，每两项作为一个rg, 3: 子频道(sid+uid_rg)列表，每三项为一组
	std::vector<uint32_t> spec_ids;

	//不必返回详细信息的用户uid列表
	std::set<uint32_t> except_uids;

	virtual void marshal(sox::Pack &pak) const {
		pak << uid;
		pak << sid;
		pak << topSid;
		pak << seq;
		pak << mode;
		marshal_container(pak, spec_ids);
		marshal_container(pak, except_uids);
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		pak >> uid;
		pak >> sid;
		pak >> topSid;
		pak >> seq;
		pak >> mode;
		unmarshal_container(pak, std::back_inserter(spec_ids));
		unmarshal_container(pak, std::inserter(except_uids, except_uids.end()));
	}
};

//服务器应答客户端指定用户详细信息
//Proxy=>Client
struct PQuerySpecPartnerInfosRes : public PPushSessData
{
	enum {uri = (855 << 8) | protocol::SESSION_SVID};
	uint32_t req_seq; //传回请求包中的序列号
	virtual void marshal(sox::Pack &pak) const {
		PPushSessData::marshal(pak);
		pak << req_seq;
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		PPushSessData::unmarshal(pak);
		pak >> req_seq;
	}
};


//预先通知Proxy新用户的状态信息，在PJoinSessionRes之前传递
//Session=>Proxy
struct PPreNoticeJoinUserStat : public sox::Marshallable {
	enum {uri = (856 << 8) | protocol::SESSION_SVID};
	uint32_t uid;
	uint32_t sid;
	uint32_t topSid;
	uint32_t new_sid;
	virtual void marshal(sox::Pack &pak) const {
		pak << uid << sid << topSid << new_sid;
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		pak >> uid >> sid >> topSid >> new_sid;
	}
};

struct PUserMsg3 : public sox::Marshallable
{
 enum {uri = (10100 << 8 | protocol::SESSION_SVID)};
 std::vector<uint32_t> uids;  //指定要发送到的用户ID
 std::vector<uint32_t> toSids;  //从此频道开始遍历广播到子频道的频道ID  
 uint32_t toSidsOnly;  //只广播给该频道的UID, 不包含其子频道
 uint64_t proxyId;
 uint32_t reserve1;
 uint32_t reserve2;
 uint16_t dataType; //数据类型, 0为普通数据, 1为开放平台使用的数据
 std::string data;
 virtual void marshal(sox::Pack &p) const {
	marshal_container(p, uids);
	marshal_container(p, toSids);
	p << proxyId << toSidsOnly << dataType << reserve1 << reserve2;
	p.push_varstr32(data.data(), data.length()); // 这里string的头是4B
 }
 virtual void unmarshal(const sox::Unpack &p) {
	unmarshal_container(p, std::back_inserter(uids));
	unmarshal_container(p, std::back_inserter(toSids));
	p >> proxyId >> toSidsOnly >> dataType >> reserve1 >> reserve2;
	data = p.pop_varstr32();                 // 这里string的头是4B
 }
};
// Support online diagnostics tool
struct PGetDiagnosticData : public sox::Marshallable {
	enum{uri = (859 << 8 | protocol::SESSION_SVID)};
	uint32_t uid;
	virtual void marshal(sox::Pack &p) const {
		p << uid ;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid;
	}
};

//======================================================================================
struct PDropUserMedia : public sox::Marshallable
{
  enum{uri = (860 << 8 | protocol::SESSION_SVID)};
  uint32_t uid; 
  virtual void marshal(sox::Pack &pak) const {
    pak << uid;
  }
  virtual void unmarshal(const sox::Unpack &pak) {
    pak >> uid;
  }
};

struct PGetDiagnosticDataRes : public sox::Marshallable {
	enum{uri = (857 << 8 | protocol::SESSION_SVID)};
    uint32_t uid;
	std::string diagnoData;
	virtual void marshal(sox::Pack &p) const {
		p << uid << diagnoData ;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >>  uid >> diagnoData;
	}
};

// VersionItem and AppInfoPack are just portions of one message.
struct VersionItem : public sox::Marshallable
{
	uint32_t version;
	uint32_t yyMin;
	uint32_t yyMax;
	std::string md5;
	std::string url;
    virtual void marshal(sox::Pack &p) const 
    {
        p << version << yyMin << yyMax << md5 << url;
    }
    virtual void unmarshal(const sox::Unpack &p) 
    {
        p >> version >> yyMin >> yyMax >> md5 >> url;
    }
};  

struct AppInfoPack : public sox::Marshallable
{
	uint32_t appID;
    uint64_t proxyID; // the proxyID of the AppServer that serves this App.
	std::string appFileName;	//music.dll
	std::string appDescription;	// "投票""抢板凳"
	uint32_t recommendedAppVersion; //推荐使用的插件版本
	std::vector<VersionItem> versionSet;
	bool opByOw;  // OW created the APP, this event need to be displayed.

    virtual void marshal(sox::Pack &p) const 
    {
        p << appID << proxyID << appFileName << appDescription << recommendedAppVersion;
        sox::marshal_container(p, versionSet);
		p << opByOw;
    }
    virtual void unmarshal(const sox::Unpack &p) 
    {
        p >> appID >> proxyID >> appFileName >> appDescription >> recommendedAppVersion;
        sox::unmarshal_container(p, std::back_inserter(versionSet));
		p >> opByOw;
    }

}; //


// define a version 2 message so server can interpret both ping messsages.
struct PMediaProxyPing2 : public sox::Marshallable {
	enum {uri = (858 << 8 | protocol::SESSION_SVID)};
	uint32_t uid;
	uint32_t sid;
	uint32_t stampc;
	uint32_t loginStamp;
    uint32_t clientRtt;
	PMediaProxyPing2(uint32_t pSid = 0, uint32_t pUid = 0, uint32_t pLoginStamp = 0, 
                    uint32_t pStampc = 0, uint32_t cRtt = 0)
		:uid(pUid), sid(pSid), stampc(pStampc), loginStamp(pLoginStamp), clientRtt(cRtt){
	}

	virtual void marshal(sox::Pack &pak) const {
		pak << uid << sid << stampc << loginStamp << clientRtt;
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		pak >> uid >> sid >> stampc >> loginStamp >> clientRtt;
	}
};

struct PNotifyVoicePkgNum: public sox::Marshallable {
	enum{uri = (860 << 8 | protocol::SESSION_SVID)};
	uint32_t sendCount;
	uint32_t uid;
	virtual void marshal(sox::Pack &pak) const {
		pak << sendCount << uid;
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		pak >> sendCount >> uid;
	}
};

struct PNotifyVoicePkgNum2: public PNotifyVoicePkgNum {
	enum{uri = (862 << 8 | protocol::SESSION_SVID)};
	uint32_t voicePlayNum;
	uint32_t voiceLossNum;
	uint32_t voiceSendNum;
	virtual void marshal(sox::Pack &pak) const {
		PNotifyVoicePkgNum::marshal(pak);
		pak << voicePlayNum << voiceLossNum << voiceSendNum;
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		PNotifyVoicePkgNum::unmarshal(pak);
		pak >> voicePlayNum >> voiceLossNum >> voiceSendNum;
	}
};

struct PNotifyVoicePkgLoss: public sox::Marshallable {
	enum{uri = (861 << 8 | protocol::SESSION_SVID)};
	uint32_t sendCount;
	uint32_t recvCount;
	uint32_t timeStamp;
	uint32_t uid;
	virtual void marshal(sox::Pack &pak) const {
		pak << sendCount << recvCount << timeStamp << uid;
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		pak >> sendCount >> recvCount >> timeStamp >> uid;
	}
};

struct PNotifyVoicePkgLoss2: public PNotifyVoicePkgLoss
{
	enum{uri = (863 << 8 | protocol::SESSION_SVID)};
	uint32_t voiceSendCount;
	uint32_t voiceRecvCount;
	virtual void marshal(sox::Pack& pak) const {
		PNotifyVoicePkgLoss::marshal(pak);
		pak << voiceSendCount << voiceRecvCount;
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		PNotifyVoicePkgLoss::unmarshal(pak);
		pak >> voiceSendCount >> voiceRecvCount;
	}
};

struct PCustomMessage : public sox::Marshallable{
  enum {uri = (900 << 8) | SESSION_SVID};
  uint32_t fromUid;
  uint32_t toUid;
  std::string msg;
  virtual void marshal(sox::Pack &pak) const {
    pak << fromUid << toUid << msg;
  }
  virtual void unmarshal(const sox::Unpack &unpak) {
    unpak >> fromUid >> toUid >> msg;
  }
};


struct LossRateReport : public sox::Marshallable
{
  uint64_t proxyId;
  uint32_t sendCount;
  uint32_t recvCount;    
    
  virtual void marshal(sox::Pack &p) const {
    p << proxyId << sendCount << recvCount;
  }

  virtual void unmarshal(const sox::Unpack &p) {
    p >> proxyId >> sendCount >> recvCount;
  }
};

//路由丢包率统计
struct PLossRateReports : public sox::Marshallable
{
  enum{uri = (253 << 8 | protocol::SESSION_SVID)};

  uint32_t fromUid;
  uint32_t sid;
  uint32_t timeStamp;
  std::vector<LossRateReport> lossRateReports;
    
  virtual void marshal(sox::Pack &p) const {
    p << fromUid << sid << timeStamp;
	sox::marshal_container(p, lossRateReports);
  }

  virtual void unmarshal(const sox::Unpack &p) {
    p >> fromUid >> sid >> timeStamp;
	sox::unmarshal_container(p, std::back_inserter(lossRateReports));
  }
};

//申请开双路媒体前端
struct PApplyMediaDouble : public sox::Marshallable
{
	enum{uri = (251 << 8 | protocol::SESSION_SVID)};

	uint32_t sid;
	uint32_t uid;
	uint32_t mainLossRate;    

	virtual void marshal(sox::Pack &p) const {
		p << sid << uid << mainLossRate;
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> sid >> uid >> mainLossRate;
	}
};

//申请开双路媒体前端回复
struct PApplyMediaDoubleRes : public sox::Marshallable
{
	enum{uri = (252 << 8 | protocol::SESSION_SVID)};

	uint8_t  permit; //0: 不允许，1：允许   

	virtual void marshal(sox::Pack &p) const {
		p.push_uint8(permit);
	}

	virtual void unmarshal(const sox::Unpack &p) {
		permit = p.pop_uint8();
	}
};



//======================================================================================

//群系统获取频道长号/短号
//requestId 待查询的长号或短号
//m_bGetRealID为true时: 获取频道长号
//m_mapChID的key为频道短/长号，将频道的长号填为resultId返回。如果频道不存在，resultId填0
//m_bGetRealID为false时: 获取频道短号
//m_mapChID的key为频道短/长号，将频道的短号填为resultId返回。如果频道不存在，resultId填0
struct PCS_GetChannelRealID : public sox::Marshallable
{
	enum {uri = (214 << 8 | protocol::SDB_SVID)};

	typedef std::map<uint32_t, uint32_t> ChIDMapType;

	uint32_t requestId;
	uint32_t resultId;
	uint32_t m_uAppURI;
	std::string m_strAppCtx;	
	bool m_bGetRealID; //true: 获取长号； false: 获取短号

	PCS_GetChannelRealID () : requestId(0), resultId(0)
	{
		m_uAppURI = 0;
		m_bGetRealID = true;
	}
	
	PCS_GetChannelRealID(uint32_t req, uint32_t uUri = 0) :
		 requestId(req), m_uAppURI(uUri), m_bGetRealID(true)
	{
	}

	virtual void marshal(sox::Pack &p) const
	{
		p << requestId << resultId << m_uAppURI << m_strAppCtx << m_bGetRealID;
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> requestId >> resultId >> m_uAppURI >> m_strAppCtx >> m_bGetRealID;
	}
};

struct PCS_GetChannelRealIDRes : public PCS_GetChannelRealID
{
	enum {uri = (215 << 8 | protocol::SDB_SVID)};
};

struct PCS_GetMyChannelList : public sox::Marshallable
{
	enum {uri = (216 << 8 | protocol::SDB_SVID)};


	virtual void marshal(sox::Pack& /*p*/) const
	{
	}

	virtual void unmarshal(const sox::Unpack& /*p*/)
	{
	}
};


struct PCS_GetMyChannelListRes : public sox::Marshallable
{
	enum {uri = (217 << 8 | protocol::SDB_SVID)};
	
	//返回结果: key是长位ID, value是短位ID
	typedef std::map<uint32_t, uint32_t> ChIDMapType;
	
	ChIDMapType m_mapChID;

	virtual void marshal(sox::Pack &p) const
	{
		sox::marshal_container(p, m_mapChID);
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		sox::unmarshal_container(p, std::inserter(m_mapChID,
			m_mapChID.begin()));
	}

};

//=========================================================================
//==========================V4版本消息定义=================================
struct user_data : public sox::Marshallable {
	uint32_t id;
	std::string data;
	virtual void marshal(sox::Pack &pk) const {
		pk << id << data;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> id >> data;
	}
};

// Client protocol
struct PServiceReq : public sox::Marshallable {
	enum {uri = (11 << 8 | protocol::SERVICE_SVID)};
	uint32_t uid;
	uint32_t sid;
	uint32_t serviceType;
	PServiceReq(uint32_t pUid = 0, uint32_t pSid = 0, uint32_t pType = 0) :
				uid(pUid), sid(pSid), serviceType(pType){}
	virtual void marshal(sox::Pack &pk) const {
		pk << uid << sid << serviceType;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid >> sid >> serviceType;
	}
};

struct PServiceReqByUids : public sox::Marshallable {
	enum {uri = (12 << 8 | protocol::SERVICE_SVID)};
	uint32_t uid;
	uint32_t serviceType;
	std::vector<uint32_t> uids;
	virtual void marshal(sox::Pack &pk) const {
		pk << uid << serviceType;
		sox::marshal_container(pk, uids);
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid >> serviceType;
		sox::unmarshal_container(up, std::back_inserter(uids));
	}
};

struct PWdReqByUids : public sox::Marshallable
{
	enum {uri = (22 << 8 | protocol::SERVICE_SVID)};
	uint32_t sid;
	uint32_t uid;
	uint32_t serviceType;
	std::vector<uint32_t> uids;
	virtual void marshal(sox::Pack &pk) const {
		pk << sid << uid << serviceType;
		marshal_container(pk, uids);
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> sid >> uid >> serviceType;
		unmarshal_container(up, std::back_inserter(uids));
	}
};


struct PUpdateReq : public sox::Marshallable {
	enum {uri = (13 << 8 | protocol::SERVICE_SVID)};
	uint32_t sid;
	uint32_t uid;
	std::string data;
	uint32_t serviceType;
	PUpdateReq(uint32_t pSid = 0, uint32_t pUid = 0, std::string pData = "", uint32_t pType = 0)
			  : sid(pSid), uid(pUid), data(pData), serviceType(pType){}
	virtual void marshal(sox::Pack &pk) const {
		pk << sid << uid << data << serviceType;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> sid >> uid >> data >> serviceType;
	}
};

struct PUpdateRes : public sox::Marshallable {
	enum {uri = (14 << 8 | protocol::SERVICE_SVID)};
	virtual void marshal(sox::Pack& /*pk*/) const
  {
	}
	virtual void unmarshal(const sox::Unpack& /*up*/)
  {
	}
};

struct PServiceRes : public sox::Marshallable {
	enum {uri = (15 << 8 | protocol::SERVICE_SVID)};
	uint32_t serviceType;
	std::vector<user_data> data;
	std::vector<uint32_t> failedUids;
	virtual void marshal(sox::Pack &pk) const {
		pk << serviceType;
		sox::marshal_container(pk, data);
		sox::marshal_container(pk, failedUids);
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> serviceType;
		sox::unmarshal_container(up, std::back_inserter(data));
		sox::unmarshal_container(up, std::back_inserter(failedUids));
	}
};

struct PUserMessage4: public PUserMessage {
	enum {uri = (31 << 8) | protocol::SERVICE_SVID};
	uint32_t sid;
	uint32_t from;
	uint32_t serviceType;
	virtual void marshal(sox::Pack &pak) const {
		PUserMessage::marshal(pak);
		pak << sid << from << serviceType;
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		PUserMessage::unmarshal(pak);
		pak >> sid >> from >> serviceType;
	}
};

enum ServiceType{WEIDENG = 1, JIFEN = 2, GONGGAO = 3, ADREPORT = 4};
struct PGetServiceAddrList: public sox::Marshallable {
	enum {uri = (32 << 8 | protocol::SERVICE_SVID)};

	uint32_t uid;
	bool isAll;
	ServiceType serviceType;
	PGetServiceAddrList(uint32_t pUid = 0, bool pIsAll = false, ServiceType pServiceType = WEIDENG)
		: uid(pUid)
		, isAll(pIsAll)
		, serviceType(pServiceType)
		{}
	virtual void marshal(sox::Pack &pk) const {
		pk << uid << isAll;
		pk.push_uint16(serviceType);
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid >> isAll;
		serviceType = (ServiceType)up.pop_uint16();
	}
};

struct PServiceAddr: public sox::Marshallable {
	enum{uri = (33 << 8 | protocol::SERVICE_SVID)};
	uint32_t serviceIp;
	std::vector<uint16_t> servicePort;
	virtual void marshal(sox::Pack &pak) const {
		pak << serviceIp;
		sox::marshal_container(pak, servicePort);
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		pak >> serviceIp;
		sox::unmarshal_container(pak, std::back_inserter(servicePort));
	}
};

struct PServiceAddrList: public sox::Marshallable {
	enum{uri = (34 << 8 | protocol::SERVICE_SVID)};
	std::vector<PServiceAddr> cncIpList;
	std::vector<PServiceAddr> ctlIpList;
	virtual void marshal(sox::Pack &pak) const {
		sox::marshal_container(pak, cncIpList);
		sox::marshal_container(pak, ctlIpList);
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		sox::unmarshal_container(pak, std::back_inserter(cncIpList));
		sox::unmarshal_container(pak, std::back_inserter(ctlIpList));
	}
};

struct PType2ServiceAddr: public sox::Marshallable {
	enum{uri = (35 << 8 | protocol::SERVICE_SVID)};

	std::map<uint32_t, PServiceAddrList> type2ServiceAddrMap;

	virtual void marshal(sox::Pack &p) const {
		sox::marshal_container(p, type2ServiceAddrMap);
	}
	virtual void unmarshal(const sox::Unpack &p) {
		sox::unmarshal_container(p, std::inserter(type2ServiceAddrMap, type2ServiceAddrMap.begin()));
	}
};

//公告解耦协议定义-hexk add begin
struct PBulletinServiceReq : public sox::Marshallable //向service查询公告
{
	enum {uri = (50 << 8 | protocol::SERVICE_SVID)};

	//发起查询的用户uid
	uint32_t uid; 
	//顶级频道ID
	uint32_t top_sid;
	//目标为顶级频道时为顶级频道ID
	uint32_t sub_sid;

	PBulletinServiceReq(uint32_t pUid = 0, uint32_t pTopSid = 0, uint32_t pSid = 0) :
						uid(pUid), top_sid(pTopSid), sub_sid(pSid){}
		
	virtual void marshal(sox::Pack &pk) const 
	{
		pk << uid << top_sid << sub_sid;
	}

	virtual void unmarshal(const sox::Unpack &up) 
	{
		up >> uid >> top_sid >> sub_sid;
	}
};

struct PBulletinServiceRes : public sox::Marshallable //service返回公告信息
{
	enum {uri = (51 << 8 | protocol::SERVICE_SVID)};
	
	//目标频道所在顶级频道ID
	uint32_t top_sid;
	//目标频道所在子频道ID，如果目标频道即为顶级频道，则该字段也设置为顶级频道ID
	uint32_t sub_sid; 
	//公告解压后的字符个数，0代表公告内容为空。
	uint32_t unzip_sz; 
	//目标频道对应的公告信息，压缩后的公告内容
	std::string zip_data; 

	virtual void marshal(sox::Pack &pk) const 
	{
		pk << top_sid << sub_sid << unzip_sz << zip_data;
	}
	
	virtual void unmarshal(const sox::Unpack &up) 
	{
		up >> top_sid >> sub_sid >> unzip_sz >> zip_data;
	}
};

enum ProxyPropertyKey{RELAY_THRESHOLD, MAX_SERVICE_MEDIA, MAX_SERVICE_SIGNAL};
struct PProxyProperty: public sox::Marshallable
{
  enum{uri = (2071 << 8 | protocol::SESSION_SVID)};
  std::map<uint32_t, uint32_t> prop;
  virtual void marshal(sox::Pack &pk) const
  {
    sox::marshal_container(pk, prop);
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    sox::unmarshal_container(up, std::inserter(prop, prop.begin()));
  }
};

struct PGetMaixuList: public sox::Marshallable
{
    enum{uri = (2072 << 8 | protocol::SESSION_SVID)};

    uint32_t                tid;
    uint32_t                sid;

    virtual void marshal(sox::Pack &p) const
    {
        p << tid << sid;
    }
    virtual void unmarshal(const sox::Unpack &p)
    {
        p >> tid >> sid;
    }
};


struct PGetMaixuListRes: public PMaixuJoin
{
    enum{uri = (2073 << 8 | protocol::SESSION_SVID)};

    uint32_t                tid;

    virtual void marshal(sox::Pack &p) const
    {
        PMaixuJoin::marshal(p);
        p << tid;
    }

    virtual void unmarshal(const sox::Unpack &p)
    {
        PMaixuJoin::unmarshal(p);
        p >> tid;
    }
};


struct PSyncSignalUser: public sox::Marshallable
{
  enum{uri = (2074 << 8 | protocol::SESSION_SVID)};
  std::map<uint32_t, std::set<uint32_t> > userlist;
  virtual void marshal(sox::Pack &pk) const
  {
    pk.push_uint32(userlist.size());
    std::map<uint32_t, std::set<uint32_t> >::const_iterator it;
    for(it = userlist.begin();it != userlist.end();++it)
    {
      pk.push_uint32(it->first);
      sox::marshal_container(pk, it->second);
    }
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    for(uint32_t i = up.pop_uint32();i > 0;--i)
    {
      uint32_t key = up.pop_uint32();
      std::set<uint32_t>& uids = userlist[key];
      sox::unmarshal_container(up, std::inserter(uids, uids.begin()));
    }
  }
};

struct PGetProxyInfo: public sox::Marshallable
{
  enum{uri = (2075 << 8 | protocol::SESSION_SVID)};
  uint64_t proxyId;
  virtual void marshal(sox::Pack &pk) const
  {
    pk << proxyId;
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> proxyId;
  }
};

typedef ZipMarshal<PSyncChannelRunTimeInfoRes, 2076 << 8 | protocol::SESSION_SVID> PSyncChannelRunTimeInfoZipRes;

struct PSessionDirect5 : public sox::Marshallable
{
  enum{uri = (20005 << 8 | protocol::SESSION_SVID)};
  uint32_t uid;
  PSessionDirect4 psd4;
  void marshal(sox::Pack &p) const{
	p << uid;
	psd4.marshal(p);
  }
  void unmarshal(const sox::Unpack &up){
	up >> uid;
	psd4.unmarshal(up);
  }
};

struct PListenPort: public sox::Marshallable{
 enum{uri = (90 << 8 | protocol::SESSION_SVID)};
 uint32_t ip;
 uint16_t tcp[4];
 uint16_t udp[4];
 virtual void marshal(sox::Pack &p) const {
  p << ip;
  for(int i=0;i<4;i++)p<<tcp[i];
  for(int i=0;i<4;i++)p<<udp[i];
 }
 virtual void unmarshal(const sox::Unpack &up) {
  up >> ip;
  for(int i=0;i<4;i++)up>>tcp[i];
  for(int i=0;i<4;i++)up>>udp[i];
 }
};

struct PListenPortRes: public sox::Marshallable{
 enum{uri = (91 << 8 | protocol::SESSION_SVID)};
 uint32_t proxy_id;
 virtual void marshal(sox::Pack &p) const {
  p << proxy_id;
 }
 virtual void unmarshal(const sox::Unpack &up) {
  up >> proxy_id;
 }
};

struct PListenPort2: public PListenPort{
  enum{uri = (184 << 8 | protocol::SESSION_SVID)};
};

struct PTcpLoginFromProxy : public sox::Marshallable {
	enum {uri = (141 << 8 | protocol::SESSION_SVID)};
	uint32_t uid;
	uint32_t sid;
  uint32_t proxy_id;
  std::string cookie;
	virtual void marshal(sox::Pack &pak) const {
		pak << uid << cookie << sid << proxy_id;
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		pak >> uid >> cookie >> sid >> proxy_id;
	}
};

//======================================================================================
struct PBindProxyMedia : public sox::Marshallable
{
  enum{uri = (300 << 8 | protocol::SESSION_SVID)};
  uint32_t proxyId;
  uint32_t reserver1;
  uint32_t reserver2;
  virtual void marshal(sox::Pack &pak) const {
    pak << proxyId << reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &pak) {
    pak >> proxyId >> reserver1 >> reserver2;
  }
};

//======================================================================================
struct PBindProxyMediaRes : public sox::Marshallable
{
  enum{uri = (301 << 8 | protocol::SESSION_SVID)};
  uint32_t reserver1;
  uint32_t reserver2;
  virtual void marshal(sox::Pack &pak) const {
    pak << reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &pak) {
    pak >> reserver1 >> reserver2;
  }
};
//======================================================================================
struct PProxyMediaPing : public sox::Marshallable
{
  enum{uri = (302 << 8 | protocol::SESSION_SVID)};
  uint32_t proxyId;
  uint32_t reserver1;
  uint32_t reserver2;
  virtual void marshal(sox::Pack &pak) const {
    pak << proxyId << reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &pak) {
    pak >> proxyId >> reserver1 >> reserver2;
  }
};

//======================================================================================
struct PProxyMediaPingRes : public sox::Marshallable
{
  enum{uri = (303 << 8 | protocol::SESSION_SVID)};
  uint32_t reserver1;
  uint32_t reserver2;
  virtual void marshal(sox::Pack &pak) const {
    pak << reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &pak) {
    pak >> reserver1 >> reserver2;
  }
};

//======================================================================================
struct PProxyException: public sox::Marshallable
{
  enum{ uri = (502 << 8 | protocol::SESSION_SVID ) };
  ExceptionReason reason;
  uint32_t proxy_id;
  virtual void marshal( sox::Pack& pk ) const
  {
    pk.push_uint16( reason );
    pk << proxy_id;
  }
  virtual void unmarshal( const sox::Unpack& up )
  {
    reason = (ExceptionReason)up.pop_uint16();
    up >> proxy_id;
  }
};

struct PProxyRecover: public sox::Marshallable
{
  enum{ uri = (503 << 8 | protocol::SESSION_SVID ) };
  ExceptionReason reason;
  uint32_t proxy_id;
  virtual void marshal( sox::Pack& pk ) const
  {
    pk.push_uint16( reason );
    pk << proxy_id;
  }
  virtual void unmarshal( const sox::Unpack& up )
  {
    reason = (ExceptionReason)up.pop_uint16();
    up >> proxy_id;
  }
};

//======================================================================================
struct PBroadcastVoice3: public PBroadcastVoice2{
	enum{uri = (1200 << 8 | protocol::SESSION_SVID)};
};

//======================================================================================
struct PStartBroadcastVoice: public sox::Marshallable {
	enum{uri = (1202 << 8 | protocol::SESSION_SVID)};
	uint32_t uid;
	uint32_t sid;
	std::vector<uint32_t> subSids;

	virtual void marshal(sox::Pack &p) const {
		p << uid << sid;
		sox::marshal_container(p, subSids);
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid >> sid;
		sox::unmarshal_container(up, std::back_inserter(subSids));
	}
};

//======================================================================================
struct PStopBroadcastVoice: public sox::Marshallable {
	enum{uri = (1203 << 8 | protocol::SESSION_SVID)};
	uint32_t uid;

	virtual void marshal(sox::Pack &p) const {
		p << uid;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid;
	}
};

//======================================================================================
struct PBroadcastText3: public PBroadcastText2{
	enum{uri = (1201 << 8 | protocol::SESSION_SVID)};
	std::vector<uint32_t> subSids;
	virtual void marshal(sox::Pack &p) const {
		PBroadcastText2::marshal(p);
		sox::marshal_container(p, subSids);
	}
	virtual void unmarshal(const sox::Unpack &up) {
		PBroadcastText2::unmarshal(up);
		sox::unmarshal_container(up, std::back_inserter(subSids));
	}
};


//##################################################################
//###################V4接偶长连接协议###############################
struct PServiceProxyAddr: public sox::Marshallable   //记录单个servcie proxy地址
{
	uint32_t serviceProxyIp;
	uint32_t users;
	std::vector<uint16_t> tcpPorts;
	std::vector<uint16_t>   udpPorts;
	bool operator < (const PServiceProxyAddr& dest) const{
		return (this->users < dest.users);
	}
	virtual void marshal(sox::Pack &pak) const {
		pak << serviceProxyIp  << users;
		marshal_container(pak, tcpPorts);
		marshal_container(pak, udpPorts);
	}
	virtual void unmarshal(const sox::Unpack &upak) {
		upak >> serviceProxyIp >> users;
		unmarshal_container(upak, std::back_inserter(tcpPorts));
		unmarshal_container(upak, std::back_inserter(udpPorts));

	}
};

struct PServiceProxyAddrList: public sox::Marshallable   //记录sercie proxy列表
{
	enum{uri = (300 << 8 | protocol::SERVICE_SVID)};
	uint8_t preferredTransfer;
	std::vector<PServiceProxyAddr> cncIpList;
	std::vector<PServiceProxyAddr> ctlIpList;
	std::map<uint16_t, std::pair<uint32_t, uint32_t> > serviceType2channels;
	virtual void marshal(sox::Pack &p) const {
		p.push_uint8(preferredTransfer);
		marshal_container(p, cncIpList);
		marshal_container(p, ctlIpList);
		marshal_container(p, serviceType2channels);
	}
	virtual void unmarshal(const sox::Unpack &up) {
		preferredTransfer = up.pop_uint8();
		unmarshal_container(up, std::back_inserter(cncIpList));
		unmarshal_container(up, std::back_inserter(ctlIpList));
		unmarshal_container(up, std::inserter(serviceType2channels, serviceType2channels.begin()));
	}
};

struct PULServiceMsg : public sox::Marshallable {
	enum{uri = (111 << 8 | protocol::SERVICE_SVID)};
	uint16_t serviceType;
	uint32_t sid;
	uint32_t uid;
	std::string msg;
	virtual void marshal(sox::Pack &pak) const {
		pak << serviceType << sid << uid << msg;
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		pak >> serviceType >> sid >> uid >> msg;
	}
};

struct PDLServiceMsgBySid : public sox::Marshallable  
{
	enum{uri = (112 << 8 | protocol::SERVICE_SVID)};
	uint16_t serviceType;
	uint32_t sid;
	std::string msg;
	virtual void marshal(sox::Pack &pak) const {
		pak << serviceType << sid << msg;
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		pak >> serviceType >> sid >> msg;
	}
};

struct PDLServiceMsgByUid : public sox::Marshallable  
{
  enum{uri = (113 << 8 | protocol::SERVICE_SVID)};
  uint16_t serviceType;
  uint32_t uid;
  std::string msg;

  uint64_t suid; // sessioning user id = suid, low 32 bit = uid(user id)
  virtual void marshal(sox::Pack &pak) const {
    pak << serviceType << uid << msg << suid;
  }
  virtual void unmarshal(const sox::Unpack &pak) {
    pak >> serviceType >> uid >> msg;
    if (!pak.empty())
    {
      pak >> suid;
    }
    else
    {
      suid = to64Uid(uid);
    }
  }
};

struct PLoginServiceProxy : public sox::Marshallable {
	enum{uri = (114 << 8 | protocol::SERVICE_SVID)};
	uint32_t uid;
	uint32_t sid;
	uint32_t keyver;
	uint32_t clientVersion;
	std::string cookies;
	/*PLoginServiceProxy(uint32_t pSid, uint32_t pUid, std::string& pCookies, uint32_t pKeyver) : uid(pUid), sid(pSid), cookies(pCookies), keyver(pKeyver){
	}*/
	virtual uint32_t getCookieKeyVersion(){return  keyver;}
	virtual void marshal(sox::Pack &p) const {
		p << uid << sid << keyver << clientVersion << cookies;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid >> sid >> keyver >> clientVersion >> cookies;
	}
};

struct PLoginServiceProxyRes : public sox::Marshallable {
	enum{uri = (115 << 8 | protocol::SERVICE_SVID)};
	uint32_t uid;
	uint32_t sid;
	virtual void marshal(sox::Pack &p) const {
		p << uid << sid;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid >> sid;
	}
};

struct PLeaveServiceProxy : public sox::Marshallable {
	enum{uri = (116 << 8 | protocol::SERVICE_SVID)};
	uint32_t uid;
	uint32_t sid;
	PLeaveServiceProxy(uint32_t pSid = 0, uint32_t pUid = 0) :
					   uid(pUid), sid(pSid){}

	virtual void marshal(sox::Pack &p) const {
		p << uid << sid;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid >> sid;
	}
};

struct PServiceProxyPing : public sox::Marshallable {
	enum {uri = (117 << 8 | protocol::SERVICE_SVID)};
	uint32_t uid;
	uint32_t sid;
	uint32_t stampc;
	uint32_t subSid;

  uint64_t suid; // sessioning user id = suid, low 32 bit = uid(user id)
	PServiceProxyPing(uint32_t pSid = 0, uint32_t pUid = 0, uint32_t pStampc = 0, uint32_t pSubSid = 0)
		:uid(pUid), sid(pSid), stampc(pStampc),subSid(pSubSid){
	}

	virtual void marshal(sox::Pack &pak) const {
		pak << uid << sid << stampc << subSid << suid;
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		pak >> uid >> sid >> stampc;
		if(!pak.empty())
		{
			pak >> subSid; 
		}
    else
    {
      subSid = 0;
    }
    if(!pak.empty())
    {
      pak >> suid;
    }
    else
    {
      suid = to64Uid(uid);
    }
	}
};

struct PServiceProxyPingRes : public sox::Marshallable {
	enum {uri = (118 << 8 | protocol::SERVICE_SVID)};
	uint32_t stampc;
	uint32_t stamps;

	virtual void marshal(sox::Pack &pk) const {
		pk << stampc << stamps;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> stampc >> stamps;
	}
};

//长连接平台,service与service proxy断开连接时，客户端会收到这条消息
struct PServiceClosed : public sox::Marshallable
{
	enum {uri = (163 << 8 | protocol::SERVICE_SVID)};
	uint16_t serviceType;
	uint32_t uid;
	uint32_t sid;
	virtual void marshal(sox::Pack &pak) const {
		pak << serviceType << uid << sid;
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		pak >> serviceType >> uid >> sid;
	}
};

struct PULServiceMsg2 : public sox::Marshallable {
	enum{uri = (311 << 8 | protocol::SERVICE_SVID)};
	uint16_t serviceType;
	uint32_t sid;
	uint32_t uid;
	std::string msg;
	virtual void marshal(sox::Pack &pak) const {
		pak << serviceType << sid << uid;
		pak.push_varstr32(msg.data(), msg.size());
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		pak >> serviceType >> sid >> uid;
		msg = pak.pop_varstr32();
	}
};

struct PULServiceMsgHash2 : public sox::Marshallable  
{
  enum { uri = ( 312 << 8 | protocol::SERVICE_SVID) };
  uint16_t serviceType;
  uint32_t sid;
  uint32_t uid;
  std::string msg;
  uint32_t uip;
  uint8_t	 terminalType;
  uint8_t  statType;

  uint32_t subsid; 

  uint64_t suid; // sessioning user id = suid, low 32 bit = uid(user id)

  enum { EXKEY_FROM = 1, EXKEY_END= 0xFFFFFFFF };
  //key = EXKEY_FROM, string = cookie::yyFrom 

  std::map<uint32_t, std::string> key2Exstr;

  PULServiceMsgHash2()
    : serviceType(0)
    , sid(0)
    , uid(0)
    , uip(0)
    , terminalType(UTT_DESKTOP) 
    , statType(S2C_DEFAULT_TYPE)
    , subsid(0)
    , suid(0)
  {}

  virtual void marshal(sox::Pack &pak) const 
  {
    pak << serviceType << sid << uid;
    pak.push_varstr32(msg.data(), msg.size());
    pak << uip << terminalType << statType << subsid << suid ;

    marshal_container(pak, key2Exstr);
  }
  virtual void unmarshal(const sox::Unpack &pak) 
  {
    pak >> serviceType >> sid >> uid;
    msg = pak.pop_varstr32();

    uip = 0;
    terminalType = UTT_DESKTOP;
    statType = S2C_DEFAULT_TYPE;
    subsid = 0;
    suid = to64Uid(uid);

    if( !pak.empty() )
    {
      pak >> uip;
    }
    else
    {
      return;
    }

    if (!pak.empty())
    {
      pak >> terminalType;
    }
    else
    {
      return;
    }	

    if(!pak.empty())
    {
      pak >> statType;
    }
    else
    {
      return;
    }

    if(!pak.empty())
    {
      pak >> subsid;
    }
    else
    {
      return;
    }

    if(!pak.empty())
    {
      pak >> suid;
    }
    else
    {
      return;
    }

    if(!pak.empty())
    {
      unmarshal_container(pak, std::inserter(key2Exstr, key2Exstr.begin()));
    }
    else
    {
      return;
    }
  }

  //Incomplete unmarshal, special for route_proxy(不完全解包，专门为route proxy设计，只解其中的三个字段)
  void unmarshal_incomplete(const sox::Unpack& up)
  {
    //  incomplete 的兼容太过复杂，容易出错
    // 所以直接使用unmarshal
    unmarshal(up);
  }
};

struct PDLServiceMsgByUid2 : public sox::Marshallable  
{
  enum{uri = (313 << 8 | protocol::SERVICE_SVID)};
  uint16_t serviceType;
  uint32_t uid;
  std::string msg;

  uint64_t suid; // sessioning user id = suid, low 32 bit = uid(user id)
  virtual void marshal(sox::Pack &pak) const {
    pak << serviceType << uid;
    pak.push_varstr32(msg.data(), msg.size());
    pak << suid;
  }
  virtual void unmarshal(const sox::Unpack &pak) {
    pak >> serviceType >> uid;
    msg = pak.pop_varstr32();
    if (!pak.empty())
    {
      pak >> suid;
    }
    else
    {
      suid = to64Uid(uid);
    }
  }
};

struct PCS_QueryCUL : public sox::Marshallable {
    //enum {uri = (1 << 8 | protocol::SMANAGER_SVID)};
    enum{uri = (150 << 8 | protocol::SERVICE_SVID)};
    uint32_t topSid;
    PCS_QueryCUL()
        :topSid(0)
    {};
    virtual void marshal(sox::Pack &p) const {
        p << topSid;
    }
    virtual void unmarshal(const sox::Unpack &p) {
        p >> topSid;
    }
};

struct PCS_QueryCULRes : public sox::Marshallable
{
    enum{uri = (151 << 8 | protocol::SERVICE_SVID)};
    uint32_t topSid;
    // sid to userCount map.
    std::map<uint32_t, uint32_t> userCountInfo;

    PCS_QueryCULRes()
        :topSid(0)
        ,userCountInfo()
    {};

    void marshal(sox::Pack &pk) const{
        pk << topSid ;
        sox::marshal_container( pk, userCountInfo);
    }
    void unmarshal(const sox::Unpack &up){
        up >> topSid;
        sox::unmarshal_container( up, std::inserter(userCountInfo, userCountInfo.begin() ) );
        //_pair;
    }
};


//批量获取顶级频道人数
struct PSS_BatchQueryChannelUserCount : public sox::Marshallable {
  enum{uri = (155 << 8 | protocol::SERVICE_SVID)};
  std::vector<uint32_t> topSids;
  std::string context; 
  PSS_BatchQueryChannelUserCount()
    :topSids()
    ,context()
  {};
  virtual void marshal(sox::Pack &p) const {
    sox::marshal_container(p,topSids);
    p<<context;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    sox::unmarshal_container( p, std::back_inserter( topSids));
    p>>context;
  }
};
struct PSS_BatchQueryChannelUserCountRes: public sox::Marshallable{

  enum{uri = (156 << 8 | protocol::SERVICE_SVID)};
  std::vector< std::pair<uint32_t, uint32_t> > userCountInfo;
  std::string context; 
  PSS_BatchQueryChannelUserCountRes()
    :userCountInfo()
    ,context()
  {};
  void marshal(sox::Pack &pk) const{
    sox::marshal_container( pk, userCountInfo);
    pk<<context;
  }
  void unmarshal(const sox::Unpack &up){
    sox::unmarshal_container( up, std::back_inserter(userCountInfo));
    up>>context;
  }
};

//获取顶级频道用户列表，返回各个子频道的Uid
//如果指定了maxUidCount，将随机返回这么多Uid
struct PSS_QueryChannelUserList: public sox::Marshallable
{
  enum{uri= ( 157 << 8 |protocol::SERVICE_SVID )};
  uint32_t topSid;
  uint32_t maxUidCount;
  std::string context;
  PSS_QueryChannelUserList()
    :topSid(0)
     ,maxUidCount(100000000)
  {}
  virtual void marshal(sox::Pack &p) const
  {
    p<<topSid<<maxUidCount<<context;
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p>> topSid>>maxUidCount>>context;
  }
};
struct PSS_QueryChannelUserListRes: public sox::Marshallable
{
  enum{uri= ( 158 << 8 |protocol::SERVICE_SVID )};
  uint32_t topSid;
  std::set<uint32_t> uids;
  uint32_t isWorking;
  std::string context;
  PSS_QueryChannelUserListRes()
    :topSid(0)
     ,uids()
     ,isWorking(1)
  {}
  virtual void marshal(sox::Pack &p) const
  {
    p<<topSid;
    sox::marshal_container(p,uids);
    p<<isWorking;
    p<< context;
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p>> topSid;
    sox::unmarshal_container(p,std::inserter(uids,uids.begin()));
    p>>isWorking;
    p>> context;
  }
};
//查询用户所在频道,子频道Id
struct PSS_QueryChannelIdByUid: public sox::Marshallable
{
  enum{uri= ( 159 << 8 |protocol::SERVICE_SVID )};
  uint32_t uid;
  std::string context; 
  PSS_QueryChannelIdByUid()
    :uid(0)
  {}
  virtual void marshal(sox::Pack &p) const
  {
    p<<uid;
    p<<context;
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p>>uid;
    p>>context;
  }
};
struct PSS_QueryChannelIdByUidRes: public sox::Marshallable
{
  enum{uri= ( 160 << 8 |protocol::SERVICE_SVID )};
  uint32_t uid;
  uint32_t topSid;
  uint32_t sid;
  uint32_t isWorking;
  std::string context; 
  PSS_QueryChannelIdByUidRes()
    :uid(0)
     ,topSid(0)
     ,sid(0)
     ,isWorking(1)
  {}
  virtual void marshal(sox::Pack &p) const
  {
    p<<uid<<topSid<<sid<<isWorking;
    p<< context;
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p>>uid>>topSid>>sid>>isWorking;
    p>>context;
  }
};

struct PServiceProxyUpdateSubSid: public sox::Marshallable
{
  enum{uri= ( 516 << 8 |protocol::SERVICE_SVID )};
  uint32_t uid;
  uint32_t topSid;
  //如果用户在顶级频道该字段填topSid
  uint32_t subSid;
  //如果用户在顶级频道或一级子频道,该字段填topSid
  //如果用户在二级子频道该字段填其所在一级子频道id
  uint32_t parentSid;

  uint64_t suid; // sessioning user id = suid, low 32 bit = uid(user id)

  virtual void marshal(sox::Pack &p) const
  {
    p << uid << topSid << subSid << parentSid << suid;
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> uid >> topSid >> subSid >> parentSid;
    if (!p.empty())
    {
      p >> suid;
    }
    else
    {
      suid = to64Uid(uid);
    }
  }

};

//递归各个有人子频道人数
//曾经叫过PCS_QueryCUL,这里添加了context字段
struct PQuerySubChannelUserCountByTopSid : public sox::Marshallable {
  enum{uri = (165 << 8 | protocol::SERVICE_SVID)};
  uint32_t topSid;
  std::string context;
  PQuerySubChannelUserCountByTopSid()
    :topSid(0)
    ,context()
  {};
  virtual void marshal(sox::Pack &p) const {
    p << topSid << context;
  
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> topSid >> context;
  }
};

struct PQuerySubChannelUserCountByTopSidRes : public sox::Marshallable
{
  enum{uri = (166 << 8 | protocol::SERVICE_SVID)};
  uint32_t topSid;
  // sid to userCount map.  
  std::map<uint32_t, uint32_t> userCountInfo;
  std::string context;

  PQuerySubChannelUserCountByTopSidRes()
    :topSid(0)
     ,userCountInfo()
    ,context()
  {};

  void marshal(sox::Pack &pk) const{
    pk << topSid << context ;
    sox::marshal_container( pk, userCountInfo);
  }
  void unmarshal(const sox::Unpack &up){
    up >> topSid >> context;
    sox::unmarshal_container( up, std::inserter(userCountInfo, userCountInfo.begin() ) );
  }
};

//查询单个子频道人数
struct PQuerySubChannelUserCount:public sox::Marshallable
{
  enum{uri = (163 << 8 | protocol::SERVICE_SVID)};
  uint32_t topSid;
  uint32_t subSid;
  std::string context;
  
  void marshal(sox::Pack &pk) const{
    pk << topSid << subSid<< context ;
  }
  void unmarshal(const sox::Unpack &up){
    up >> topSid >> subSid>> context;
  }
};


struct PQuerySubChannelUserCountRes: public sox::Marshallable
{
  enum{uri = (164 << 8 | protocol::SERVICE_SVID)};
  uint32_t topSid;
  uint32_t subSid;
  std::string context;
  uint32_t userCount;
  
  void marshal(sox::Pack &pk) const{
    pk << topSid << subSid<< context << userCount;
  }
  void unmarshal(const sox::Unpack &up){
    up >> topSid >> subSid>> context >> userCount;
  }
};

struct PSS_QuerySessionIdByUid:public sox::Marshallable
{
    enum{uri= ( 167 << 8 |protocol::SERVICE_SVID )};
    uint32_t uid;
    PSS_QuerySessionIdByUid():uid(0){}
    virtual void marshal(sox::Pack &p) const
    {
        p<<uid;
    }
    virtual void unmarshal(const sox::Unpack &p)
    {
        p>>uid;
    }
};
struct PSS_QuerySessionIdByUidRes:public sox::Marshallable
{
    enum{uri= ( 168 << 8 |protocol::SERVICE_SVID )};
    uint32_t uid;
    uint32_t topSid;
    uint32_t subSid;
    uint32_t sessionId;
    uint32_t isWorking;
    PSS_QuerySessionIdByUidRes()
        :uid(0)
         ,topSid(0)
         ,subSid(0)
         ,sessionId(0)
         ,isWorking(1)
    {}
    virtual void marshal(sox::Pack &p) const
    {
        p<<uid<<topSid<<subSid<<sessionId<<isWorking;
    }
    virtual void unmarshal(const sox::Unpack &p)
    {
        p>>uid>>topSid>>subSid>>sessionId>>isWorking;
    }
};


struct PSS_BatchQueryChannelIdByUid: public sox::Marshallable
{
    enum{uri= ( 170 << 8 |protocol::SERVICE_SVID )};
    std::set<uint32_t> uids;
    std::string context;   
    PSS_BatchQueryChannelIdByUid()
        :uids()
         ,context()
    {}
    virtual void marshal(sox::Pack &p) const
    {
        sox::marshal_container(p, uids);
        p<<context;
    }
    virtual void unmarshal(const sox::Unpack &p)
    {
        sox::unmarshal_container(p, std::inserter(uids, uids.begin()));
        p>>context;
    }
};

struct PSS_BatchQueryChannelIdByUidRes: public sox::Marshallable
{
    enum{uri= ( 171 << 8 |protocol::SERVICE_SVID )};
    std::map<uint32_t, std::pair<uint32_t,uint32_t> > uid2tidsid;
    uint32_t isWorking;
    std::string context;   
    PSS_BatchQueryChannelIdByUidRes()
        :uid2tidsid()
         ,isWorking(1)
         ,context()
    {}
    virtual void marshal(sox::Pack &p) const
    {
        p<<isWorking<<context;
        sox::marshal_container(p, uid2tidsid);
    }
    virtual void unmarshal(const sox::Unpack &p)
    {
        p>>isWorking>>context;
        sox::unmarshal_container(p, std::inserter(uid2tidsid, uid2tidsid.begin()));
    }
};


////////////////////////////////////////////////////////////
//////////////////V4 积分解耦消息定义///////////////////////
struct PULServiceMsgHash : public PULServiceMsg  
{
	enum{uri = (154 << 8 | protocol::SERVICE_SVID)};
};

struct JiFen : public sox::Marshallable {
	uint32_t  m_uUid; 
	uint32_t  m_uUserJiFen;
	uint32_t  m_sMemberJiFen;

	virtual void marshal(sox::Pack & pk) const {
		pk << m_uUid << m_uUserJiFen << m_sMemberJiFen;
	}

	virtual void unmarshal(const sox::Unpack &up) {
		up >> m_uUid >> m_uUserJiFen >> m_sMemberJiFen;
	}
};

struct PCS_GetJifenByUidsReq : public sox::Marshallable {
	enum {uri = (232 << 8 | protocol::SERVICE_SVID)};
	uint32_t uid;
	uint32_t sid;
	std::vector<uint32_t> uids;
	virtual void marshal(sox::Pack &pk) const {
		pk << uid <<sid;
		marshal_container(pk, uids);
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid >>sid;
		unmarshal_container(up, std::back_inserter(uids));
	}
};

struct PCS_GetJifenByUidsRes : public sox::Marshallable {
	enum {uri = (233 << 8 | protocol::SERVICE_SVID)};
	uint32_t uid;
	uint32_t sid;
	std::vector<JiFen> data; 
	virtual void marshal(sox::Pack &pk) const {
		pk << uid << sid;
		marshal_container(pk, data);
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid >> sid;
		unmarshal_container(up, std::back_inserter(data));
	}
};

struct PCS_UpdateJifenReq : public sox::Marshallable {
	enum {uri = (234 << 8 | protocol::SERVICE_SVID)};
	uint32_t uid; 
	uint32_t sid; 

	PCS_UpdateJifenReq(uint32_t pUid = 0, uint32_t pSid = 0) : uid(pUid), sid(pSid){

	}
	virtual void marshal(sox::Pack & pk) const {
		pk << uid << sid;
	}

	virtual void unmarshal(const sox::Unpack & up) {
		up >> uid >> sid;
	}
};

struct PCS_UpdateJifenRes : public sox::Marshallable {
	enum {uri = (235 << 8 | protocol::SERVICE_SVID)};
	uint32_t uid;
	uint32_t sid;
	std::vector<JiFen> data;
	virtual void marshal(sox::Pack &pk) const {
		pk << uid << sid;
		marshal_container(pk, data);
	}

	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid >> sid;
		unmarshal_container(up, std::back_inserter(data));
	}
};

struct PCS_BrocastJifen : public PCS_GetJifenByUidsRes
{
	enum {uri = (238 << 8 | protocol::SERVICE_SVID)};
};

// add by: lxxie <lixiangxie@gmail.com>
// date:   2011-03-29
// brief:  for custom defined authorization code
struct PUpdateUserPermission: public sox::Marshallable
{
	enum {uri = (4014 << 8) | protocol::SESSION_SVID};
	uint32_t topSid;
	uint32_t uid;
	uint64_t permission;

	virtual void marshal (sox::Pack& pack) const
	{
		pack << topSid << uid << permission;
	}

	virtual void unmarshal (const sox::Unpack& unpack)
	{
		unpack >> topSid >> uid >> permission;
	}
};

struct PSessionPermission: public sox::Marshallable
{
	enum {uri = (4015 << 8) | protocol::SESSION_SVID};
	
	uint32_t topSid;
	uint32_t uid;
	uint64_t permission;

	std::map<uint32_t, uint64_t> reserve1;
	std::map<uint32_t, uint64_t> reserve2;

	virtual void marshal (sox::Pack& pack) const
	{
		pack << topSid << uid << permission;
		marshal_container (pack, reserve1);
		marshal_container (pack, reserve2);
	}

	virtual void unmarshal (const sox::Unpack& unpack)
	{
		unpack >> topSid >> uid >> permission;
		unmarshal_container (unpack, std::inserter (reserve1, reserve1.begin()));
		unmarshal_container (unpack, std::inserter (reserve2, reserve2.begin()));
	}
};


struct PMobileChangeFolder: public sox::Marshallable {
  enum {uri = (20004 << 8 | protocol::SESSION_SVID)};
  uint32_t uid; 		// 用户uid
  uint32_t sid;		// 用户进入的子频道
  virtual void marshal(sox::Pack &p) const {
    p << uid << sid ;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> uid >> sid;
  }
};

struct PMobileGetChannelInfo : public sox::Marshallable {
  enum{uri = (20010 << 8 | protocol::SESSION_SVID)};
  uint32_t sid;
  uint32_t uid;
  virtual void marshal(sox::Pack &p) const {
    p << sid << uid;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> sid >> uid;
  }
};

struct PMobileGetChannelInfoRes : public sox::Marshallable {
  enum {uri = (20011 << 8 | protocol::SESSION_SVID)};
  uint32_t topSid;                  // 顶层频道id
  uint32_t receptionSid;            // 接待频道id
  ChannelUserMap disablevoice;      // 被禁言的用户列表
  ChannelUserMap disabletext;       // 被禁止打字的用户列表
  std::vector<uint32_t> chTextDisabled; // 禁止打字的频道
  std::vector<uint32_t> disableVisitorTextChs; // 禁止游客打字的频道

  virtual void marshal(sox::Pack &p) const {
    p << topSid << receptionSid;
    p << disablevoice;
    p << disabletext;
    sox::marshal_container(p, chTextDisabled);
    sox::marshal_container(p, disableVisitorTextChs);

  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> topSid >> receptionSid;
    up >> disablevoice;
    up >> disabletext;
    sox::unmarshal_container(up, std::back_inserter(chTextDisabled));
    sox::unmarshal_container(up, std::back_inserter(disableVisitorTextChs));
  }
};

typedef ZipMarshal<PMobileGetChannelInfoRes, 20012 << 8 | protocol::SESSION_SVID> PMobileGetChannelInfoZipRes;

struct PMobileGetUserInfo: public sox::Marshallable
{
  enum { uri = (20013 << 8 | protocol::SESSION_SVID) };
  uint32_t uid;
  uint32_t sid;
  std::vector<uint32_t> queryUids;
  virtual void marshal(sox::Pack &p) const
  {
    p << uid << sid;
    sox::marshal_container(p, queryUids);
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> uid >> sid;
    sox::unmarshal_container(up, std::back_inserter(queryUids));
  }
};

struct PMaixuSerialPack: public sox::Marshallable
{
  enum{uri = (12111 << 8 | protocol::SESSION_SVID)};
  uint64_t serial;
  std::string cmd;
  virtual void marshal(sox::Pack &pk) const
  {
    pk << serial;
    pk.push_varstr32(cmd.data(), cmd.length());
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> serial;
    cmd = up.pop_varstr32();
  }
};

struct PGetMediaProxyInfo2 : public sox::Marshallable {
  enum{uri = (870 << 8 | protocol::SESSION_SVID)};
  uint32_t uid;
  uint32_t ip;
  uint32_t reserver1;
  virtual void marshal(sox::Pack &p) const {
    p << uid << ip << reserver1;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> uid >> ip >> reserver1;
  }
};

struct PGetVideoProxyInfo: public sox::Marshallable {
  enum{uri = (10010 << 8 | protocol::SESSION_SVID)};
  uint32_t uid;
  uint32_t ip;
  uint32_t appid;
  uint32_t codeRate;
  virtual void marshal(sox::Pack &p) const {
    p << uid << ip << appid << codeRate;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> uid >> ip >> appid >> codeRate;
  }
};

// 管理员连麦请求及响应，1号麦序请求后被邀请者接受
struct PAddChorus : public sox::Marshallable{
  enum{uri = (13200 << 8 | protocol::SESSION_SVID)};
  uint32_t tid;
  uint32_t sid;
  uint32_t admin; // 管理员uid
  uint32_t mic_first; // 1号麦
  uint32_t invitee; // 连麦用户
  virtual void marshal(sox::Pack &pk) const {
     pk << tid << sid << admin<< mic_first<< invitee;
  }
  virtual void unmarshal(const sox::Unpack &up) {
     up >> tid >> sid >> admin>> mic_first>> invitee;
  }  
};

// 1号麦邀请其他用户连麦发起的请求协议，服务器发送该协议给受邀者
struct PInviteChorus: public sox::Marshallable{
  enum{uri = (13201<< 8 | protocol::SESSION_SVID)};
  uint32_t tid;
  uint32_t sid;
  uint32_t mic_first; // 1号麦
  uint32_t invitee; // 连麦用户
  virtual void marshal(sox::Pack &pk) const {
     pk << tid << sid << mic_first<< invitee;
  }
  virtual void unmarshal(const sox::Unpack &up) {
     up >> tid >> sid >> mic_first>> invitee;
  }  
};

//用户接受和拒绝连麦发起的请求协议，服务器根据res广播或者只发送给邀请者
struct PInviteChorusRes: public sox::Marshallable{
  enum{uri = (13202<< 8 | protocol::SESSION_SVID)};
  uint32_t tid;
  uint32_t sid;
  uint32_t res; // 0: ok, 1: refuse
  uint32_t mic_first; // 1号麦
  uint32_t invitee; // 连麦用户
  virtual void marshal(sox::Pack &pk) const {
     pk << tid << sid << res << mic_first<< invitee;
  }
  virtual void unmarshal(const sox::Unpack &up) {
     up >> tid >> sid >> res >> mic_first>> invitee;
  }  
};

// 取消连麦发起的请求协议，服务器将这个协议广播下去
struct PRemoveChorus: public sox::Marshallable{
  enum{uri = (13203 << 8 | protocol::SESSION_SVID)};
  uint32_t tid;
  uint32_t sid;
  uint32_t oper; // 0 : 表示操作者为1号麦序, 非0: 操作者为管理员，填uid
  uint32_t mic_first; // 1号麦
  uint32_t invitee; // 连麦用户
  virtual void marshal(sox::Pack &pk) const {
     pk << tid << sid << oper<< mic_first<< invitee;
  }
  virtual void unmarshal(const sox::Unpack &up) {
     up >> tid >> sid >> oper>> mic_first>> invitee;
  }  
};


// 获取连麦用户列表
struct PGetChorusList: public sox::Marshallable{
  enum{uri = (13204 << 8 | protocol::SESSION_SVID)};
  uint32_t tid;
  uint32_t sid;
  virtual void marshal(sox::Pack &pk) const {
     pk << tid << sid;
  }
  virtual void unmarshal(const sox::Unpack &up) {
     up >> tid >> sid;
  }  
};

// 获取连麦用户列表返回
struct PGetChorusListRes: public sox::Marshallable{
  enum{uri = (13205 << 8 | protocol::SESSION_SVID)};
  uint32_t tid;
  uint32_t sid;
  uint32_t mic_first; // 1号麦
  std::set<uint32_t> users;
  virtual void marshal(sox::Pack &pk) const {
     pk << tid << sid << mic_first;
     marshal_container(pk, users);
  }

  virtual void unmarshal(const sox::Unpack &up) {
     up >> tid >> sid >> mic_first;
     unmarshal_container(up, std::inserter(users, users.begin()));
  }  
};

struct PClientSvcReq: public sox::Marshallable
{
  enum{uri = (12090 << 8 | protocol::SESSION_SVID)};
  uint32_t topSid;
  uint32_t uid;
  uint32_t type;
  uint32_t reserved1;
  uint32_t reserved2;
  std::string data;

  enum
  {
    ONLINE  = 1,
    SINFO   = 2,
    UINFO   = 3,
    ROLER   = 4,
    AUTH    = 5,
    MICLIST = 6,
  };

  PClientSvcReq(): reserved1(0), reserved2(0) {}

  virtual void marshal(sox::Pack &pk) const
  {
    pk << topSid << uid << type << reserved1 << reserved2;
    pk.push_varstr32(data.data(), data.length());
  }

  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> topSid >> uid >> type >> reserved1 >> reserved2;
    data = up.pop_varstr32();
  }
};

struct PClientSvcRes: public sox::Marshallable
{
  enum{uri = (12099 << 8 | protocol::SESSION_SVID)};
  uint32_t topSid;
  uint32_t uid;
  uint32_t type;
  uint32_t reserved1;
  uint32_t reserved2;
  std::string data;

  PClientSvcRes(): reserved1(0), reserved2(0) {}

  virtual void marshal(sox::Pack &pk) const
  {
    pk << topSid << uid << type << reserved1 << reserved2;
    pk.push_varstr32(data.data(), data.length());
  }
  virtual void unmarshal(const sox::Unpack & up)
  {
    up >> topSid >> uid >> type >> reserved1 >> reserved2;
    data = up.pop_varstr32();
  }
};

struct PServiceProxys: public sox::Marshallable   //记录sercie proxy列表
{
std::vector<PServiceProxyAddr> IpList;
virtual void marshal(sox::Pack &p) const {
	marshal_container(p, IpList);
}
virtual void unmarshal(const sox::Unpack &up) {
	unmarshal_container(up, std::back_inserter(IpList));
}
};

typedef std::map<uint16_t, std::pair<uint32_t, uint32_t> > svcIdChannelsMap_t;

//全球化部署涉及到的消息
struct CGroupId2SvcProxy: public sox::Marshallable
{
    std::map<uint16_t, PServiceProxys> m_mapData;
    virtual void marshal(sox::Pack &pak) const
    {
        marshal_container(pak, m_mapData);

    }
    virtual void unmarshal(const sox::Unpack &pak)
    {
        unmarshal_container(pak, std::inserter(m_mapData, m_mapData.begin()));
    }
};

struct PSS_GPServiceProxyAddrList3 : public sox::Marshallable
{
    enum{uri = (1701 << 8 | protocol::SERVICE_SVID)};

    uint8_t preferredTransfer;
    std::set<uint16_t> serviceType;
    std::map<uint32_t,CGroupId2SvcProxy > m_mapIsp2SvcProxy;//key:isp,value:GroupId2SvcProxy
    svcIdChannelsMap_t serviceType2channels;
    uint32_t fromSid;
    uint32_t toSid;
    std::vector<std::pair<uint32_t, uint32_t> > sessionVersion;
    std::vector<std::pair<uint32_t, uint32_t> > serviceVersion;
    uint32_t udpFromSid;
    uint32_t udpToSid;
    uint32_t m_uDefaultIsp;//当客户端找不到和自己匹配的isp的时候，使用的默认isp对应的proxy列表
    virtual void marshal(sox::Pack &pak) const
    {
        pak<<m_uDefaultIsp;
        pak.push_uint8(preferredTransfer);
        marshal_container(pak, serviceType);
        marshal_container(pak, m_mapIsp2SvcProxy);
        marshal_container(pak, serviceType2channels);
        pak << fromSid << toSid;
        sox::marshal_container(pak, sessionVersion);
        sox::marshal_container(pak, serviceVersion);
        pak << udpFromSid;
        pak << udpToSid;

    }
    virtual void unmarshal(const sox::Unpack &pak)
    {
        pak>>m_uDefaultIsp;
        preferredTransfer = pak.pop_uint8();
        unmarshal_container(pak, std::inserter(serviceType, serviceType.begin()));
        unmarshal_container(pak, std::inserter(m_mapIsp2SvcProxy, m_mapIsp2SvcProxy.begin()));
        unmarshal_container(pak, std::inserter(serviceType2channels, serviceType2channels.begin()));
        if(pak.empty())
        {
            fromSid = toSid = 0;
            sessionVersion.clear();
            serviceVersion.clear();
        }else
        {
            pak >> fromSid >> toSid;
            sox::unmarshal_container(pak, std::back_inserter(sessionVersion));
            sox::unmarshal_container(pak, std::back_inserter(serviceVersion));
        }
        if(pak.empty())
        {
            udpFromSid = udpToSid = 0;
        }
        else
        {
            pak >> udpFromSid >> udpToSid;
        }
    }
};
typedef ZipMarshal<PSS_GPServiceProxyAddrList3, (1702 << 8 | protocol::SERVICE_SVID)> PSS_GPServiceProxyAddrListZip3;

struct PNotifyCurrentStream3: public sox::Marshallable {
	enum{uri = (10031 << 8 | protocol::SESSION_SVID)};
	uint32_t uid;
	uint32_t tid;
	uint32_t sid; // sub channel
	std::map<uint64_t, uint32_t> stream2app;
	void marshal(sox::Pack &p) const{
		p << uid << tid << sid;
  		sox::marshal_container(p, stream2app);
	}
	void unmarshal(const sox::Unpack &up){
		up >> uid >> tid >> sid;
		sox::unmarshal_container(up, std::inserter(stream2app, stream2app.begin()));
	}
};

struct PGetVideoProxyInfoRes2: public sox::Marshallable 
{
  enum{uri = (10036 << 8 | protocol::SESSION_SVID)};

  uint32_t appid;
  std::vector<IpInfo> videoProxyInfo;
  std::map<uint8_t, uint32_t> configInfo; // key :VideoConfig
  LoginRetryProc retryType; // 重连操作控制
  uint16_t retryFreq;       // 重连频率控制，单位s

  PGetVideoProxyInfoRes2() : retryType(E_LOGIN_SUCCESS), retryFreq(0) {};

  void marshal(sox::Pack &p) const{
    p << appid;
    sox::marshal_container(p, videoProxyInfo);
    sox::marshal_container(p, configInfo);
    p.push_uint8(retryType);
    p << retryFreq;
  }
  void unmarshal(const sox::Unpack &up){
    up >> appid;
    sox::unmarshal_container(up, std::back_inserter(videoProxyInfo));
    sox::unmarshal_container(up, std::inserter(configInfo, configInfo.begin()));
    if (!up.empty())
    {
      retryType = (LoginRetryProc)up.pop_uint8();
      up >> retryFreq;
    }
  }
};

struct PLoginServiceProxy2 : public PLoginServiceProxy 
{
	enum{uri = (498 << 8 | protocol::SERVICE_SVID)};
  enum ESvcLoginMod
  {
    SP_LOGIN_FIRST = 1, // 第一次进频道登录
    SP_LOGIN_RECON = 2, // 重连登录
    SP_LOGIN_BY_PASS = 3 //旁路测试登陆
  };
	uint8_t loginMode;
	std::string tickets;
	PLoginServiceProxy2()
	{
		uid = 0;
		sid = 0;
		clientVersion = 0;
		cookies.clear();
		loginMode = SP_LOGIN_FIRST;
	}
	virtual void marshal(sox::Pack &p) const
	{
		PLoginServiceProxy::marshal(p);
		p << loginMode << tickets;
	}
	virtual void unmarshal(const sox::Unpack &up)
	{
		PLoginServiceProxy::unmarshal(up);
		if (up.empty())
		{
			loginMode = SP_LOGIN_FIRST;
		}
		else
		{
			up >> loginMode;
			if(up.empty())
			{
				tickets = "";
			}
			else
			{
				up>>tickets;
			}
		}
	}
};

struct PGroupIds : public sox::Marshallable
{
	std::vector<uint16_t> groupIds;
	virtual void marshal(sox::Pack &pak) const {
		marshal_container(pak, groupIds);
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		unmarshal_container(pak, std::back_inserter(groupIds));
	}
};

struct PLoginServiceProxyRes2 : public PLoginServiceProxyRes  
{
	enum{uri = (499 << 8 | protocol::SERVICE_SVID)};
	std::map<uint16_t, PGroupIds> type2GroupIds;
    uint16_t    innerRes;
	virtual void marshal(sox::Pack &pak) const {
		PLoginServiceProxyRes::marshal(pak);
		marshal_container(pak, type2GroupIds);
        pak << innerRes;
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		PLoginServiceProxyRes::unmarshal(pak);
		unmarshal_container(pak, std::inserter(type2GroupIds, type2GroupIds.begin()));
        if (!pak.empty())
        {
            pak >> innerRes;
        }
	}
};

enum ConnectionType
{ 
  SHORT_CONN = 0,
  LONG_CONN = 1,
  ERROR_CONN
};

struct PServiceUInfoReq : public sox::Marshallable
{
  enum {uri = (240 << 8 | protocol::SERVICE_SVID)};
  uint32_t uid;
  uint32_t sid;
  std::set<uint32_t> reqUids;
  ConnectionType connType;

  virtual void marshal(sox::Pack &pk) const {
    pk << uid << sid;
    marshal_container(pk, reqUids);
    pk.push_uint8(connType);
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> uid >> sid;
    unmarshal_container(up, std::inserter(reqUids, reqUids.begin()));
    connType = (ConnectionType)up.pop_uint8();
  }
};

//长短用户信息连接请求2，以connType区分
struct PServiceUInfoReq2 : public PServiceUInfoReq
{
  enum {uri = (1105 << 8 | protocol::SERVICE_SVID)};
};

struct ServiceUserInfo2 : public sox::Marshallable
{
	uint32_t uid;
	std::string nickname;
	std::string	sign;
	protocol::uinfo::SEX sex;
	uint32_t yyid;
	std::string reserved;

	virtual void marshal(sox::Pack &pk) const {
		pk << uid << nickname << sign << yyid << reserved;
		pk.push_uint8(sex);
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid >> nickname >> sign >> yyid >> reserved;
		sex = (protocol::uinfo::SEX)up.pop_uint8();
	}
};

//短连接用户信息请求响应2
struct PServiceUInfoRes2 : public sox::Marshallable
{
  enum {uri = (1106 << 8 | protocol::SERVICE_SVID)};
  std::vector<ServiceUserInfo2> m_vecUidToInfo;
  virtual void marshal(sox::Pack &pk) const
  {
    marshal_container(pk, m_vecUidToInfo);
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    unmarshal_container(up, std::back_inserter(m_vecUidToInfo));
  }
};

//长连接用户信息请求响应2
struct PServiceUInfoBroadRes2 : public PServiceUInfoRes2
{
  enum {uri = (1107 << 8 | protocol::SERVICE_SVID)};
};

//长连接用户信息广播更新2
struct PServiceUInfoBroadUpdate2 : public sox::Marshallable
{
  enum {uri = (1108 << 8 | protocol::SERVICE_SVID)};

  ServiceUserInfo2 uinfo;

  virtual void marshal(sox::Pack &pk) const {
    uinfo.marshal(pk);
  }
  virtual void unmarshal(const sox::Unpack &up) {
    uinfo.unmarshal(up);
  }
};

struct PLoginSvcProxy : public sox::Marshallable
{
  enum { uri = (2501 << 8 | protocol::SERVICE_SVID) };
  enum { SP_LOGIN_FIRST, SP_LOGIN_AGAIN, SP_LOGIN_BY_PASS};
  enum {CLIENT_NOT_SUPPORT_SUID = 0xFFFFFFFF};
  uint8_t  loginMode;
  uint32_t uid; // must set , cookie check will it
  uint32_t sid; //not necessary, for reconnect used
  uint32_t subsid; //not necessary, for reconnect used
  uint32_t isp;
  uint32_t protocolVer;
  std::string clientVer;
  std::string cookie;
  std::string ticket;

  uint64_t suid; // sessioning user id = suid, low 32 bit = uid(user id)

  PLoginSvcProxy()
    : loginMode(0)
    , uid(0)
    , sid(0)
    , subsid(0)
    , isp(0)
    , protocolVer(0)
    , suid(CLIENT_NOT_SUPPORT_SUID)
  {

  }

  virtual void marshal(sox::Pack& pk) const
  {
    pk << loginMode << uid << sid << subsid << isp << protocolVer << clientVer << cookie << ticket << suid;
  }

  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> loginMode >> uid >> sid >> subsid >> isp >> protocolVer >> clientVer >> cookie >> ticket;

    if(!up.empty())
    {
      up >> suid;
      if(suid ==0 )
      {
        suid = to64Uid(uid); // 防止 client 忘记填suid, 或者填0了
      }
    }
    else
    {
      suid = CLIENT_NOT_SUPPORT_SUID; // service proxy 使用CLIENT_NOT_SUPPORT_SUID 来判断，client是否支持 suid 
      return;
    }

  }
};

struct PLoginSvcProxyRes : public sox::Marshallable
{
  enum { uri = (2502 << 8 | protocol::SERVICE_SVID) };
  enum {CLIENT_NOT_SUPPORT_SUID = 0xFFFFFFFF};

  uint8_t  innerRes;
  uint32_t reserve;


  uint64_t suid; // sessioning user id = suid, low 32 bit = uid(user id)

  PLoginSvcProxyRes() : innerRes(RES_SUCCESS), reserve(0), suid(CLIENT_NOT_SUPPORT_SUID) {}

  virtual void marshal(sox::Pack& pk) const
  {
    pk << innerRes << reserve << suid;
  }
  virtual void unmarshal(const sox::Unpack& up)
  {
    up >> innerRes >> reserve;

    if(!up.empty())
    {
      up >> suid;
    }
    else
    {
      suid = CLIENT_NOT_SUPPORT_SUID; // CLIENT_NOT_SUPPORT_SUID 来判断Service proxy 是否支持 suid 
      return;
    }

  }
};

struct PJoinChannel : public sox::Marshallable
{
  enum { uri = (2504 << 8 | protocol::SERVICE_SVID) };
  uint32_t uid;
  uint32_t sid;
  uint32_t subsid;

  uint64_t suid; // sessioning user id = suid, low 32 bit = uid(user id)

  virtual void marshal(sox::Pack& pk) const
  {
    pk << uid << sid << subsid << suid;
  }

  virtual void unmarshal(const sox::Unpack& up)
  {
    up >> uid >> sid >> subsid;

    suid = to64Uid(uid);
    if(!up.empty())
    {
      up >> suid;
    }
    else
    {
      return;
    }
  }
};

struct PLeaveChannel : public sox::Marshallable
{
  enum { uri = (2505 << 8 | protocol::SERVICE_SVID) };
  uint32_t uid;

  uint64_t suid; // sessioning user id = suid, low 32 bit = uid(user id)

  virtual void marshal(sox::Pack& pk) const
  {
    pk << uid << suid;
  }
  virtual void unmarshal(const sox::Unpack& up)
  {
    up >> uid;

    suid = to64Uid(uid);
    if(!up.empty())
    {
      up >> suid;
    }
    else
    {
      return;
    }
  }
};

struct PULServiceMsgHashTL : public sox::Marshallable
{
  enum { uri = (502 << 8 | protocol::SERVICE_SVID) };
  uint16_t serviceType;
  uint32_t sid;
  uint32_t uid;
  std::string msg;

  uint64_t suid; // sessioning user id = suid, low 32 bit = uid(user id)

  virtual void marshal(sox::Pack &pk) const
  {
    pk << serviceType << sid << uid;
    pk.push_varstr32(msg.data(), msg.size());
    pk << suid;
  }
  virtual void unmarshal(const sox::Unpack &pak) 
  {
    pak >> serviceType >> sid >> uid;
    msg = pak.pop_varstr32();

    suid = to64Uid(uid);
    if(!pak.empty())
    {
      pak >> suid;
    }
    else
    {
      return;
    }

  }

};

struct PDLServiceMsgByUidTL : public PDLServiceMsgByUid2
{
  enum { uri = (503 << 8 | protocol::SERVICE_SVID) };
};

struct TextChatExProperties : public sox::Marshallable
{
  enum EXT_INFO_TYPE
  {
    EXT_VIP_TYPE = 1,
    EXT_ROLE_TYPE = 2,
    EXT_GENDER = 3,
    EXT_IMID = 4,
    EXT_VIP_LIANGNO=5
  };

  std::map<uint16_t, std::string> props;

  void SetVip(const std::string& v)
  { 
    props[EXT_VIP_TYPE] = v;
  }

  void SetRole(const std::string& v)
  { 
    props[EXT_ROLE_TYPE] = v;
  }

  void SetGender(const std::string& v)
  {
    props[EXT_GENDER] = v;
  }

  void SetImid(const std::string& v)
  {
    props[EXT_IMID] = v;
  }

  void SetVipLiangNumber(const std::string& v)
  {
    props[EXT_VIP_LIANGNO] = v;
  }

  bool isVip()
  {
    return props.find(EXT_VIP_TYPE) != props.end();
  }

  bool isRole()
  {
    return props.find(EXT_ROLE_TYPE) != props.end();
  }

  bool GetVip(std::string &s)
  { 
    std::map<uint16_t, std::string>::iterator it = props.find(EXT_VIP_TYPE);
    if (it != props.end())
    {
      s = it->second;
      return true;
    }
    return false;
  }

  bool GetRole(std::string &s)
  { 
    std::map<uint16_t, std::string>::iterator it = props.find(EXT_ROLE_TYPE);
    if (it != props.end())
    {
      s = it->second;
      return true;
    }
    return false;
  }

  bool GetGender(std::string &s)
  {
    std::map<uint16_t, std::string>::iterator it = props.find(EXT_GENDER);
    if (it != props.end())
    {
      s = it->second;
      return true;
    }
    return false;
  }

  bool GetImid(std::string &s)
  {
    std::map<uint16_t, std::string>::iterator it = props.find(EXT_IMID);
    if (it != props.end())
    {
      s = it->second;
      return true;
    }
    return false;
  }

  bool GetVipLiangNumber(std::string &s)
  {
    std::map<uint16_t, std::string>::iterator it = props.find(EXT_VIP_LIANGNO);
    if (it != props.end())
    {
      s = it->second;
      return true;
    }
    return false;
  }

  virtual void marshal(sox::Pack &p) const
  {
    sox::marshal_container(p, props);
  }

  virtual void unmarshal(const sox::Unpack &p)
  {
    sox::unmarshal_container(p, std::inserter(props, props.begin()));
  }
};

struct PTextChatMobile: public sox::Marshallable
{
  enum{ uri = (12126 << 8 | protocol::SERVICE_SVID) };
  uint32_t from;
  uint32_t topsid;
  uint32_t sid;
  std::string chat;
  std::string reserver1;
  std::string reserver2;
  std::string nick;
  TextChatExProperties extInfo;

  virtual void marshal(sox::Pack &p) const 
  {
    p << from << topsid << sid << chat << reserver1 << reserver2;
    p << nick;
    extInfo.marshal(p);
  }
  virtual void unmarshal(const sox::Unpack &p) 
  {
    p >> from >> topsid >> sid >> chat >> reserver1 >> reserver2;
    if (!p.empty())
    {
      p >> nick;
    }
    if (!p.empty())
    {
      extInfo.unmarshal(p);
    }
  }
};

struct PTextChatMobileRes: public PTextChatMobile
{
	enum{ uri = (12127 << 8 | protocol::SERVICE_SVID) };

	virtual void marshal(sox::Pack &p) const 
	{
		PTextChatMobile::marshal(p);
	}
	virtual void unmarshal(const sox::Unpack &p) 
	{
		PTextChatMobile::unmarshal(p);
	}
};


// lxxie add ;

enum SyncRoleReason
{
          INVITE_CODE = 1, 
};

enum MRM_LOCS_URI
{
  MRM_LOCS_REGISTER_URI = 1,
  MRM_LOCS_REGRES_URI,
  MRM_LOCS_PING_URI,
  MRM_LOCS_PING_RES_URI,
  MRM_LOCS_ALLOC_PROXY_URI,               // allocate proxy
  MRM_LOCS_ALLOC_PROXY_RES_URI,
  MRM_LOCS_REBUILD_GROUP_URI,             // rebuild MR group
  MRM_LOCS_REBUILD_GROUP_RES_URI,
  MRM_LOCS_RTT_PING_URI,
  MRM_LOCS_RTT_PING_RES_URI,
  MRM_LOCS_ALLOC_PROXY_VIA_UDP_LINKD_URI,
  //  MRM_LOCS_ALLOC_PROXY_VIA_UDP_LINKD_RES_URI,
  MAX_MRM_LOCS_URI
};

struct PUserAllocProxy : public sox::Marshallable
{
  enum{uri = ( MRM_LOCS_ALLOC_PROXY_URI << 8 | protocol::LOCATION_SERVER_SVID )};
  uint32_t Version;
  uint32_t Uid;
  uint32_t ChanId;
  uint32_t UserIp;
  std::vector<uint32_t> forbiddenIps;
  uint32_t clientType;
  uint32_t serviceType;

  PUserAllocProxy()
    : Version(1), clientType(0), serviceType(0)
  {} 

  virtual void marshal(sox::Pack &pak) const
  {
    pak << Version << Uid << ChanId << UserIp;
    sox::marshal_container(pak, forbiddenIps);
    pak << clientType << serviceType;
  }
  virtual void unmarshal(const sox::Unpack &pak)
  {
    pak >> Version >> Uid >> ChanId >> UserIp;
    sox::unmarshal_container(pak, std::back_inserter(forbiddenIps));
    if(Version >= 1 && pak.size() >= 8) pak >> clientType >> serviceType;
  }
};

struct PUserAllocProxyRes : public sox::Marshallable
{
  enum{uri = (MRM_LOCS_ALLOC_PROXY_RES_URI << 8 | protocol::LOCATION_SERVER_SVID)};
  enum { TCP_TYPE = 1, UDP_TYPE = 2 };
  uint32_t Version;
  uint32_t Uid;
  uint32_t ChanId;
  std::vector<IpInfo> ProxyInfo;
  LoginRetryProc retryType;
  uint16_t retryFreq;  // in second
  uint32_t mrmGrpId;
  uint32_t linkdType; //1-tcp 2-upd
  
  PUserAllocProxyRes()
      : Version(3),
      retryType(E_LOGIN_SUCCESS),
      retryFreq(10),
      linkdType(TCP_TYPE)
  {}

  virtual void marshal(sox::Pack &pak) const
  {
    pak << Version << Uid << ChanId;
    sox::marshal_container(pak, ProxyInfo);
    pak.push_uint8(retryType);
    pak << retryFreq ;
    pak << mrmGrpId;
    pak << linkdType;

  }
  virtual void unmarshal(const sox::Unpack &pak)
  {
    pak >> Version >> Uid >> ChanId;
    sox::unmarshal_container(pak, std::back_inserter(ProxyInfo));
    retryType = (LoginRetryProc) pak.pop_uint8();
    pak >> retryFreq ;
    if( Version >= 1) pak >> mrmGrpId;
    if( Version >= 3 && pak.size() >= 4) 
    {
      pak >> linkdType;
    }
  }
};

struct PUserAllocProxyViaUdpLinkd : public PUserAllocProxy
{
  enum{uri = ( MRM_LOCS_ALLOC_PROXY_VIA_UDP_LINKD_URI << 8 | protocol::LOCATION_SERVER_SVID )};

  virtual void marshal(sox::Pack &pak) const
  {
    PUserAllocProxy::marshal(pak);
  }
  virtual void unmarshal(const sox::Unpack &pak)
  {
    PUserAllocProxy::unmarshal(pak);
  }
};
}
}
#endif /*PCOMMON_SESSION_H_*/

