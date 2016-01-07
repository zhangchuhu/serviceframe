/*
 * PSimpleCacheUpdate.h
 *
 *  Created on: 2011-3-24
 *      Author: Caizhi
 */

#ifndef PSIMPLECACHEUPDATE_H_
#define PSIMPLECACHEUPDATE_H_

#include "common/packet.h"
#include "common/core/base_svid.h"
#include "common/protocol/const.h"

struct PSimpleCacheUpdate: public sox::Marshallable {
	enum {uri = (7 << 8 | protocol::YAUTHD_SVID)};

	uint32_t updatedKey;

	virtual void marshal(sox::Pack &p) const {
		p << updatedKey;
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> updatedKey;
	}
};

#endif /* PSIMPLECACHEUPDATE_H_ */
