#ifndef PJIFEN_H_
#define PJIFEN_H_

#include "common/core/base_svid.h"
#include "common/protocol/psinfo.h"
#include "common/packet.h"
namespace server {

namespace jifen {

enum id_status
{
	INIT = 0,			//初始化,频道不可用

	STAGE1 = 1,			//审核阶段1初始化
	STAGE1_FAIL = 11,	//审核阶段1失败
	STAGE1_SUCC = 12,	//审核阶段1成功

	STAGE2 = 2,			//审核阶段2初始化
	STAGE2_FAIL = 21,	//审核阶段1失败
	STAGE2_SUCC = 22	//审核阶段1成功
};

struct PUserOnlineStart : public sox::Marshallable {
	enum {uri = (1 << 8) |protocol::ONLINE_SVID};
	uint32_t serverID;
	uint32_t uid;
	virtual void marshal(sox::Pack &pk) const {
		pk << serverID << uid ;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> serverID >> uid;
	}
};

struct PUserOnlineOff : public PUserOnlineStart {
	enum {uri = (2 << 8) |protocol::ONLINE_SVID};
};

}
}
#endif /*PJIFENG_H_*/
