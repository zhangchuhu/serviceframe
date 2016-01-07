#ifndef __REDIS_HELPER_H__
#define __REDIS_HELPER_H__
#include <string>
#include <sstream>

#include "../libredis/libredis.h"
#include "../app_protocol/app_protocol.h"

namespace libredis{
	class RedisHelper{

	public:

		RedisHelper(const std::string redis_name_): redis_name(redis_name_) {}

		~RedisHelper(){}

		inline std::string uid2Key(uint32_t uid)
		{
			std::stringstream ss;
			ss << "im_online_" << uid;
			return ss.str();
		}

		bool redisSet(const std::string& key, protocol::ImUserStatusCommon& im_user_status);

		bool redisGet(const std::string& key, protocol::ImUserStatusCommon& im_user_status);	

	private:

		std::string redis_name;

	};

}
#endif