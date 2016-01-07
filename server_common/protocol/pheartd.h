#ifndef __PROTOCOL_HEARTD_H_
#define __PROTOCOL_HEARTD_H_

#include "common/core/base_svid.h"
#include "common/packet.h"
#include "common/protocol/const.h"
#include <vector>

namespace server
{
		namespace heartd
		{
				enum{ heartd_listen_port = 1500};

				struct PAllocPort : public sox::Marshallable {
						PAllocPort(){
								number = 1;
								pid	   = 0;
						}
						enum {uri = (1 << 8 | 15)};
						uint16_t number;		//		请求的数目
						uint16_t pid;			//		 进程id
						uint16_t svid;			//      服务器类型id		

						virtual void marshal(sox::Pack & pk) const{
								pk << number << pid << svid;
						}
						virtual void unmarshal(const sox::Unpack & upk){
								upk >> number >> pid >> svid;
						}
				};

				struct PAllocPortRes {
						enum {uri = (1 << 8 | 15)};
						std::vector<uint16_t> ports;

						virtual void marshal(sox::Pack & pk) const{
								sox::marshal_container(pk, ports);
						}
						virtual void unmarshal(const sox::Unpack & upk){
								sox::unmarshal_container(upk, std::back_inserter(ports));
						}
						
				};


		}
}

#endif
