#pragma once

#include <vector>
#include <string>
#include "protocol/const.h"
#include "core/base_svid.h"

namespace server {
namespace yauth {

struct POpenIDLogRequest: public sox::Marshallable {
	enum {
		uri = (33 << 8) | protocol::YAUTHD_SVID
	};

	uint32_t appId;
	uint32_t uid;
	time_t time;
	int code;

	virtual void marshal(sox::Pack &p) const {
		p << appId << uid << (uint32_t) time << (uint32_t) code; // force
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> appId >> uid;
		uint32_t tmp;
		p >> tmp;
		time = (time_t) tmp;
		
		p >> tmp;
		code = (int) tmp;
	}
};

}
}
