#ifndef _PULOCATE_H_
#define _PULOCATE_H_

#include "core/base_svid.h"
#include "int_types.h"

namespace server{
	namespace ulocator{
		struct PFreeBucket : public sox::Marshallable{
			PFreeBucket(){}
			PFreeBucket(uint32_t buc) : bucket(buc) {}

			enum{uri =  (133 << 8 | protocol::LINKD_SVID)};

			uint32_t bucket;

			virtual void marshal(sox::Pack &p) const{
				p << bucket;
			}
			virtual void unmarshal(const sox::Unpack &p){
				p >> bucket;
			}
		};
	}
}
#endif

