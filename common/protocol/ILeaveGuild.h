#ifndef ILeaveGuild_h__
#define ILeaveGuild_h__
#include "ChannelRoler.h"
#include "common/int_types.h"

namespace protocol{
	namespace session{
		struct LeaveGuildInfo{
			std::wstring name;
			std::wstring sid;
			std::wstring asid;
			ChannelRoler oldRoler;
			uint32_t jifen;
			time_t leaveTime;			
			time_t clearJifenTime;
			std::wstring adminOldName;		
			bool isAutoLeave;				//是主动离开还是被管理员踢掉
		};

		struct ILeaveGuildWatcher{
			virtual void onRefresh(const std::vector<LeaveGuildInfo> &) = 0;
		};

		struct ILeaveGuildWatchBase{
			virtual void watch(ILeaveGuildWatcher *) = 0;
			virtual void revoke(ILeaveGuildWatcher *) = 0;
		};
		//如果已经去过，将不再从服务器取，只有在注销后才会再去取。
		struct ILeaveGuild: public ILeaveGuildWatchBase{
			virtual ~ILeaveGuild(){}
			//清空列表
			virtual void clear() = 0;
			//调用该接口则发起一次服务器同步
			virtual void sync() = 0;
		};
	}
}
#endif // ILeaveGuild_h__