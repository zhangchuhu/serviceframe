#ifndef PROTOCOL_SESS_SEARCH_H
#define PROTOCOL_SESS_SEARCH_H
#include "common/protocol/psmanager.h"

struct PFindChannelProxy: public PFindChannel {
	enum {uri = (1000 << 8 | protocol::SDB_SVID)};
	uint32_t serverid;
	uint32_t uid;

	virtual void marshal(sox::Pack &p) const {
		p << serverid << uid;
		PFindChannel::marshal(p);
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> serverid >> uid;
		PFindChannel::unmarshal(p);
	}
};

struct PFindResultProxy: public PFindResult {
	enum {uri = (1001 << 8 | protocol::SDB_SVID)};
	uint32_t serverid;
	uint32_t uid;

	virtual void marshal(sox::Pack &p) const {
		p << serverid << uid;
		PFindResult::marshal(p);
	}
		                
	virtual void unmarshal(const sox::Unpack &p) {
		p >> serverid >> uid;
		PFindResult::unmarshal(p);
	}
};


#endif
