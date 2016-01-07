#pragma once 

#include "common/unistr.h"
#include "common/int_types.h"
#include "common/protocol/const.h"

namespace protocol
{
	namespace glistcache
	{

		//群成员状态改变通知接口

		class IGUserStatsWatcher
		{
		public:
			virtual ~IGUserStatsWatcher() {};

			//批量用户状态通知

			virtual void OnGetGUserStats(const std::map<uint32_t,protocol::E_IMSTATUS> &mapGUserStats) = 0;

			//单个用户状态通知

			virtual void OnGUserStatChange(//const uint32_t				&uGrpID,	//群ID
										   const uint32_t				&uUid,		//用户ID
										   const protocol::E_IMSTATUS	&emState	//用户状态
										   ) = 0;
		};

		//欢察者操作接口定义

		class IOpGUserStatsWatcher
		{
		public:
			virtual ~IOpGUserStatsWatcher() {};

			virtual void watch(IGUserStatsWatcher *w) = 0;
			virtual void revoke(IGUserStatsWatcher *w) = 0;
		};

		//群成员状态改变接口定义

		class IGUserStats : public virtual IOpGUserStatsWatcher
		{
		public:
			virtual ~IGUserStats() {};

			//批量请求用户的状态

			virtual void GetGUserStats(const std::vector<uint32_t> &vecUid) = 0;
		};

	}
}