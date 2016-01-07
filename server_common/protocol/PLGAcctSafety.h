/*
 * PLGAcctSafety.h
 *
 *  Created on: Sep 19, 2011
 *      Author: jeffcai
 */

#ifndef PLGACCTSAFETY_H_
#define PLGACCTSAFETY_H_

#include "common/packet.h"
#include "common/core/base_svid.h"
#include "common/protocol/const.h"
#include <string>

namespace server {
namespace login {

struct PLGGetAcctSafetyLevelReq: public sox::Marshallable {
	enum {
		uri = (101 << 8 | protocol::LG_ACCT_SAFETY_SVID)
	};

	uint32_t yyuid;
	uint32_t contextId;
	std::string contextText;

	virtual void marshal(sox::Pack &p) const {
		p << yyuid << contextId << contextText;
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> yyuid >> contextId >> contextText;
	}

};

struct PLGGetAcctSafetyLevelRes: public sox::Marshallable {
	enum {
		uri = (102 << 8 | protocol::LG_ACCT_SAFETY_SVID)
	};

	uint16_t res_code;
	uint16_t safetyLevelScore;
	std::string safetyLevelDescription;

	uint32_t contextId;
	std::string contextText;

	virtual void marshal(sox::Pack &p) const {
		p << res_code << safetyLevelScore << safetyLevelDescription << contextId << contextText;
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> res_code >> safetyLevelScore >> safetyLevelDescription >> contextId >> contextText;
	}

};

struct PLGAcctSafetyLevelSelfNotify: public sox::Marshallable {
	enum {
		uri = (103 << 8 | protocol::LG_ACCT_SAFETY_SVID)
	};

	uint32_t uid;

	virtual void marshal(sox::Pack &p) const {
		p << uid;
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> uid;
	}

};

struct PLGAcctSafetyLevelCheckCache: public sox::Marshallable {
	enum {
		uri = (104 << 8 | protocol::LG_ACCT_SAFETY_SVID)
	};

	uint32_t uid;
	uint16_t score;

	virtual void marshal(sox::Pack &p) const {
		p << uid << score;
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> uid >> score;
	}

};

struct PLGGetUserMobileStatusReq : sox::Marshallable {
        enum {
                uri = (110 << 8 | protocol::LG_ACCT_SAFETY_SVID)
        };

        uint32_t uid;
	uint32_t contextId;

        virtual void marshal(sox::Pack &p) const {
                p << uid << contextId;
        }

        virtual void unmarshal(const sox::Unpack &p) {
                p >> uid >> contextId;
        }

};

struct PLGGetUserMobileStatusRes : sox::Marshallable {
        enum {
                uri = (111 << 8 | protocol::LG_ACCT_SAFETY_SVID)
        };

        uint32_t uid;
        uint32_t contextId;
	bool isBind;

        virtual void marshal(sox::Pack &p) const {
                p << uid << contextId << isBind;
        }

        virtual void unmarshal(const sox::Unpack &p) {
                p >> uid >> contextId >> isBind;
        }

};



}
}



#endif /* PLGACCTSAFETY_H_ */
