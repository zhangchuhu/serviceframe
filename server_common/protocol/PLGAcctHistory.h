/*
 * PLGAcctHistory.h
 *
 *  Created on: Sep 26, 2011
 *      Author: jeffcai
 */

#ifndef PLGACCTHISTORY_H_
#define PLGACCTHISTORY_H_

#include "common/packet.h"
#include "common/core/base_svid.h"
#include "common/protocol/const.h"
#include <string>

namespace server {
namespace login {

struct PLGAcctInfoLastRecord: public sox::Marshallable {
	uint32_t loginTime;
	uint32_t logoutTime;
	uint32_t ip;

	virtual void marshal(sox::Pack &p) const {
		p << loginTime << logoutTime << ip;
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> loginTime >> logoutTime >> ip;
	}
};

struct PLGGetAcctLastInfoBatchReq: public sox::Marshallable {
	enum {
		uri = (1 << 8 | protocol::LG_ACCT_HISTORY_SVID)
	};

	std::vector<uint32_t> uids;
	uint32_t contextId;
	std::string contextText;

	virtual void marshal(sox::Pack &p) const {
		marshal_container(p, uids);
		p << contextId << contextText;
	}

	virtual void unmarshal(const sox::Unpack &p) {
		unmarshal_container(p, std::back_inserter(uids));
		p >> contextId >> contextText;
	}

};

struct PLGGetAcctLastInfoBatchRes: public sox::Marshallable {
	enum {
		uri = (2 << 8 | protocol::LG_ACCT_HISTORY_SVID)
	};

	std::map<uint32_t, PLGAcctInfoLastRecord> infos;

	uint32_t contextId;
	std::string contextText;

	virtual void marshal(sox::Pack &p) const {
		p << contextId << contextText;

		uint32_t infoSize = infos.size();
		p << infoSize;

		for (std::map<uint32_t, PLGAcctInfoLastRecord>::const_iterator i=infos.begin(); i != infos.end(); ++i) {
			p << i->first;
			p << i->second;
		}
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> contextId >> contextText;
		uint32_t infoSize = 0;

		p >> infoSize;

		uint32_t uid;
		PLGAcctInfoLastRecord record;

		for(uint32_t i=0;i<infoSize;i++) {
			p >> uid;
			p >> record;

			infos[uid] = record;
		}
	}

};

struct PLGSendLastInfoMsg: public sox::Marshallable {
	enum {
		uri = (10 << 8 | protocol::LG_ACCT_HISTORY_SVID)
	};

	uint32_t uid;
	uint32_t loginTime;
	uint32_t logoutTime;
	uint32_t ip;

	virtual void marshal(sox::Pack &p) const {
		p << uid << loginTime << logoutTime << ip;
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> uid >> loginTime >> logoutTime >> ip;
	}

};

struct PLGAccountOpsMsg: public sox::Marshallable {
        enum {
                uri = (12 << 8 | protocol::LG_ACCT_HISTORY_SVID)
        };

        uint32_t uid;
	uint32_t time;
	uint16_t op;
        uint32_t ip;

        virtual void marshal(sox::Pack &p) const {
                p << uid << time << op << ip;
        }

        virtual void unmarshal(const sox::Unpack &p) {
                p >> uid >> time >> op >> ip;
        }

};


}
}

#endif /* PLGACCTHISTORY_H_ */
