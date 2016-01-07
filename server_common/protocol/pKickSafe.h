#ifndef _PKICKSAFE_H_
#define _PKICKSAFE_H_

#include "int_types.h"
#include "core/base_svid.h"
#include "protocol/const.h"
#include "packet.h"
#include <string>
#include <vector>

namespace server{
	namespace kick_safe{

		struct PReadyToKick : public sox::Marshallable{
			enum {uri = (2 << 8 | 50)};
			uint32_t uid;
			uint32_t ip;
			std::string kick_type_str;
			std::string src_type; //如果没有就与kick_type_str一致

			PReadyToKick() : uid(0) {}

			virtual void marshal(sox::Pack &pk) const {
				pk << uid << ip << kick_type_str << src_type;
			}
			virtual void unmarshal(const sox::Unpack &up) {
				up >> uid >> ip >> kick_type_str >> src_type;
			}
		};

		struct PReadyToKick2 : public sox::Marshallable{
			enum {uri = (3 << 8 | 50)};
			uint32_t uid;
			uint32_t ip;//该账号登陆ip
			std::string kick_type_str;
			std::string src_type; //如果没有就与kick_type_str一致
			std::string note; //踢人原因
			uint32_t time_interval; //单位是小时,软封填87600
			uint32_t ban_type; //自定义一个整形值

			PReadyToKick2() 
				: uid(0) 
				, ip(0)
				, time_interval(0)
				, ban_type(10000)
				{}

			virtual void marshal(sox::Pack &pk) const {
				pk << uid << ip << kick_type_str << src_type << note << time_interval << ban_type;
			}
			virtual void unmarshal(const sox::Unpack &up) {
				up >> uid >> ip >> kick_type_str >> src_type >> note >> time_interval >> ban_type;
			}
		};
	}
}

#endif
