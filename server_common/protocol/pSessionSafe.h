#ifndef _PSESSION_SAFE_H_
#define _PSESSION_SAFE_H_

#include "int_types.h"
#include "core/base_svid.h"
#include "protocol/const.h"
#include "packet.h"
#include <string>
#include <vector>

namespace server{
	namespace session_safe{
	
		struct PSessionUpload : public sox::Marshallable{
			enum {uri = (61 << 8 | 9)};

			uint32_t uid;
			uint32_t ip; //网络字节序
			uint32_t time; //时间戳
			uint32_t top_sid; //主频道id
			uint32_t sub_sid; //子频道id
			std::string pc_info;
			
			PSessionUpload()
				: uid(0)
				, ip(0)
				, time(0)
				, top_sid(0)
				, sub_sid(0)
				, pc_info()
				{}

			virtual void marshal(sox::Pack &p) const{
				p << uid << ip << time << top_sid << sub_sid << pc_info;
			}
			virtual void unmarshal(const sox::Unpack &p){
				p >> uid >> ip >> time >> top_sid >> sub_sid >> pc_info;
			}
		};
	}
}

#endif
