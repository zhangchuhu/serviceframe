#pragma once

#include "const.h"
#include "common/int_types.h"


namespace protocol
{
	namespace vip
	{
		enum EOnlinePriOp
		{
			ENUM_SET_HIDE_TO = 0,        // 设置在线对其隐身
			ENUM_SET_UNHIDE_TO,          // 设置隐身对其可见
			ENUM_UNSET_HIDE_TO,          // 取消在线对其隐身
			ENUM_UNSET_UNHIDE_TO         // 取消隐身对其可见
		};

		enum EOnlinePriResCode
		{
			ENUM_ONLINE_PRI_OK = 0,      // 设置成功
			ENUM_ONLINE_PRI_OVERLIMIT,   // 设置人数超过限制
			ENUM_ONLINE_PRI_NOT_VIP,     // 用户非vip
			ENUM_ONLINE_PRI_DBERR
		};

		class IVipOnlineWatcher
		{
		public:

			virtual ~IVipOnlineWatcher() {}

			// 设置在线对其隐身 / 隐身对其可见的 响应
			virtual void onSetOnlinePrivilegeRes(const uint32_t& uBuddyId, const EOnlinePriOp& emOp, const EOnlinePriResCode& res_code) = 0;

			// 取得我设置过的好友列表
			// @uMaxNum  用户当前等级可关注的最大好友数
			virtual void onGetOnlinePirvilege(const std::set<uint32_t>& hideList, const std::set<uint32_t>& unHideList, const uint32_t& uMaxNum) = 0;
		};

		class IVipOnlineWatcherBase
		{
		public:

			virtual ~IVipOnlineWatcherBase() {}
			virtual void watch(IVipOnlineWatcher *w) = 0;
			virtual void revoke(IVipOnlineWatcher *w) = 0;
		};

		class ICVipOnlinePrivilege : public virtual IVipOnlineWatcherBase
		{
		public:
		
			// set online privilege
			virtual void setOnlinePrivilege(uint32_t uBuddyId, EOnlinePriOp emOp) = 0;
		};
	}
}

