/*
 * PTagUser.h
 *
 *  Created on: Mar 13, 2013
 *      Author: jeffcai
 */

#ifndef PTAGUSER_H_
#define PTAGUSER_H_

#include "common/packet.h"
#include "common/core/base_svid.h"
#include "common/protocol/const.h"
#include <string>
#include <map>

namespace server {
namespace login {

struct PTagUserMsg: public sox::Marshallable {
	enum {
		uri = (110 << 8 | protocol::LG_GENERIC_INTERNAL_SVID)
	};

	uint32_t uid;
	uint32_t ip;	// in network order
	uint32_t time;
	std::string tag;

	virtual void marshal(sox::Pack &p) const {
		p << uid << ip << time << tag;
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> uid >> ip >> time >> tag;
	}

};

}
}

#endif /* PTAGUSER_H_ */
