#ifndef _TST_ANY_PACK_H_
#define _TST_ANY_PACK_H_

#include "common/core/base_svid.h"
#include "common/packet.h"
#include "common/res_code.h"
#include <vector>
#include "core/corelib/RouterBase.h"
#include "common/core/sender.h"
#include <assert.h>

namespace server{
	namespace router{
		struct stringmarshallable  : public sox::Marshallable
		{
			std::string load;
			virtual void marshal(sox::Pack &pk) const{
				pk.push(load.data(),load.size());
			}
			virtual void unmarshal(const sox::Unpack &up){
				assert(false);
			}
		};
		struct PassAnyPack  : public sox::Marshallable
		{
			enum {uri = 158 << 8 | ::protocol::ROUTE_SVID};
			std::string dest;
			uint32_t loaduri;
			std::string load;
			virtual void marshal(sox::Pack &pk) const{
				pk << dest;
				pk << loaduri;
				pk << load;
			}
			virtual void unmarshal(const sox::Unpack &up){
				up >> dest;
				up >> loaduri;
				up >> load;
			}
		};

    }
}
#endif

