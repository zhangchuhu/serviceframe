#include "redis_helper.h"


#include "vip_dao/redis_packet.h"
#include "../vip_dao/SingleInstance.h"

using namespace std;

using namespace libredis;
using namespace sox;
using namespace protocol;

bool RedisHelper::redisSet(const std::string& key, protocol::ImUserStatusCommon& im_user_status)
{
	RedisPacket  ss;
	ss<<im_user_status.m_uUid<<im_user_status.m_uLinkdId<<im_user_status.m_status;
	RedisFactory* factory=SingleDaoIns<RedisFactory>::getInstance();
	RedisStringHelper string_helper(*(factory->GetRedisByName(redis_name)));
	return string_helper.set(key,ss.data());
}
bool RedisHelper::redisGet(const std::string& key, protocol::ImUserStatusCommon& im_user_status)
{
	RedisFactory* factory=SingleDaoIns<RedisFactory>::getInstance();
	RedisStringHelper string_helper(*(factory->GetRedisByName(redis_name)));
	string value;
	
	if (!string_helper.get(key,value))
	{
		return false;
	}
	uint32_t status;
	RedisPacket ss(value);
	ss>>im_user_status.m_uUid>>im_user_status.m_uLinkdId>>status;
	im_user_status.m_status=(APP_IMSTATUS) status;
	return true;
}