#ifndef PSMEMBERLIST_H_
#define PSMEMBERLIST_H_
#include "common/protocol/psessionbase.h"
#include "common/protocol/const.h"
#include <iterator>

namespace protocol {
namespace session {

struct POnMemberAdd : public sox::Marshallable {
	uint32_t uid;
	std::string nick;
	std::string sign;
	uinfo::SEX sex;
	uint32_t pid;
	uint32_t uinfoJifen;
	uint32_t smemberJifen;
	std::vector<MRolerAndCh> rolers;
	virtual void marshal(sox::Pack &p) const {
		p << uid << nick << sign << pid << uinfoJifen << smemberJifen;
		p.push_uint8(sex);
		sox::marshal_container(p, rolers);
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> uid >> nick >> sign >> pid >> uinfoJifen >> smemberJifen;
		sex = (uinfo::SEX)p.pop_uint8();
		sox::unmarshal_container(p, std::back_inserter(rolers));
	}
};

struct POnMemberRemove : public sox::Marshallable {
	uint32_t uid;
	uint32_t sid;

	virtual void marshal(sox::Pack &p) const {
		p << uid << sid;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> uid >> sid;
	}
};

struct PChangeFolder : public sox::Marshallable {
	enum {uri = (98 << 8) | SESSION_SVID};
	//std::string uid;
	uint32_t from;
	uint32_t to;
	std::string passwd;

	virtual void marshal(sox::Pack &p) const {
		p << from << to << passwd;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> from >> to >> passwd;
	}
};


struct PChangePInfo : public sox::Marshallable {
	enum {uri = (99 << 8) | SESSION_SVID};
	std::string nick;
	uinfo::SEX gender;
	std::string sign;
	virtual void marshal(sox::Pack &p) const {
		p  << nick << sign;
		p.push_uint8(gender);
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p  >> nick >> sign;
		gender = (uinfo::SEX)p.pop_uint8();
	}
};

struct PPInfoChanged : public PChangePInfo {
	uint32_t uid;
	virtual void marshal(sox::Pack &pak) const {
		PChangePInfo::marshal(pak);
		pak << uid;
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		PChangePInfo::unmarshal(pak);
		pak >> uid;
	}
};

struct PPushSessData: public sox::Marshallable{
	enum {uri = (100 << 8) | SESSION_SVID};
	std::vector<POnMemberAdd> adds;
	std::vector<uint32_t> removes;
	std::vector<std::pair<uint32_t, uint32_t> > changes;
	std::vector<uint32_t> folders;
	std::vector<uint32_t> settles;
	std::vector<uint32_t> deSettles;
	virtual void marshal(sox::Pack &pak) const {
		sox::marshal_container(pak, adds);
		sox::marshal_container(pak, removes);
		sox::marshal_container(pak, changes);
		sox::marshal_container(pak, folders);
		sox::marshal_container(pak, settles);
		sox::marshal_container(pak, deSettles);
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		adds.reserve(pak.peek_uint32());
		sox::unmarshal_container(pak, std::back_inserter(adds));
		sox::unmarshal_container(pak, std::back_inserter(removes));
		sox::unmarshal_container(pak, std::back_inserter(changes));
		sox::unmarshal_container(pak, std::back_inserter(folders));
		sox::unmarshal_container(pak, std::back_inserter(settles));
		sox::unmarshal_container(pak, std::back_inserter(deSettles));
	}
};

struct PMaixuJoin: public ChannelQueueMar{
	enum {uri = (101 << 8) | SESSION_SVID};
	uint32_t channelId;
	virtual void marshal(sox::Pack &pak) const {
		ChannelQueueMar::marshal(pak);
		pak << channelId;
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		ChannelQueueMar::unmarshal(pak);
		pak >> channelId;
	}
};

}
}
#endif /*PSMEMBERLIST_H_*/
