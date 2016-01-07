#pragma once

#include "common/packet.h"
#include "common/core/base_svid.h"
#include "common/protocol/const.h"
#include <string>

namespace server {
namespace login {

struct PLGIsFreqIpReq: public sox::Marshallable {
	enum {
		uri = (1 << 8 | protocol::LG_FREQ_IP_SVID)
	};

	uint32_t yyuid;
	std::string ip;
	uint32_t contextId;
	std::string contextText;

	virtual void marshal(sox::Pack &p) const {
		p << yyuid << ip << contextId << contextText;
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> yyuid >> ip >> contextId >> contextText;
	}

};

struct PLGIsFreqIpRes: public sox::Marshallable {
	enum {
		uri = (2 << 8 | protocol::LG_FREQ_IP_SVID)
	};

	uint16_t res_code;
	bool result;
	uint32_t contextId;
	std::string contextText;

	virtual void marshal(sox::Pack &p) const {
		p << res_code << result << contextId << contextText;
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> res_code >> result >> contextId >> contextText;
	}

};

}
}
