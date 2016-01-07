#ifndef PQLIST_H_
#define PQLIST_H_
#include "common/packet.h"
#include <vector>
#include <list>
#include <iterator>
#include "common/core/base_svid.h"

namespace protocol{
namespace qlist{

struct QListItem : public sox::Marshallable {
	uint32_t asid;
	uint32_t sid;
	uint32_t tid;
	std::string name;

	virtual void marshal(sox::Pack &p) const {
		p << asid << sid << tid << name;
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> asid >> sid >> tid >> name;
	}
};

struct PSyncQList : public sox::Voidmable {
	enum {uri = 8 << 8 | protocol::SLIST_SVID};
};

struct PSyncQListRes : public sox::Marshallable {
	enum {uri = 9 << 8 | protocol::SLIST_SVID};
	std::vector<QListItem> items;
	virtual void marshal(sox::Pack &p) const {
		marshal_container(p, items);
	}
	virtual void unmarshal(const sox::Unpack &p) {
		unmarshal_container(p, std::back_inserter(items));
	}
};

struct PAddQList : public sox::Marshallable {
	enum {uri = 10 << 8 | protocol::SLIST_SVID};
	uint32_t asid;
	uint32_t sid;
	uint32_t tid;
	std::string name;
	virtual void marshal(sox::Pack &p) const {
		p << asid << sid << tid << name;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> asid >> sid >> tid >> name;
	}
};

struct PAddQListRes : public sox::Marshallable {
	enum {uri = 11 << 8 | protocol::SLIST_SVID};
	uint32_t asid;
	uint32_t sid;
	uint32_t tid;
	std::string name;
	virtual void marshal(sox::Pack &p) const {
		p << asid << sid << tid << name;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> asid >> sid >> tid >> name;
	}
};

struct PRemoveQList : public sox::Marshallable {
	enum {uri = 12 << 8 | protocol::SLIST_SVID};
	uint32_t asid;
	uint32_t sid;
	uint32_t tid;
	std::string name;
	virtual void marshal(sox::Pack &p) const {
		p << asid << sid << tid << name;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> asid >> sid >> tid >> name;
	}
};

struct PRemoveQListRes  : public sox::Marshallable {
	enum {uri = 13 << 8 | protocol::SLIST_SVID};
	uint32_t asid;
	uint32_t sid;
	uint32_t tid;
	std::string name;
	virtual void marshal(sox::Pack &p) const {
		p << asid << sid << tid << name;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> asid >> sid >> tid >> name;
	}
};

}
}

#endif /*PQLIST_H_*/
