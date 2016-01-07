#ifndef PROTOCOL_ROUTER_H
#define PROTOCOL_ROUTER_H
#include "common/res_code.h"
#include "common/packet.h"
#include "common/core/base_svid.h"
#include "core/corelib/RouterBase.h"

namespace common{
namespace router{
	// 数据经linkd实现透传
	struct PTTransmit : public core::RouterBase{
		enum {uri = 3 << 8 | protocol::ROUTE_SVID};
		std::string toNameSrv;

		virtual void marshal(sox::Pack &pk) const{
			core::RouterBase::marshal(pk);
			pk << toNameSrv;
		}
		virtual void unmarshal(const sox::Unpack &up){
			core::RouterBase::unmarshal(up);
			up >> toNameSrv;
		}

		virtual int emit(core::IConn* /*conn*/, core::IServerIdDispatcher* ) { return 0; }
		virtual void gc(core::IPacketGc *) {}
	};

/*
struct PBatchRouter: public sox::Marshallable{
	enum {uri = 1 << 8 | protocol::ROUTE_SVID};
	std::string from;
	std::vector<std::string> tos;
	uint8_t ttl;
	uint32_t ruri;
	uint16_t resCode;
	std::string load;
	PRouter():ttl(2), resCode(RES_SUCCESS){}

	virtual void marshal(sox::Pack &pk) const{
		pk.push_uint8(ttl);
		pk << from << ruri << resCode << load;
		sox::marshal_container(pk, tos);

	}
	virtual void unmarshal(const sox::Unpack &up){
		ttl = up.pop_uint8();
		up >> from >> ruri >> resCode >> load;
		sox::unmarshal_container(up, std::back_inserter(tos));
	}
};*/

}
}
#endif

